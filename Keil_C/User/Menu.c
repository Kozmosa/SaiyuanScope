#include "Menu.h"
#include <stdio.h>  
#include "Data_Acquisition.h" // 【关键】引入头文件以读取 Acq_Data
#include "TFT_Lcd.h"          

// ==================================================================
//  1. 外部变量引用声明 (保持不变)
// ==================================================================
extern uint8_t menu_page;           
extern uint8_t menu_choice;         
extern uint16_t key_count;          
extern uint8_t KeyValue;            
extern uint16_t Encoder_Count;      
extern uint16_t Encoder_Count_last; 
extern uint8_t Encoder_mode;        

extern uint8_t is_show[10];         
extern uint8_t CH1_TIME_DIV_mode;   
extern uint8_t CH1_VOLT_DIV_mode;   
extern uint8_t CH2_TIME_DIV_mode;   
extern uint8_t CH2_VOLT_DIV_mode;   
extern uint8_t Trigger_mode;        

extern int16_t Horiziontal1_offset;
extern int16_t Vertical1_offset;
extern int16_t Horiziontal2_offset;
extern int16_t Vertical2_offset;

extern uint8_t Wave_show_mode;
extern uint8_t show_wave_save_count;
extern uint8_t wave_save_count;
extern uint8_t uart3_enable;

extern char* CH1_TIME_DIV[];
extern char* CH1_VOLT_DIV[];
extern char* CH2_TIME_DIV[];
extern char* CH2_VOLT_DIV[];
extern char* Trigger[];

extern uint16_t wave1_value[];
extern uint16_t wave2_value[];
extern uint16_t wave1_save_value[5][200]; 
extern uint16_t wave2_save_value[5][200];

// 【新增】定义一个缓冲区，用于存放转换后的字符串
static char disp_buf[32]; 

// ==================================================================
//  2. 私有辅助函数 (保持不变)
// ==================================================================

// 处理旋钮方向
static int Handle_Encoder(void) 
{
    int direction = 0;
    if (Encoder_Count > Encoder_Count_last) direction = 1;
    else if (Encoder_Count < Encoder_Count_last) direction = -1;

    if (direction != 0) {
        Encoder_Count = 10000;
        Encoder_Count_last = 10000;
    }
    return direction;
}

// 清除菜单区域
static void Clear_Menu_Area(void) 
{
    for(int i = 0; i <= 15; i++) {
        LCD_ShowString(207, 16 * i, "               ", MENU_BG_COLOR, BLACK, 16);
    }
}

// 显示菜单文本 (带高亮逻辑)
static void Draw_Menu_Item_Str(uint16_t x, uint16_t y, char* str, uint8_t current_choice, uint8_t my_index) 
{
    uint16_t color = (current_choice == my_index) ? MENU_TEXT_SEL : MENU_TEXT_UNSEL;
    LCD_ShowString(x, y, str, MENU_BG_COLOR, color, 16);
}

// 显示菜单数字 (带高亮逻辑)
static void Draw_Menu_Item_Num(uint16_t x, uint16_t y, int num, uint8_t current_choice, uint8_t my_index) 
{
    uint16_t color = (current_choice == my_index) ? MENU_TEXT_SEL : MENU_TEXT_UNSEL;
    if (num >= 0) LCD_ShowString(x - 8, y, "+", MENU_BG_COLOR, color, 16);
    else          LCD_ShowString(x - 8, y, "-", MENU_BG_COLOR, color, 16);
    LCD_ShowIntNum(x, y, (num >= 0 ? num : -num), 3, MENU_BG_COLOR, color, 16);
}

