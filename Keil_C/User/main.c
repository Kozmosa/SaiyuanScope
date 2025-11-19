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
uint16_t AD1;//ADC1通道采样结果
uint16_t AD2;
uint16_t AD3;
uint16_t AD4;
uint16_t AD5;
uint16_t AD6;
uint16_t AD7;
uint16_t AD8;
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

    while(1)
    {
        ADC_Conversion();  // ADC ??

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


void ADC_Conversion()//ADC采集
{
	for(int ADC_num=1;ADC_num<=8;ADC_num++)//8个ADC通道采集
	{
		if(ADC_num==1) ADC_SetChannel(ADC,ADC_Channel_0);
		if(ADC_num==2) ADC_SetChannel(ADC,ADC_Channel_1);
		if(ADC_num==3) ADC_SetChannel(ADC,ADC_Channel_10);
		if(ADC_num==4) ADC_SetChannel(ADC,ADC_Channel_11);
		if(ADC_num==5) ADC_SetChannel(ADC,ADC_Channel_12);
		if(ADC_num==6) ADC_SetChannel(ADC,ADC_Channel_13);
		if(ADC_num==7) ADC_SetChannel(ADC,ADC_Channel_14);
		if(ADC_num==8) ADC_SetChannel(ADC,ADC_Channel_15);
		static volatile uint16_t ADC_Value;//定义变量存储ADC值
		//ADC单次转化
		ADC_SoftwareStartConv(ADC);//软件触发ADC
		while(ADC_Flag == RESET);//等待ADC转换结束
		ADC_Flag = RESET;
		LCD_ShowIntNum (270 , 20*ADC_num,ADC_GetConversionValue(ADC), 6, RED, WHITE,16 );//ADC采样结果
		if(ADC_num==1) AD1=ADC_GetConversionValue(ADC);
		if(ADC_num==2) AD2=ADC_GetConversionValue(ADC);
		if(ADC_num==3) AD3=ADC_GetConversionValue(ADC);
		if(ADC_num==4) AD4=ADC_GetConversionValue(ADC);
		if(ADC_num==5) AD5=ADC_GetConversionValue(ADC);
		if(ADC_num==6) AD6=ADC_GetConversionValue(ADC);
		if(ADC_num==7) AD7=ADC_GetConversionValue(ADC);
		if(ADC_num==8) AD8=ADC_GetConversionValue(ADC);
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