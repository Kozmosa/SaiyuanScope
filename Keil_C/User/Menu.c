#include "Menu.h"
#include <stdio.h>  // 用于 sprintf (如果需要格式化显示)

// ==================================================================
//  1. 外部变量引用声明
//  (这些变量定义在 main.c 或其他文件中，此处通过 extern 引用)
// ==================================================================

// 系统控制变量
extern uint8_t menu_page;           // 当前菜单页码
extern uint8_t menu_choice;         // 当前选中的菜单项索引
extern uint16_t key_count;          // 按键计数
extern uint8_t KeyValue;            // 按键值
extern uint16_t Encoder_Count;      // 旋钮计数值
extern uint16_t Encoder_Count_last; // 上一次旋钮计数值
extern uint8_t Encoder_mode;        // 旋钮模式 (0:导航, 1:参数修改)

// 示波器参数变量
extern uint8_t is_show[10];         // 各项数据显示开关数组
extern uint8_t CH1_TIME_DIV_mode;   // CH1 时间档位索引
extern uint8_t CH1_VOLT_DIV_mode;   // CH1 电压档位索引
extern uint8_t CH2_TIME_DIV_mode;   // CH2 时间档位索引
extern uint8_t CH2_VOLT_DIV_mode;   // CH2 电压档位索引
extern uint8_t Trigger_mode;        // 触发模式索引

// 波形位移变量 (注意是带符号类型)
extern int16_t Horiziontal1_offset;
extern int16_t Vertical1_offset;
extern int16_t Horiziontal2_offset;
extern int16_t Vertical2_offset;

// 功能控制变量
extern uint8_t Wave_show_mode;
extern uint8_t show_wave_save_count;
extern uint8_t wave_save_count;
extern uint8_t uart3_enable;

// 字符串数组资源 (定义在 main.c 或 const_data.c 中)
extern char* CH1_TIME_DIV[];
extern char* CH1_VOLT_DIV[];
extern char* CH2_TIME_DIV[];
extern char* CH2_VOLT_DIV[];
extern char* Trigger[];

// 波形数据缓冲 (用于保存功能)
extern uint16_t wave1_value[];
extern uint16_t wave2_value[];
extern uint16_t wave1_save_value[5][200]; 
extern uint16_t wave2_save_value[5][200];

// ==================================================================
//  2. 私有辅助函数
// ==================================================================

/**
 * @brief 处理旋钮输入的通用逻辑
 * @return 0:无动作, 1:正转(加), -1:反转(减)
 */
static int Handle_Encoder(void) 
{
    int direction = 0;
    if (Encoder_Count > Encoder_Count_last) direction = 1;
    else if (Encoder_Count < Encoder_Count_last) direction = -1;

    if (direction != 0) {
        Encoder_Count = 10000;      // 复位计数值
        Encoder_Count_last = 10000;
    }
    return direction;
}

/**
 * @brief 清除菜单显示区域
 */
static void Clear_Menu_Area(void) 
{
    for(int i = 0; i <= 15; i++) {
        LCD_ShowString(207, 16 * i, "               ", MENU_BG_COLOR, BLACK, 16);
    }
}

/**
 * @brief 显示菜单文本项 (自动处理选中颜色)
 */
static void Draw_Menu_Item_Str(uint16_t x, uint16_t y, char* str, uint8_t current_choice, uint8_t my_index) 
{
    uint16_t color = (current_choice == my_index) ? MENU_TEXT_SEL : MENU_TEXT_UNSEL;
    LCD_ShowString(x, y, str, MENU_BG_COLOR, color, 16);
}

/**
 * @brief 显示带正负号的数字 (自动处理选中颜色)
 */
static void Draw_Menu_Item_Num(uint16_t x, uint16_t y, int num, uint8_t current_choice, uint8_t my_index) 
{
    uint16_t color = (current_choice == my_index) ? MENU_TEXT_SEL : MENU_TEXT_UNSEL;
    
    // 显示 +/- 符号
    if (num >= 0) LCD_ShowString(x - 8, y, "+", MENU_BG_COLOR, color, 16);
    else          LCD_ShowString(x - 8, y, "-", MENU_BG_COLOR, color, 16);
    
    // 显示绝对值数字
    LCD_ShowIntNum(x, y, (num >= 0 ? num : -num), 3, MENU_BG_COLOR, color, 16);
}

