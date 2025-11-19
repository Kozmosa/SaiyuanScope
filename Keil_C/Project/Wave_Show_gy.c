#include "sc32_conf.h"
#include "system_sc32f1xxx.h"
#include "TFT_Lcd.h"
#include "lcdfont.h"
#include "Picture.h"

/**
 * @brief 在LCD上绘制网格
 * @param grid_size 每个格子的像素大小（例如10就是 10×10 一个格子）
 * @param color 网格线颜色（RGB565）
 */
// 画格子（网格）
// width, height : 屏幕宽度与高度
// grid_w, grid_h : 每格的宽度、高度
// color : 线的颜色
void LCD_DrawGrid(uint16_t width, uint16_t height,
                  uint16_t grid_w, uint16_t grid_h,
                  uint16_t color)
{
    uint16_t t;

    // 1. 画水平线
    for(t = 0; t <= height; t += grid_h)
    {
        TFT_DrawLine(0, t, width, t, color);
    }

    // 2. 画垂直线
    for(t = 0; t <= width; t += grid_w)
    {
        TFT_DrawLine(t, 0, t, height, color);
    }
}
