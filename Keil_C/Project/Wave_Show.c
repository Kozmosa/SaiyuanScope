#include "sc32_conf.h"
#include "system_sc32f1xxx.h"
#include "TFT_Lcd.h"
#include "lcdfont.h"
#include "Picture.h"

/**
 * @brief ?LCD?????
 * @param grid_size ?????????(??10?? 10×10 ????)
 * @param color ?????(RGB565)
 */
void LCD_DrawGrid(uint16_t grid_size, uint16_t color)
{   uint32_t LCD_H = 200;
    uint32_t LCD_W = 255;
    // ???(????,?? grid_size ???)
    for (uint16_t y = 0; y < LCD_H; y += grid_size)
    {
        for (uint16_t x = 0; x < LCD_W; x++)
        {
            LCD_DrawPoint(x, y, color);
        }
    }

    // ???(????,?? grid_size ???)
    for (uint16_t x = 0; x < LCD_W; x += grid_size)
    {
        for (uint16_t y = 0; y < LCD_H; y++)
        {
            LCD_DrawPoint(x, y, color);
        }
    }
}
