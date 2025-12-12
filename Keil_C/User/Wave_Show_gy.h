#ifndef __WAVE_SHOW_GY_H
#define __WAVE_SHOW_GY_H

#include "sc32.h"           
#include "TFT_Lcd.h"        
#include "Data_Acquisition.h" 

// ================= ?????? =================
// ?? TFT_Lcd.h ????????,??????
#ifndef CYAN
#define CYAN      0x07FF
#endif
#ifndef YELLOW
#define YELLOW    0xFFE0
#endif
#ifndef BLACK
#define BLACK     0x0000
#endif
#ifndef BLUE
#define BLUE      0x001F
#endif

// ================= ?????? =================
#define WAVE_X_START      0     
#define WAVE_DISP_WIDTH   240   
#define WAVE_DISP_HEIGHT  200   
#define WAVE_CENTER_Y     100   

// ================= ???? =================
#define WAVE_COLOR_CH1    YELLOW  
#define WAVE_COLOR_CH2    CYAN    
#define WAVE_COLOR_BG     BLACK   

// ================= ???? =================
void Wave_Show_Init(void);      
void Wave_Show_Update(void);    

void LCD_DrawGrid(uint16_t width, uint16_t height, uint16_t grid_w, uint16_t grid_h, uint16_t color);

#endif /* __WAVE_SHOW_GY_H */