// ==================================================================
//  3. 主菜单逻辑函数
// ==================================================================
void menu(void)
{
    // ---------------------------------------------------------
    // 页面 0: 初始化/清屏 -> 跳转页面 1
    // ---------------------------------------------------------
    if (menu_page == 0)
    {
        Clear_Menu_Area();
        menu_page = 1;
        return; 
    }

    // ---------------------------------------------------------
    // 页面 1: 监视页面 (CH1/CH2 参数显示)
    // ---------------------------------------------------------
    if (menu_page == 1)
    {
        // --- 1. 输入处理 ---
        int enc_dir = Handle_Encoder();
        
        // 导航：上下移动光标
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
                is_show[menu_choice - 1] ^= 1; // 切换显示开关
            }
            else if (menu_choice == 11) {
                // AutoSet 代码占位
            }
            else if (menu_choice == 12) {
                menu_page = 2; // 跳转下一页
                menu_choice = 21;
                return;
            }
        }

        // --- 2. 显示处理 ---
        // 静态标签
        LCD_ShowString(270, 0,   "CH1",  MENU_BG_COLOR, BLACK, 16);
        LCD_ShowString(210, 16,  "MAX",  MENU_BG_COLOR, BLACK, 16);
        LCD_ShowString(210, 32,  "MIN",  MENU_BG_COLOR, BLACK, 16);
        LCD_ShowString(210, 48,  "AMP",  MENU_BG_COLOR, BLACK, 16);
        LCD_ShowString(210, 64,  "Freq", MENU_BG_COLOR, BLACK, 16);
        LCD_ShowString(210, 80,  "DUTY", MENU_BG_COLOR, BLACK, 16);
        
        LCD_ShowString(270, 100, "CH2",  MENU_BG_COLOR, BLACK, 16);
        LCD_ShowString(210, 116, "MAX",  MENU_BG_COLOR, BLACK, 16);
        LCD_ShowString(210, 132, "MIN",  MENU_BG_COLOR, BLACK, 16);
        LCD_ShowString(210, 148, "AMP",  MENU_BG_COLOR, BLACK, 16);
        LCD_ShowString(210, 164, "Freq", MENU_BG_COLOR, BLACK, 16);
        LCD_ShowString(210, 180, "DUTY", MENU_BG_COLOR, BLACK, 16);

        // 动态数据项 (这里暂时用固定字符串演示，实际可结合 sprintf)
        Draw_Menu_Item_Str(270, 16,  is_show[0] ? "2.15V"  : "     ", menu_choice, 1);
        Draw_Menu_Item_Str(270, 32,  is_show[1] ? "1.02V"  : "     ", menu_choice, 2);
        Draw_Menu_Item_Str(270, 48,  is_show[2] ? "1.13V"  : "     ", menu_choice, 3);
        Draw_Menu_Item_Str(270, 64,  is_show[3] ? "1000Hz" : "      ", menu_choice, 4);
        Draw_Menu_Item_Str(270, 80,  is_show[4] ? "50%"    : "   ",    menu_choice, 5);

        Draw_Menu_Item_Str(270, 116, is_show[5] ? "2.15V"  : "     ", menu_choice, 6);
        Draw_Menu_Item_Str(270, 132, is_show[6] ? "1.02V"  : "     ", menu_choice, 7);
        Draw_Menu_Item_Str(270, 148, is_show[7] ? "1.13V"  : "     ", menu_choice, 8);
        Draw_Menu_Item_Str(270, 164, is_show[8] ? "1000Hz" : "      ", menu_choice, 9);
        Draw_Menu_Item_Str(270, 180, is_show[9] ? "50%"    : "   ",    menu_choice, 10);

        Draw_Menu_Item_Str(250, 196, "AUTOSET", menu_choice, 11);
        Draw_Menu_Item_Str(270, 224, "->",      menu_choice, 12);
    }

    // ---------------------------------------------------------
    // 页面 2: 初始化/清屏 -> 跳转页面 3
    // ---------------------------------------------------------
    if (menu_page == 2)
    {
        Clear_Menu_Area();
        menu_page = 3;
        return;
    }

    // ---------------------------------------------------------
    // 页面 3: 控制页面 (设置参数、功能)
    // ---------------------------------------------------------
    if (menu_page == 3)
    {
        int enc_dir = Handle_Encoder();

        // --- 输入逻辑 ---
        if (Encoder_mode == 0) // 导航模式
        {
            if (enc_dir == 1) {
                menu_choice++;
                if (menu_choice > 38) menu_choice = 38;
            }
            else if (enc_dir == -1) {
                menu_choice--;
                if (menu_choice < 21) menu_choice = 21;
            }
        }
        else if (Encoder_mode == 1) // 编辑模式
        {
            if (enc_dir != 0) {
                switch(menu_choice) {
                    case 21: if(enc_dir==1 && CH1_TIME_DIV_mode<17) CH1_TIME_DIV_mode++; else if(enc_dir==-1 && CH1_TIME_DIV_mode>0) CH1_TIME_DIV_mode--; break;
                    case 22: if(enc_dir==1 && CH1_VOLT_DIV_mode<7)  CH1_VOLT_DIV_mode++; else if(enc_dir==-1 && CH1_VOLT_DIV_mode>0) CH1_VOLT_DIV_mode--; break;
                    case 23: if(enc_dir==1 && CH2_TIME_DIV_mode<17) CH2_TIME_DIV_mode++; else if(enc_dir==-1 && CH2_TIME_DIV_mode>0) CH2_TIME_DIV_mode--; break;
                    case 24: if(enc_dir==1 && CH2_VOLT_DIV_mode<7)  CH2_VOLT_DIV_mode++; else if(enc_dir==-1 && CH2_VOLT_DIV_mode>0) CH2_VOLT_DIV_mode--; break;
                    case 25: if(enc_dir==1 && Trigger_mode<2)       Trigger_mode++;      else if(enc_dir==-1 && Trigger_mode>0)      Trigger_mode--;      break;
                    
                    case 26: Horiziontal1_offset += enc_dir; 
                             if(Horiziontal1_offset > 0) Horiziontal1_offset = 0; 
                             if(Horiziontal1_offset < -19) Horiziontal1_offset = -19; break;
                    case 27: Vertical1_offset += enc_dir;    
                             if(Vertical1_offset > 10) Vertical1_offset = 10;   
                             if(Vertical1_offset < -10) Vertical1_offset = -10;    break;
                    case 28: Horiziontal2_offset += enc_dir; 
                             if(Horiziontal2_offset > 0) Horiziontal2_offset = 0; 
                             if(Horiziontal2_offset < -19) Horiziontal2_offset = -19; break;
                    case 29: Vertical2_offset += enc_dir;    
                             if(Vertical2_offset > 10) Vertical2_offset = 10;   
                             if(Vertical2_offset < -10) Vertical2_offset = -10;    break;
                    
                    case 34: // 查看保存波形
                        if(enc_dir==1) { show_wave_save_count++; if(show_wave_save_count>4) show_wave_save_count=0; }
                        else           { if(show_wave_save_count>0) show_wave_save_count--; else show_wave_save_count=4; }
                        Wave_show_mode=8; 
                        break;
                }
            }
        }

        // --- 确认键逻辑 ---
        if (key_count >= 1)
        {
            key_count = 0;
            
            // 需要进入编辑模式的选项
            if ((menu_choice >= 21 && menu_choice <= 29) || 
                menu_choice == 30 || menu_choice == 34 || menu_choice == 36 || menu_choice == 37) 
            {
                Encoder_mode = !Encoder_mode; // 切换 编辑/导航 模式
                
                // 特定功能逻辑
                if (menu_choice == 30) Wave_show_mode = (Encoder_mode == 1) ? 6 : 0; // STOP
                if (menu_choice == 34) Wave_show_mode = (Encoder_mode == 1) ? 8 : 0; // VIEW
                if (menu_choice == 36) Wave_show_mode = (Encoder_mode == 1) ? 2 : 0; // FFT1
                if (menu_choice == 37) Wave_show_mode = (Encoder_mode == 1) ? 4 : 0; // FFT2
            }
            // 单次动作
            else if (menu_choice == 31) { uart3_enable = 1; }
            else if (menu_choice == 32) { /* CLEAR */ }
            else if (menu_choice == 33) { // SAVE
                for(int i=0; i<=198; i++) {
                    wave1_save_value[wave_save_count][i] = wave1_value[200+i];
                    wave2_save_value[wave_save_count][i] = wave2_value[200+i];
                }
                wave_save_count++;
                if(wave_save_count >= 5) wave_save_count = 0;
            }
            else if (menu_choice == 38) { // 返回首页
                menu_choice = 1;
                menu_page = 0;
            }
        }

        // --- 2. 显示处理 ---
        LCD_ShowString(210, 0,  "CH1", MENU_BG_COLOR, BLACK, 16);
        Draw_Menu_Item_Str(250, 0,  CH1_TIME_DIV[CH1_TIME_DIV_mode], menu_choice, 21);
        Draw_Menu_Item_Str(250, 16, CH1_VOLT_DIV[CH1_VOLT_DIV_mode], menu_choice, 22);

        LCD_ShowString(210, 32, "CH2", MENU_BG_COLOR, BLACK, 16);
        Draw_Menu_Item_Str(250, 32, CH2_TIME_DIV[CH2_TIME_DIV_mode], menu_choice, 23);
        Draw_Menu_Item_Str(250, 48, CH2_VOLT_DIV[CH2_VOLT_DIV_mode], menu_choice, 24);

        LCD_ShowString(210, 64, "TRG", MENU_BG_COLOR, BLACK, 16);
        Draw_Menu_Item_Str(250, 64, Trigger[Trigger_mode], menu_choice, 25);

        LCD_ShowString(210, 80,  "HPOS1", MENU_BG_COLOR, BLACK, 16);
        Draw_Menu_Item_Num(270, 80,  Horiziontal1_offset, menu_choice, 26);

        LCD_ShowString(210, 96,  "VPOS1", MENU_BG_COLOR, BLACK, 16);
        Draw_Menu_Item_Num(270, 96,  Vertical1_offset,    menu_choice, 27);

        LCD_ShowString(210, 112, "HPOS2", MENU_BG_COLOR, BLACK, 16); 
        Draw_Menu_Item_Num(270, 112, Horiziontal2_offset, menu_choice, 28);

        LCD_ShowString(210, 128, "VPOS2", MENU_BG_COLOR, BLACK, 16);
        Draw_Menu_Item_Num(270, 128, Vertical2_offset,    menu_choice, 29);

        // 功能键区域
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