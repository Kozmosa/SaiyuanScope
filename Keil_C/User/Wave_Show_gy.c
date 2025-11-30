#include "Wave_Show_gy.h"

// ??????
extern int16_t Vertical1_offset; 
extern int16_t Vertical2_offset; 

// ??????
static uint16_t Old_Y_Ch1[WAVE_DISP_WIDTH]; 
static uint16_t Old_Y_Ch2[WAVE_DISP_WIDTH];

// ================= ???? =================

/**
 * @brief  ??????
 */
static uint16_t Software_Trigger(uint16_t *buffer, uint16_t limit)
{
    uint16_t i;
    uint16_t trigger_level = 2048; 
    
    for(i = 1; i < limit - 10; i++)
    {
        if(buffer[i-1] < trigger_level && buffer[i] >= trigger_level)
        {
            return i; 
        }
    }
    return 0; 
}

/**
 * @brief  ???? Y ??
 */
static uint16_t Calc_Y_Pos(uint16_t adc_val, int16_t offset)
{
    int32_t y = adc_val / 25; 
    
    y = WAVE_DISP_HEIGHT - y; 
    y -= (offset * 10); 

    if(y < 0) y = 0;
    if(y >= WAVE_DISP_HEIGHT) y = WAVE_DISP_HEIGHT - 1;
    
    return (uint16_t)y;
}

// ================= ???? =================

void Wave_Show_Init(void)
{
    // ???????
    for(int i = 0; i < WAVE_DISP_WIDTH; i++)
    {
        Old_Y_Ch1[i] = WAVE_CENTER_Y;
        Old_Y_Ch2[i] = WAVE_CENTER_Y;
    }
    
    LCD_Fill(0, 0, 240, 320, WAVE_COLOR_BG);
    LCD_DrawGrid(240, 200, 20, 20, BLUE);
}

void Wave_Show_Update(void)
{
    uint16_t x; // ????????? i
    uint16_t start_idx_1, start_idx_2;
    uint16_t new_y1, new_y1_next;
    uint16_t new_y2, new_y2_next;

    // 1. ?????
    start_idx_1 = Software_Trigger(&Acq_Data.Buffer[0], 200);
    start_idx_2 = Software_Trigger(&Acq_Data.Buffer[DAQ_SAMPLE_DEPTH], 200);

    // 2. ????
    for(x = 0; x < WAVE_DISP_WIDTH - 1; x++)
    {
        // --- A. ???? ---
        uint16_t val1_now  = Acq_Data.Buffer[start_idx_1 + x];
        uint16_t val1_next = Acq_Data.Buffer[start_idx_1 + x + 1];
        
        uint16_t val2_now  = Acq_Data.Buffer[DAQ_SAMPLE_DEPTH + start_idx_2 + x];
        uint16_t val2_next = Acq_Data.Buffer[DAQ_SAMPLE_DEPTH + start_idx_2 + x + 1];

        // --- B. ????? ---
        new_y1      = Calc_Y_Pos(val1_now,  Vertical1_offset);
        new_y1_next = Calc_Y_Pos(val1_next, Vertical1_offset);
        
        new_y2      = Calc_Y_Pos(val2_now,  Vertical2_offset);
        new_y2_next = Calc_Y_Pos(val2_next, Vertical2_offset);

        // --- C. ???? (?????? TFT_DrawLine) ---
        TFT_DrawLine(WAVE_X_START + x, Old_Y_Ch1[x], 
                     WAVE_X_START + x + 1, Old_Y_Ch1[x+1], WAVE_COLOR_BG);
                     
        TFT_DrawLine(WAVE_X_START + x, Old_Y_Ch2[x], 
                     WAVE_X_START + x + 1, Old_Y_Ch2[x+1], WAVE_COLOR_BG);

        // --- D. ???? ---
        TFT_DrawLine(WAVE_X_START + x, new_y1, 
                     WAVE_X_START + x + 1, new_y1_next, WAVE_COLOR_CH1);
                     
        TFT_DrawLine(WAVE_X_START + x, new_y2, 
                     WAVE_X_START + x + 1, new_y2_next, WAVE_COLOR_CH2);

        // --- E. ?????? ---
        Old_Y_Ch1[x] = new_y1;
        Old_Y_Ch2[x] = new_y2;
        
        // ????
        if(x == WAVE_DISP_WIDTH - 2) {
            Old_Y_Ch1[x+1] = new_y1_next;
            Old_Y_Ch2[x+1] = new_y2_next;
        }
    }
}

void LCD_DrawGrid(uint16_t width, uint16_t height,
                  uint16_t grid_w, uint16_t grid_h,
                  uint16_t color)
{
    uint16_t t;
    for(t = 0; t <= height; t += grid_h)
    {
        TFT_DrawLine(0, t, width, t, color);
    }
    for(t = 0; t <= width; t += grid_w)
    {
        TFT_DrawLine(t, 0, t, height, color);
    }
}