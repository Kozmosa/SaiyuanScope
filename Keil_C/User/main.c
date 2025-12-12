/**
 *****************************************************************************************************
  * @file           main.c
  * @brief          SC32R803 8通道采集主程序
 *****************************************************************************************************
 */

#include "SC_Init.h"
#include "SC_it.h"
#include "..\Drivers\SCDriver_list.h"
#include "HeadFiles\SysFunVarDefine.h"
#include "TFT_Lcd.h"
#include "Picture.h"
#include "lcdfont.h"

// 引入功能模块
#include "Data_Acquisition.h" // 【关键】包含此头文件以访问 AD 宏和 Acq_Data
#include "Menu.h"
#include "Wave_Show_gy.h" 
#include "FFT_xtc.h"
#include <string.h> 

// =========================================================
//  外部变量引用
// =========================================================

// 【修改点1】: 
// 不需要再手动 extern AD_TypeDef AD; 
// 因为 Data_Acquisition.h 中已经有了 #define AD Acq_Data
// 只要包含头文件，就可以直接使用 AD.v[0] 或 Acq_Data.Buffer[0]

// 【修改点2】: 变量名修正
// Data_Acquisition.c 中定义的是 Acq_Done_Flag，而不是 ADC_Flag
extern volatile uint8_t Acq_Done_Flag;   

// 引用 Global_Variables.c 中的变量
extern uint16_t wave1_value[1000]; 
extern uint16_t wave2_value[1000];
extern uint16_t key_count;         
extern uint8_t  KeyValue;          

// =========================================================
//  全局变量定义
// =========================================================
uint8_t UART3_RxPacket[100];
uint8_t UART3_RxFlag = 0;
char    UART3_RxString[100];
extern uint8_t UART3_TxPacket[10];

// =========================================================
//  函数声明
// =========================================================
void screen_init(void);
void key_Get(void);
// 其他串口函数声明略...
#include <stdio.h> // 确保包含这个以使用 sprintf

#include "SC_Init.h"
#include "SC_it.h"
#include "..\Drivers\SCDriver_list.h"
#include "HeadFiles\SysFunVarDefine.h"
#include "TFT_Lcd.h"
#include <stdio.h> 
#include "Data_Acquisition.h" // 务必包含此头文件

// =========================================================
//  外部变量引用
// =========================================================
extern volatile uint8_t Acq_Done_Flag;      // 采集完成标志
extern volatile uint32_t Debug_ISR_Count;   // 中断计数器
// AD 在 Data_Acquisition.h 中定义为 Acq_Data 的别名

// =========================================================
//  调试函数：直接显示8个通道的数值
// =========================================================
void Debug_Show_8_Channels(void)
{
    char buf[50];
    uint16_t y_pos = 10; // 起始Y坐标
    
    // 1. 显示标题
    LCD_ShowString(10, y_pos, "ADC 8-CH RAW DEBUG", RED, BLACK, 16);
    y_pos += 24; // 标题高度

    // 2. 循环显示 8 个通道
    for(int i = 0; i < 8; i++)
    {
        uint16_t raw = AD.v[i]; // 获取原始值 (0~16383)
        // 换算电压: Raw * 3.3 / 16383
        float volt = (float)raw * 3.3f / 16383.0f; 
        
        // 格式化字符串: "CH0: 16383 = 3.300V"
        sprintf(buf, "CH%d: %5d = %.3fV", i, raw, volt);
        
        // 显示一行
        LCD_ShowString(10, y_pos, buf, WHITE, BLACK, 16);
        y_pos += 20; // 行间距
    }
}

// =========================================================
//  Main 函数
// =========================================================
int main(void)
{
    // 1. 硬件初始化
    IcResourceInit();    
    // 【关键】开启全局中断，否则 DMA 中断进不去
    __enable_irq(); 	
    
    screen_init();        
    
    // 2. 界面初始化 (清黑屏)
    LCD_Fill(0, 0, 240, 320, BLACK);
    
    // 3. 初始化采集 (DMA + ADC)
    Data_Acquisition_Init();

    while(1)
    {
        // ---------------------------------------------------------
        // 任务 1: 实时监控寄存器状态 (调试用，如果不动说明挂了)
        // ---------------------------------------------------------
        // 读取 DMA0 状态 (0x07 表示传输完成且中断置位)
        uint32_t dma_status = DMA0->DMA_STS; 
        
        // 读取 ADC 状态 (0x01 表示转换完成)
        uint32_t adc_status = ADC->ADC_STS;

        char buf[50];
        // 在屏幕底部显示: DMA状态, ADC状态, 中断进入次数
        // 如果 Cnt 不涨，说明中断没进去；如果 D:00，说明 DMA 没动
        sprintf(buf, "D:%02X A:%02X Cnt:%d", dma_status, adc_status, Debug_ISR_Count);
        LCD_ShowString(10, 220, buf, YELLOW, BLACK, 16);
				Debug_Show_8_Channels();
        // ---------------------------------------------------------
        // 任务 2: 处理采集数据
        // ---------------------------------------------------------
        if(Acq_Done_Flag == 1)
        {
            // 1. 清除标志
            Acq_Done_Flag = 0;

            // 2. 显示 8 个通道的数据
            Debug_Show_8_Channels();

            // 3. 启动下一轮采集 (接力)
            Data_Acquisition_Start_Next();
        }

        // ---------------------------------------------------------
        // 任务 3: 系统扫描
        // ---------------------------------------------------------
        Sys_Scan();
    }
}