// ==================================================================
//  3. 主菜单逻辑函数
// ==================================================================
void menu(void)
{
    // Page 0: 初始化/清屏
    if (menu_page == 0)
    {
        Clear_Menu_Area();
        menu_page = 1;
        return; 
    }

    // ------------------------------------------------------------------
    // Page 1: 监视页面 (修改部分：显示采集到的原始值)
    // ------------------------------------------------------------------
    if (menu_page == 1)
    {
        // --- 输入处理 ---
        int enc_dir = Handle_Encoder();
        
        if (KeyValue == 8 || enc_dir == 1) { 
            menu_choice++;
            if (menu_choice > 12) menu_choice = 12;
            KeyValue = 0;
        }
        else if (KeyValue == 7 || enc_dir == -1) { 
            menu_choice--;
            if (menu_choice < 1) menu_choice = 1;
            KeyValue = 0;
        }

        // 确认键逻辑
        if (key_count >= 1) {
            key_count = 0; 
            if (menu_choice <= 10) {
                is_show[menu_choice - 1] ^= 1; 
            }
            else if (menu_choice == 12) {
                menu_page = 2; 
                menu_choice = 21;
                return;
            }
        }

        // --- 显示静态标签 (稍微修改了文字以匹配Raw显示) ---
        LCD_ShowString(270, 0,   "CH1",  MENU_BG_COLOR, BLACK, 16);
        LCD_ShowString(210, 16,  "Raw",  MENU_BG_COLOR, BLACK, 16); // 原MAX改为Raw
        LCD_ShowString(210, 32,  "Volt", MENU_BG_COLOR, BLACK, 16); // 原MIN改为Volt(模拟电压)
        LCD_ShowString(210, 48,  "AMP",  MENU_BG_COLOR, BLACK, 16);
        LCD_ShowString(210, 64,  "Freq", MENU_BG_COLOR, BLACK, 16);
        LCD_ShowString(210, 80,  "DUTY", MENU_BG_COLOR, BLACK, 16);
        
        LCD_ShowString(270, 100, "CH2",  MENU_BG_COLOR, BLACK, 16);
        LCD_ShowString(210, 116, "Raw",  MENU_BG_COLOR, BLACK, 16);
        LCD_ShowString(210, 132, "Volt", MENU_BG_COLOR, BLACK, 16);
        LCD_ShowString(210, 148, "AMP",  MENU_BG_COLOR, BLACK, 16);
        LCD_ShowString(210, 164, "Freq", MENU_BG_COLOR, BLACK, 16);
        LCD_ShowString(210, 180, "DUTY", MENU_BG_COLOR, BLACK, 16);

        // ========================================================
        // 动态数据项：显示采集到的数值
        // ========================================================
        
// ========================================================
// 动态数据项：显示采集到的数值
// ========================================================

        // --- CH1 数据 ---
        if (is_show[0]) {
            // 1. 显示原始ADC值 (0~16383)
            // 使用 %5d 以便对齐5位数字
            sprintf(disp_buf, "%5d", Acq_Data.Buffer[0]); 
            Draw_Menu_Item_Str(260, 16, disp_buf, menu_choice, 1);

            // 2. 转换成电压显示 (3.3V 参考电压, 14位精度)
            // 公式修改：除以 16383.0f
            float voltage = (float)Acq_Data.Buffer[0] * 3.3f / 16383.0f;
            
            sprintf(disp_buf, "%.2fV", voltage);
            Draw_Menu_Item_Str(260, 32, disp_buf, menu_choice, 2);
        } else {
            Draw_Menu_Item_Str(270, 16, "OFF", menu_choice, 1);
            Draw_Menu_Item_Str(270, 32, "OFF", menu_choice, 2);
        }

        // ... 中间代码 ...

        // --- CH2 数据 ---
        if (is_show[5]) {
            // 1. 显示原始ADC值
            sprintf(disp_buf, "%5d", Acq_Data.Buffer[1]);
            Draw_Menu_Item_Str(260, 116, disp_buf, menu_choice, 6);

            // 2. 显示电压 (14位精度)
            float voltage = (float)Acq_Data.Buffer[1] * 3.3f / 16383.0f;
            
            sprintf(disp_buf, "%.2fV", voltage);
            Draw_Menu_Item_Str(260, 132, disp_buf, menu_choice, 7);
        } else {
            Draw_Menu_Item_Str(270, 116, "OFF", menu_choice, 6);
            Draw_Menu_Item_Str(270, 132, "OFF", menu_choice, 7);
}

        // 其他参数暂时留空
        Draw_Menu_Item_Str(270, 148, "----", menu_choice, 8);
        Draw_Menu_Item_Str(270, 164, "----", menu_choice, 9);
        Draw_Menu_Item_Str(270, 180, "----", menu_choice, 10);

        Draw_Menu_Item_Str(250, 196, "AUTOSET", menu_choice, 11);
        Draw_Menu_Item_Str(270, 224, "->",      menu_choice, 12);
    }

    // ... (Page 2 和 Page 3 的代码保持原样，不用修改) ...
    // Page 2: 初始化/清屏
    if (menu_page == 2)
    {
        Clear_Menu_Area();
        menu_page = 3;
        return;
    }

    // Page 3: 控制页面
    if (menu_page == 3)
    {
        // ... (此处粘贴你原来 Page 3 的所有代码) ...
        int enc_dir = Handle_Encoder();

        // 输入逻辑
        if (Encoder_mode == 0) { 
            if (enc_dir == 1) { menu_choice++; if (menu_choice > 38) menu_choice = 38; }
            else if (enc_dir == -1) { menu_choice--; if (menu_choice < 21) menu_choice = 21; }
        }
        else if (Encoder_mode == 1) { 
            // ... (编辑模式逻辑保持不变) ...
            if (enc_dir != 0) {
                switch(menu_choice) {
                    case 21: if(enc_dir==1 && CH1_TIME_DIV_mode<17) CH1_TIME_DIV_mode++; else if(enc_dir==-1 && CH1_TIME_DIV_mode>0) CH1_TIME_DIV_mode--; break;
                    case 22: if(enc_dir==1 && CH1_VOLT_DIV_mode<7)  CH1_VOLT_DIV_mode++; else if(enc_dir==-1 && CH1_VOLT_DIV_mode>0) CH1_VOLT_DIV_mode--; break;
                    case 23: if(enc_dir==1 && CH2_TIME_DIV_mode<17) CH2_TIME_DIV_mode++; else if(enc_dir==-1 && CH2_TIME_DIV_mode>0) CH2_TIME_DIV_mode--; break;
                    case 24: if(enc_dir==1 && CH2_VOLT_DIV_mode<7)  CH2_VOLT_DIV_mode++; else if(enc_dir==-1 && CH2_VOLT_DIV_mode>0) CH2_VOLT_DIV_mode--; break;
                    case 25: if(enc_dir==1 && Trigger_mode<2)       Trigger_mode++;      else if(enc_dir==-1 && Trigger_mode>0)      Trigger_mode--;      break;
                    case 26: Horiziontal1_offset += enc_dir; if(Horiziontal1_offset > 0) Horiziontal1_offset = 0; if(Horiziontal1_offset < -19) Horiziontal1_offset = -19; break;
                    case 27: Vertical1_offset += enc_dir;    if(Vertical1_offset > 10) Vertical1_offset = 10;   if(Vertical1_offset < -10) Vertical1_offset = -10;    break;
                    case 28: Horiziontal2_offset += enc_dir; if(Horiziontal2_offset > 0) Horiziontal2_offset = 0; if(Horiziontal2_offset < -19) Horiziontal2_offset = -19; break;
                    case 29: Vertical2_offset += enc_dir;    if(Vertical2_offset > 10) Vertical2_offset = 10;   if(Vertical2_offset < -10) Vertical2_offset = -10;    break;
                    case 34: if(enc_dir==1) { show_wave_save_count++; if(show_wave_save_count>4) show_wave_save_count=0; } else { if(show_wave_save_count>0) show_wave_save_count--; else show_wave_save_count=4; } Wave_show_mode=8; break;
                }
            }
        }

        // 确认键逻辑
        if (key_count >= 1) {
            key_count = 0;
            if ((menu_choice >= 21 && menu_choice <= 29) || menu_choice == 30 || menu_choice == 34 || menu_choice == 36 || menu_choice == 37) {
                Encoder_mode = !Encoder_mode;
                if (menu_choice == 30) Wave_show_mode = (Encoder_mode == 1) ? 6 : 0; 
                if (menu_choice == 34) Wave_show_mode = (Encoder_mode == 1) ? 8 : 0; 
                if (menu_choice == 36) Wave_show_mode = (Encoder_mode == 1) ? 2 : 0; 
                if (menu_choice == 37) Wave_show_mode = (Encoder_mode == 1) ? 4 : 0; 
            }
            else if (menu_choice == 31) uart3_enable = 1; 
            else if (menu_choice == 33) {
                // Save logic
                int i;
                for(i=0; i<=198; i++) {
                    wave1_save_value[wave_save_count][i] = wave1_value[200+i];
                    wave2_save_value[wave_save_count][i] = wave2_value[200+i];
                }
                wave_save_count++; if(wave_save_count >= 5) wave_save_count = 0;
            }
            else if (menu_choice == 38) { menu_choice = 1; menu_page = 0; }
        }

        // Page 3 显示处理
        LCD_ShowString(210, 0,  "CH1", MENU_BG_COLOR, BLACK, 16);
        Draw_Menu_Item_Str(250, 0,  CH1_TIME_DIV[CH1_TIME_DIV_mode], menu_choice, 21);
        Draw_Menu_Item_Str(250, 16, CH1_VOLT_DIV[CH1_VOLT_DIV_mode], menu_choice, 22);

        LCD_ShowString(210, 32, "CH2", MENU_BG_COLOR, BLACK, 16);
        Draw_Menu_Item_Str(250, 32, CH2_TIME_DIV[CH2_TIME_DIV_mode], menu_choice, 23);
        Draw_Menu_Item_Str(250, 48, CH2_VOLT_DIV[CH2_VOLT_DIV_mode], menu_choice, 24);

        LCD_ShowString(210, 64, "TRG", MENU_BG_COLOR, BLACK, 16);
        Draw_Menu_Item_Str(250, 64, Trigger[Trigger_mode], menu_choice, 25);

        LCD_ShowString(210, 80,  "HPOS1", MENU_BG_COLOR, BLACK, 16); Draw_Menu_Item_Num(270, 80,  Horiziontal1_offset, menu_choice, 26);
        LCD_ShowString(210, 96,  "VPOS1", MENU_BG_COLOR, BLACK, 16); Draw_Menu_Item_Num(270, 96,  Vertical1_offset,    menu_choice, 27);
        LCD_ShowString(210, 112, "HPOS2", MENU_BG_COLOR, BLACK, 16); Draw_Menu_Item_Num(270, 112, Horiziontal2_offset, menu_choice, 28);
        LCD_ShowString(210, 128, "VPOS2", MENU_BG_COLOR, BLACK, 16); Draw_Menu_Item_Num(270, 128, Vertical2_offset,    menu_choice, 29);

        Draw_Menu_Item_Str(205, 144, "STOP",   menu_choice, 30);
        Draw_Menu_Item_Str(238, 144, "SEND1",  menu_choice, 31);
        Draw_Menu_Item_Str(279, 144, "CLEAR",  menu_choice, 32);
        Draw_Menu_Item_Str(210, 160, "SAVE",   menu_choice, 33);
        Draw_Menu_Item_Str(260, 160, "VIEW",   menu_choice, 34);
        Draw_Menu_Item_Str(230, 176, "DECODE", menu_choice, 35);
        Draw_Menu_Item_Str(210, 192, "FFT1",   menu_choice, 36);
        Draw_Menu_Item_Str(260, 192, "FFT2",   menu_choice, 37);
        Draw_Menu_Item_Str(270, 224, "<-",     menu_choice, 38);
    }
}