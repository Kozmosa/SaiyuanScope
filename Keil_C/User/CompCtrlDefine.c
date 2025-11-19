/**
 *****************************************************************************************************
  * @copyright	(c)  Shenzhen Saiyuan Microelectronics Co., Ltd
  * @file	CompCtrlDefine.C
  * @author	 
  * @version 	
  * @date	
  * @brief	Store composite control define
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



void IcResourceInit(void)
{
    
    
        SC_Init(); /***MCU init***/
        TK_Init();
    
    
    
}


