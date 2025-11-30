#include "Data_Acquisition.h"

// 实例化全局变量
Data_Acq_Buffer_t Acq_Data;
volatile uint8_t  Acq_Done_Flag = 0;

// 内部变量
volatile uint8_t Current_Ch_Index = 0; 

void Data_Acquisition_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef  ADC_InitStructure;
    DMA_InitTypeDef  DMA_InitStructure;

    // ---------------- 1. 开启时钟 ----------------
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA, ENABLE);
    // ADC 和 GPIO 时钟通常默认开启或由内部电路自动管理
    // 如果需要显式开启，请查阅 sc32f1xxx_rcc.h 确认宏定义

    // ---------------- 2. GPIO 配置 ----------------
    // 手动初始化结构体
    GPIO_InitStructure.GPIO_DriveLevel = GPIO_DriveLevel_0; 
    GPIO_InitStructure.GPIO_Pin  = DAQ_CH1_PIN | DAQ_CH2_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_HI; // 高阻输入
    GPIO_Init(DAQ_CH1_GPIO_PORT, &GPIO_InitStructure);

    // ---------------- 3. DMA 配置 ----------------
    DMA_DeInit(DMA0);
    DMA_StructInit(&DMA_InitStructure);
    
    DMA_InitStructure.DMA_SrcAddress = (uint32_t)&(ADC->ADC_VALUE);
    DMA_InitStructure.DMA_DstAddress = (uint32_t)&Acq_Data.Buffer[0];
    DMA_InitStructure.DMA_BufferSize = DAQ_SAMPLE_DEPTH;
    DMA_InitStructure.DMA_SourceMode = DMA_SourceMode_FIXED;
    DMA_InitStructure.DMA_TargetMode = DMA_TargetMode_INC;
    DMA_InitStructure.DMA_DataSize   = DMA_DataSize_HalfWord;
    
    // 触发源：ADC
    // 如果 sc32f1xxx_dma.h 中没有 DMA_Request_ADC，请尝试用 0 代替
    DMA_InitStructure.DMA_Request = DMA_Request_ADC; 
    
    DMA_InitStructure.DMA_CircularMode = DMA_CircularMode_Disable;
    // 注意：优先级宏通常为全大写
    DMA_InitStructure.DMA_Priority = DMA_Priority_HIGH; 
    
    DMA_Init(DMA0, &DMA_InitStructure);

    // 开启 DMA 传输完成中断
    DMA_ITConfig(DMA0, DMA_IT_TCIE, ENABLE);
    
    // 使用 CMSIS 标准函数配置中断
    NVIC_EnableIRQ(DMA0_IRQn); 
    NVIC_SetPriority(DMA0_IRQn, 0);

    // ---------------- 4. ADC 配置 ----------------
    ADC_DeInit(ADC);
    ADC_StructInit(&ADC_InitStructure);
    
    ADC_InitStructure.ADC_ConvMode = ADC_ConvMode_Continuous; 
    // 修正：使用库文件中确认存在的宏 ADC_Prescaler_3CLOCK (分频系数为3)
    // 如果需要更慢的速度，可以尝试 ADC_Prescaler_8CLOCK
    ADC_InitStructure.ADC_Prescaler = ADC_Prescaler_3CLOCK; 
    ADC_InitStructure.ADC_VREF = ADC_VREF_VDD;
    ADC_InitStructure.ADC_EAIN = DAQ_CH1_ADC_CH;
    
    ADC_Init(ADC, &ADC_InitStructure);
    ADC_SetChannel(ADC, DAQ_CH1_ADC_CH);
    ADC_DMACmd(ADC, ENABLE);

    // ---------------- 5. 启动 ----------------
    DMA_Cmd(DMA0, ENABLE);
    ADC_Cmd(ADC, ENABLE);
    ADC_SoftwareStartConv(ADC);
}

void Data_Acquisition_Start_Next(void)
{
    if(Acq_Done_Flag == 1)
    {
        Acq_Done_Flag = 0;
        Current_Ch_Index = 0;

        ADC_SetChannel(ADC, DAQ_CH1_ADC_CH);

        DMA_Cmd(DMA0, DISABLE);
        DMA_SetDstAddress(DMA0, (uint32_t)&Acq_Data.Buffer[0]);
        DMA_SetCurrDataCounter(DMA0, DAQ_SAMPLE_DEPTH);
        DMA_Cmd(DMA0, ENABLE);

        // 如果 ADC 还在运行则不需要这句，但加上更保险
        if((ADC->ADC_CON & ADC_CON_ADCS) == 0) {
             ADC_SoftwareStartConv(ADC);
        }
    }
}

