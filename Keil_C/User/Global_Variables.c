#include "sc32.h" // ???????????????,????? uint8_t/u8 ?

// ==========================================
//  ????????
// ==========================================
uint8_t menu_page = 0;           // ??????
uint8_t menu_choice = 1;         // ??????????
uint16_t Encoder_Count_last = 0; // ????????
uint8_t Encoder_mode = 0;        // ????
uint16_t key_count; 
// ==========================================
//  ?????????
// ==========================================
// ????????
uint8_t is_show[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}; 

uint8_t CH1_TIME_DIV_mode = 0;   // CH1 ????
uint8_t CH1_VOLT_DIV_mode = 0;   // CH1 ????
uint8_t CH2_TIME_DIV_mode = 0;   // CH2 ????
uint8_t CH2_VOLT_DIV_mode = 0;   // CH2 ????
uint8_t Trigger_mode = 0;        // ????

// ??????
int16_t Horiziontal1_offset = 0;
int16_t Vertical1_offset = 0;
int16_t Horiziontal2_offset = 0;
int16_t Vertical2_offset = 0;

// ==========================================
//  ????????
// ==========================================
uint8_t Wave_show_mode = 0;
uint8_t show_wave_save_count = 0;
uint8_t wave_save_count = 0;
uint8_t uart3_enable = 0;

// ==========================================
//  ??????? (???????)
// ==========================================
// ??????????,?????????????
char* CH1_TIME_DIV[] = {
    "10us", "20us", "50us", "100us", "200us", "500us",
    "1ms",  "2ms",  "5ms",  "10ms",  "20ms",  "50ms",
    "100ms","200ms","500ms","1s",    "2s",    "5s"
};

char* CH2_TIME_DIV[] = {
    "10us", "20us", "50us", "100us", "200us", "500us",
    "1ms",  "2ms",  "5ms",  "10ms",  "20ms",  "50ms",
    "100ms","200ms","500ms","1s",    "2s",    "5s"
};

char* CH1_VOLT_DIV[] = {
    "10mV", "20mV", "50mV", "100mV", "200mV", "500mV", "1V", "2V"
};

char* CH2_VOLT_DIV[] = {
    "10mV", "20mV", "50mV", "100mV", "200mV", "500mV", "1V", "2V"
};

char* Trigger[] = {
    "Auto", "Normal", "Single"
};

// ==========================================
//  ????????
// ==========================================
// ?????????,????????? 
// ??????? 500 ? 1000,?????????
uint16_t wave1_value[1000] = {0}; 
uint16_t wave2_value[1000] = {0};

// ???????? (5?,??200?)
uint16_t wave1_save_value[5][200] = {0}; 
uint16_t wave2_save_value[5][200] = {0};