void screen_init()//屏幕初始化
{
	Lcd_Init();
	LCD_Fill ( 1, 1, 320, 240, BLACK );//本来是花屏，刷新后为白屏
}




void key_Get()//按压旋转编码器
{
	if(GPIO_ReadDataBit(GPIOA,GPIO_Pin_6)==0)
			{
				 delay_ms ( 20 );
				while(GPIO_ReadDataBit(GPIOA,GPIO_Pin_6)==0);
				delay_ms(20);
				key_count++;
			}
			
}

void UART3_Send_Packet()//wifi串口发送数据包
{
	static uint8_t RxState = 0;		//定义表示当前状态机状态的静态变量
	static uint8_t pRxPacket = 0;	//定义表示当前接收数据位置的静态变量
	 UART3_TxPacket[0]=0x01;
	UART3_TxPacket[1]=0xA2;
	UART3_TxPacket[2]=0x12;
	UART3_TxPacket[3]=0x23;
	UART3_TxPacket[4]=0x34;
	UART3_TxPacket[5]=0x45;
	UART3_TxPacket[6]=0x56;
	UART3_TxPacket[7]=0x65;
	UART3_TxPacket[8]=0x54;
	UART3_TxPacket[9]=0x43;
	UART_SendArray(UART3,UART3_TxPacket, 10);					
}

void UART3_Send_String()//wifi串口发送文本
{
	UART_SendString(UART3,"uni_wro");
}

void UART3_receive_Packet()//wifi串口接受数据(数据包格式FF xx xx xx xx FE)
{
	static uint8_t RxState = 0;		//定义表示当前状态机状态的静态变量
	static uint8_t pRxPacket = 0;	//定义表示当前接收数据位置的静态变量
	if (UART_GetFlagStatus(UART3, UART_Flag_RX) == SET)	//判断是否是USART3的接收事件触发的中断
	{
		
		uint8_t RxData = UART_ReceiveData(UART3);			//读取数据寄存器，存放在接收的数据变量

		/*使用状态机的思路，依次处理数据包的不同部分*/

		/*当前状态为0，接收数据包包头*/
				if (RxState == 0)
		{
			if (RxData == 0xFF)			//如果数据确实是包头
			{
				RxState = 1;			//置下一个状态
				pRxPacket = 0;			//数据包的位置归零
			}
		}
		/*当前状态为1，接收数据包数据*/
		else if (RxState == 1)
		{
			UART3_RxPacket[pRxPacket] = RxData;	//将数据存入数据包数组的指定位置
			pRxPacket ++;				//数据包的位置自增
			if (pRxPacket >=4)			//如果收够4个数据
			{
				RxState = 2;			//置下一个状态
			}
		}
		/*当前状态为2，接收数据包包尾*/
		else if (RxState == 2)
		{
			if (RxData == 0xFE)			//如果数据确实是包尾部
			{
				RxState = 0;			//状态归0
				UART3_RxFlag = 1;		//接收数据包标志位置1，成功接收一个数据包
			}
		}


	}
}
void UART3_receive_String()
{
	static uint8_t RxState = 0;		//定义表示当前状态机状态的静态变量
	static uint8_t pRxPacket = 0;	//定义表示当前接收数据位置的静态变量
	if (UART_GetFlagStatus(UART3, UART_Flag_RX) == SET)	//判断是否是USART1的接收事件触发的中断
	{
		uint8_t RxData = UART_ReceiveData(UART3);			//读取数据寄存器，存放在接收的数据变量

		/*使用状态机的思路，依次处理数据包的不同部分*/

		/*当前状态为0，接收数据包包头*/
		if (RxState == 0)
		{
			 RxState=1;
            for(uint8_t i=0;i<100;i++)
            {
                UART3_RxString[i]=' ';
            }
            pRxPacket=0;
            UART3_RxString[pRxPacket]=RxData;
            pRxPacket++;
		}
		/*当前状态为1，接收数据包数据，同时判断是否接收到了第一个包尾*/
		else if (RxState == 1)
		{
			if (RxData == '\r')			//如果收到第一个包尾
			{
				RxState = 2;			//置下一个状态
			}
			else						//接收到了正常的数据
			{
				UART3_RxString[pRxPacket] = RxData;		//将数据存入数据包数组的指定位置
				pRxPacket ++;			//数据包的位置自增
			}
		}
		/*当前状态为2，接收数据包第二个包尾*/
		else if (RxState == 2)
		{
			if (RxData == '\n')			//如果收到第二个包尾
			{
				RxState = 0;			//状态归0
				UART3_RxString[pRxPacket] = '\0';			//将收到的字符数据包添加一个字符串结束标志
				UART3_RxFlag = 1;		//接收数据包标志位置1，成功接收一个数据包
			}
		}


	}
}
