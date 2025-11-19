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

#include "SC_Init.h"
#include "SC_it.h"
#include "..\Drivers\SCDriver_list.h"
#include "HeadFiles\SysFunVarDefine.h"




/**
  * @brief This function implements main function.
  * @note 
  * @param
  */
int main(void)
{
    
    
    IcResourceInit();
    
    
    /*****MainLoop*****/
    while(1)
    {
        
        /***User program***/
        Sys_Scan();     //°´¼üÉ¨Ãè
        
        
        
    }
    
    
}
