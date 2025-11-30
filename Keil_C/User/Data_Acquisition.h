#ifndef __DATA_ACQUISITION_H
#define __DATA_ACQUISITION_H

#include "sc32.h"
#include "sc32f1xxx_adc.h"
#include "sc32f1xxx_dma.h"
#include "sc32f1xxx_gpio.h"
#include "sc32f1xxx_rcc.h"

// 配置区域
#define DAQ_SAMPLE_DEPTH    500
#define DAQ_TOTAL_BUF_SIZE  (DAQ_SAMPLE_DEPTH * 2)

#define DAQ_CH1_GPIO_PORT   GPIOA
#define DAQ_CH1_PIN         GPIO_Pin_0 
#define DAQ_CH1_ADC_CH      ADC_Channel_0

#define DAQ_CH2_GPIO_PORT   GPIOA
#define DAQ_CH2_PIN         GPIO_Pin_1 
#define DAQ_CH2_ADC_CH      ADC_Channel_1

typedef struct {
    uint16_t Buffer[DAQ_TOTAL_BUF_SIZE]; 
} Data_Acq_Buffer_t;

extern Data_Acq_Buffer_t Acq_Data;
extern volatile uint8_t  Acq_Done_Flag;
// ... 其他 extern ...

extern volatile uint8_t Current_Ch_Index; 


// ... 函数声明 ...
void Data_Acquisition_Init(void);
void Data_Acquisition_Start_Next(void);

#endif /* __DATA_ACQUISITION_H */
