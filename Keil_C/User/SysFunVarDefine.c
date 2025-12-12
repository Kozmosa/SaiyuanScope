/**
 *****************************************************************************************************
  * @copyright	(c)  Shenzhen Saiyuan Microelectronics Co., Ltd
  * @file	         SysFunVarDefine.c
  * @author	 
  * @version 	
  * @date	
  * @brief	         Store var control and function control define
  * @details         
 *****************************************************************************************************
 * @attention
 *
 *****************************************************************************************************
 */
#include "SC_Init.h"
#include "SC_it.h"
#include "..\Drivers\SCDriver_list.h"
#include "HeadFiles\SysFunVarDefine.h"
#include "TFT_Lcd.h"
//**********************************************************************

uint8_t exKeyValue = 0;
uint8_t gTkIsValid = 0;
uint8_t KeyValue = 0;
uint8_t NOKEYCount = 0;
uint64_t TK_exKeyValueFlag = 0;
////用于保存滑轮档位值
unsigned int Wheel_value = 0;
////用于存放滑条档位值
unsigned int Slider_value = 0;
 void ChangeTouchKeyvalue(void)
{
    
     switch(TK_exKeyValueFlag)
     {
         case 0x00008000:exKeyValue = 1;
         break;
         case 0x00010000:exKeyValue = 2;
         break;
         case 0x00040000:exKeyValue = 3;
         break;
         case 0x00100000:exKeyValue = 4;
         break;
         case 0x00200000:exKeyValue = 5;
         break;
         case 0x00400000:exKeyValue = 6;
         break;
         case 0x00020000:exKeyValue = 7;
         break;
         case 0x00080000:exKeyValue = 8;
         break;
         default:exKeyValue = 0xff;
         break;
     }
    
    
}
 void UpdateDisplay(void)
{
    
    if(exKeyValue != 0xff)
    {
        if(gTkIsValid == 0)    //松手前只出一次键
        {
            gTkIsValid = 1;
            KeyValue=exKeyValue;
            if(KeyValue != 0xff)
            {
                switch(KeyValue)
								{
										case 1:
										break;
										case 2:
										break;
										case 3:
										break;
										case 4:
										break;
										case 5:
										break;
										case 6:
										break;
										default:
										break;
										
								}
								
            }
        }
    }
    else
    {
        gTkIsValid = 0;
    }
    
    
}
 void Sys_Scan(void)
{
    
    if(TK_TouchKeyStatus&0x40)        //当按键个数大于8个时，要判断半轮标志是否被置起，然后启动下半轮
    {
        TK_TouchKeyStatus &= 0xBf;    // 清除标志位， 需要外部清除。
        TK_Restart();                //启动下一轮转换
                
    }
    if(TK_TouchKeyStatus&0x80)        //重要步骤2:  触摸键扫描一轮标志，是否调用TouchKeyScan()一定要根据此标志位置起后
    {
        TK_TouchKeyStatus &= 0x7f;
        //重要步骤3: 清除标志位， 需要外部清除。
        TK_exKeyValueFlag = TK_TouchKeyScan();
        Slider_value = TKSlideModuleDealArray[0].OutValue;
        //printf("exKeyValue: %d\r\n",Slider_value);
        //按键数据处理函数
        ChangeTouchKeyvalue();
        UpdateDisplay();
       // printf("exKeyValue: %d\r\n",KeyValue);
		
        TK_Restart();
        //启动下一轮转换
    }
    
    
}


