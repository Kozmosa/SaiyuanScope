
#ifndef __WAVE_SHOW_GY_H
#define __WAVE_SHOW_GY_H

#include "sc32_conf.h"
#include "system_sc32f1xxx.h"
#include "TFT_Lcd.h"
#include "lcdfont.h"

/**
 * @brief  在LCD上绘制网格
 * @param  grid_size  每个格子的大小（px），如10表示 10×10 的网格
 * @param  color      网格线的颜色（RGB565）
 * @note   LCD_DrawPoint() 必须已在 TFT_Lcd.c 中实现
 */
void LCD_DrawGrid(uint16_t grid_size, uint16_t color);

#endif  // __LCD_GRID_H
