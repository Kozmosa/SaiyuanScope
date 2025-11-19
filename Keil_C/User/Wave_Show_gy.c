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
// ???(??)
// width, height : ???????
// grid_w, grid_h : ????????
// color : ????
void LCD_DrawGrid(uint16_t width, uint16_t height,
                  uint16_t grid_w, uint16_t grid_h,
                  uint16_t color)
{
    uint16_t t;

    // 1. ????
    for(t = 0; t <= height; t += grid_h)
    {
        TFT_DrawLine(0, t, width, t, color);
    }

    // 2. ????
    for(t = 0; t <= width; t += grid_w)
    {
        TFT_DrawLine(t, 0, t, height, color);
    }
}
