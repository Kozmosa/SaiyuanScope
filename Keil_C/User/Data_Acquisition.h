#ifndef __DATA_ACQUISITION_H
#define __DATA_ACQUISITION_H

#include "sc32f1xxx.h"

// ================= 定义 =================
// 采集深度定义为 8 (满足底层需求)
#define DAQ_SAMPLE_DEPTH 8

// 引脚定义 (根据你的实际电路修改)
#define DAQ_CH1_GPIO_PORT   GPIOA
#define DAQ_CH1_PIN         GPIO_Pin_0
#define DAQ_CH2_PIN         GPIO_Pin_1
// 注意：AIN10-15 的引脚也需要确保在 Init 中初始化

// ================= 数据结构统一 =================
// 使用 Union 使得一套数据可以被不同的名称访问，兼容旧代码
typedef union {
    uint16_t Buffer[DAQ_SAMPLE_DEPTH]; // Wave_Show_gy.c 使用这个名字
    uint16_t v[DAQ_SAMPLE_DEPTH];      // main.c 可能使用这个名字
} Data_Acq_Buffer_t;

// 为了兼容 main.c 中的 "extern AD_TypeDef AD;"
typedef Data_Acq_Buffer_t AD_TypeDef;

// ================= 全局变量声明 =================
extern Data_Acq_Buffer_t Acq_Data;

// 宏定义：将 main.c 里的 AD 映射到 Acq_Data，解决变量名冲突
#define AD Acq_Data 

extern volatile uint8_t Acq_Done_Flag;

// ================= 函数声明 =================
void Data_Acquisition_Init(void);
void Data_Acquisition_Start(void);      
void Data_Acquisition_Start_Next(void); 

#endif /* __DATA_ACQUISITION_H */