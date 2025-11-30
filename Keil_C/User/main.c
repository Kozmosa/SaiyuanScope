/**
 *****************************************************************************************************
  * @copyright	(c)  Shenzhen Saiyuan Microelectronics Co., Ltd
  * @file	         main.c
  * @author	 
  * @version 	
  * @date	
  * @brief	         
  * @details         Contains the MCU initialization function and its H file
 *****************************************************************************************************
 * @attention
 *
 *****************************************************************************************************
 */
/*******************Includes************************************************************************/
#include "sc32f1xxx_adc.h"
#include "SC_Init.h"
#include "SC_it.h"
#include "Wave_Show_gy.h"
#include "..\Drivers\SCDriver_list.h"
#include "HeadFiles\SysFunVarDefine.h"
#include "TFT_Lcd.h"
#include "Picture.h"
#include "lcdfont.h"
#include "Data_Acquisition.H"
#include "FFT_xtc.h"
#include "Menu.h"


static volatile FlagStatus ADC_Flag;//自定义标志位

void screen_init();//屏幕启动
void ADC_Conversion_IRQ();//处理ADC中断
void ADC_Conversion();//ADC采样
void key_Get();//按压旋转编码器
void UART3_Send_Packet();//wifi串口发送数据包
void UART3_Send_String();//wifi串口发送文本
void UART3_receive_Packet();//wifi串口接受数据
void UART3_receive_String();//wifi串口接受数据


extern uint8_t KeyValue ;	//触控信息
extern uint16_t TIM0_count;//定时器 1ms中断一次 每1ms,count+=1
extern uint16_t Encoder_Count;//转动旋转编码器计数,初始值10000，向左旋转1格-1，向右旋转一格+1
uint16_t key_count;//按压旋转编码器计数,每按压一次加1
extern uint8_t UART3_RxFlag;	
uint8_t UART3_RxPacket[100];				//定义接收数据包数组，数据包格式"@MSG\r\n"
uint8_t UART3_RxFlag;					//定义接收数据包标志位
char UART3_RxString[100];				//定义接收数据包数组，数据包格式"@MSG\r\n"

typedef struct {
	uint16_t v[8];   // v[0]..v[7] 对应原来的 AD1..AD8
} ADC_Values_t;

ADC_Values_t AD; // 全局结构体实例，保存8路ADC采样结果

/* 为兼容原有代码中的 AD1..AD8 名称，保留宏映射 */
#define AD1 (AD.v[0])
#define AD2 (AD.v[1])
#define AD3 (AD.v[2])
#define AD4 (AD.v[3])
#define AD5 (AD.v[4])
#define AD6 (AD.v[5])
#define AD7 (AD.v[6])
#define AD8 (AD.v[7])
/**
  * @brief This function implements main function.
  * @note 
  * @param
  */
int main(void)
{
    IcResourceInit();     // ?????
    screen_init();        // ?????
    
    // ??? ???(????)
    LCD_Fill(0, 0, 240, 320, BLACK);   // ????
    LCD_DrawGrid(200, 255, 20, 15, BLUE);  // ???
    
    // (????????,????)
    // TFT_DrawLine(0, 160, 240, 160, RED);
    // TFT_DrawLine(120, 0, 120, 320, RED);
		Data_Acquisition_Init();
    while(1)
    {
        
		menu();
        // 【新增 2】 检查采集是否完成
        if(Acq_Done_Flag == 1)
        {
            /* 
               数据已存放在 Acq_Data.Buffer[] 中
               0~499 是通道1，500~999 是通道2
               在这里处理数据（如发送串口、计算等），不需要画图
            */
					
            // 【重要】 处理完数据后，必须调用此函数启动下一轮采集
            Data_Acquisition_Start_Next();
        }
        Sys_Scan();        // ????
        key_Get();

        // ??????(????)
       
    }
}









void screen_init()//屏幕初始化
{
	Lcd_Init();
	LCD_Fill ( 1, 1, 320, 240, BLACK );//本来是花屏，刷新后为白屏
}


void ADC_Conversion_IRQ()//ADC中断处理
{
  if(ADC_GetFlagStatus(ADC, ADC_Flag_ADCIF))//判断ADC标志位
  {
    ADC_ClearFlag(ADC, ADC_Flag_ADCIF);//清除ADC标志位
    ADC_Flag = SET;//自定义标志位置起
  }
}


void ADC_Conversion(void) // ADC采集
{
    // ADC通道映射表
    uint8_t ADC_Channel_Table[8] = {
        ADC_Channel_0, ADC_Channel_1, ADC_Channel_10, ADC_Channel_11,
        ADC_Channel_12, ADC_Channel_13, ADC_Channel_14, ADC_Channel_15
    };

    for (int i = 0; i < 8; i++) {
        ADC_SetChannel(ADC, ADC_Channel_Table[i]);   // 选择通道
        ADC_SoftwareStartConv(ADC);                  // 软件触发ADC
        while (ADC_Flag == RESET);                   // 等待ADC转换完成
        ADC_Flag = RESET;

        AD.v[i] = ADC_GetConversionValue(ADC);       // 存入结构体数组
        LCD_ShowIntNum(270, 20*(i+1), AD.v[i], 6, RED, WHITE, 16); // 显示
    }
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
