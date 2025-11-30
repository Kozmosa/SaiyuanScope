
#ifndef __FFT_XTC_H
#define __FFT_XTC_H

#include "sc32_conf.h"
#include "system_sc32f1xxx.h"



void LCD_DrawFFT(float *b,uint8_t length,uint16_t color);
void FFT(int16_t *a, float *f);
#endif  // __LCD_GRID_H
