#ifndef __GLOBAL_VARIABLES_H
#define __GLOBAL_VARIABLES_H

#include "sc32.h" // ??????????

// ==========================================
//  ?????? (?? Global_Variables.c)
// ==========================================

extern uint8_t menu_page;
extern uint8_t menu_choice;
extern uint16_t Encoder_Count_last;
extern uint8_t Encoder_mode;

extern uint8_t is_show[10];

extern uint8_t CH1_TIME_DIV_mode;
extern uint8_t CH1_VOLT_DIV_mode;
extern uint8_t CH2_TIME_DIV_mode;
extern uint8_t CH2_VOLT_DIV_mode;
extern uint8_t Trigger_mode;

extern int16_t Horiziontal1_offset;
extern int16_t Vertical1_offset;
extern int16_t Horiziontal2_offset;
extern int16_t Vertical2_offset;

extern uint8_t Wave_show_mode;
extern uint8_t show_wave_save_count;
extern uint8_t wave_save_count;
extern uint8_t uart3_enable;

// ??????
extern char* CH1_TIME_DIV[];
extern char* CH2_TIME_DIV[];
extern char* CH1_VOLT_DIV[];
extern char* CH2_VOLT_DIV[];
extern char* Trigger[];

// ????????
extern uint16_t wave1_value[1000];
extern uint16_t wave2_value[1000];
extern uint16_t wave1_save_value[5][200];
extern uint16_t wave2_save_value[5][200];

#endif /* __GLOBAL_VARIABLES_H */