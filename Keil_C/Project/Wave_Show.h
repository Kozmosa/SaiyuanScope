#ifndef __WAVE_SHOW_GY_H
#define __WAVE_SHOW_GY_H

#include "sc32_conf.h"
#include "system_sc32f1xxx.h"
#include "TFT_Lcd.h"
#include "lcdfont.h"

/**
 * @brief  ?LCD?????
 * @param  grid_size  ???????(px),?10?? 10×10 ???
 * @param  color      ??????(RGB565)
 * @note   LCD_DrawPoint() ???? TFT_Lcd.c ???
 */
void LCD_DrawGrid(uint16_t grid_size, uint16_t color);

#endif  // __LCD_GRID_H
