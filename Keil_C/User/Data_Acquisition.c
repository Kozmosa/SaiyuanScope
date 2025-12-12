#include "Data_Acquisition.h"
#include "sc32f1xxx_adc.h"
#include "sc32f1xxx_dma.h"
#include "sc32f1xxx_gpio.h"
#include "sc32f1xxx_rcc.h"
#include "SC_it.h"

// ================= 变量定义 =================
Data_Acq_Buffer_t Acq_Data;  
volatile uint8_t  Acq_Done_Flag = 0;
volatile uint32_t Debug_ISR_Count = 0; 

static volatile uint8_t Current_Ch_Index = 0;

static const ADC_ChannelTypedef Channel_List[DAQ_SAMPLE_DEPTH] = {
    ADC_Channel_0,  ADC_Channel_1,   
    ADC_Channel_10, ADC_Channel_11,
    ADC_Channel_12, ADC_Channel_13, 
    ADC_Channel_14, ADC_Channel_15
};

// 【关键修正】：根据你提供的手册，ADC 是 59 (0x3B)
// 必须左移 4 位才能写入 DMA_CFG 的高位
#define ADC_DMA_REQ_SRC  (59 << 4) 

void Data_Acquisition_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef  ADC_InitStructure;
    DMA_InitTypeDef  DMA_InitStructure;

    // 1. 开启时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA, ENABLE);


    // 2. GPIO 配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_HI; 
    GPIO_InitStructure.GPIO_DriveLevel = GPIO_DriveLevel_0;
    GPIO_InitStructure.GPIO_Pin = DAQ_CH1_PIN | DAQ_CH2_PIN; 
    GPIO_Init(DAQ_CH1_GPIO_PORT, &GPIO_InitStructure);

    // 3. DMA 配置 (使用 DMA0)
    DMA_DeInit(DMA0);
    DMA_StructInit(&DMA_InitStructure);
    
    // 源地址 & 目标地址
    DMA_InitStructure.DMA_SrcAddress = (uint32_t)&(ADC->ADC_VALUE); 
    DMA_InitStructure.DMA_DstAddress = (uint32_t)&Acq_Data.Buffer[0];
    
    // 基础配置
    DMA_InitStructure.DMA_BufferSize = 1;                    
    DMA_InitStructure.DMA_SourceMode = DMA_SourceMode_FIXED; 
    DMA_InitStructure.DMA_TargetMode = DMA_TargetMode_FIXED; 
    DMA_InitStructure.DMA_DataSize   = DMA_DataSize_HalfWord;
    
    // 【重点】使用移位后的 59
    DMA_InitStructure.DMA_Request    = ADC_DMA_REQ_SRC;      
    
    DMA_InitStructure.DMA_CircularMode = DMA_CircularMode_Disable;
    DMA_InitStructure.DMA_Priority   = DMA_Priority_VERY_HIGH;
    
    DMA_Init(DMA0, &DMA_InitStructure);

    // 4. DMA 中断配置
    // SC32 手册：DMA0 对应中断号 11 (请确保 startup 文件里是 DMA0_IRQHandler)
    DMA_ITConfig(DMA0, DMA_IT_TCIE, ENABLE);
    NVIC_SetPriority(DMA0_IRQn, 0); 
    NVIC_EnableIRQ(DMA0_IRQn);

    // 5. ADC 配置
    ADC_DeInit(ADC);
    ADC_StructInit(&ADC_InitStructure);
    ADC_InitStructure.ADC_ConvMode = ADC_ConvMode_Single; 
    ADC_InitStructure.ADC_Prescaler = ADC_Prescaler_3CLOCK; 
    ADC_InitStructure.ADC_VREF = ADC_VREF_VDD;
    ADC_InitStructure.ADC_EAIN = Channel_List[0];
    ADC_Init(ADC, &ADC_InitStructure);
    
    // 6. 启动配置
    ADC_SetChannel(ADC, Channel_List[0]);
    
    // 【重点】手册里说 "59: ADCCON->DMAEN"
    // 这意味着 ADC 这一侧的 DMA 开关必须打开
    ADC_DMACmd(ADC, ENABLE); 
    
    // 双重保险：强制置位 ADC_IDE 寄存器的 DMAEN 位 (通常是 bit 0)
    // 如果你的头文件里没有 ADC_IDE，可能是 ADC_CON 的某个位，ADC_DMACmd 函数应该已经处理了
    // ADC->ADC_IDE |= 0x01; 

    // 开启 DMA
    DMA_Cmd(DMA0, ENABLE);
    // 开启 ADC
    ADC_Cmd(ADC, ENABLE);
    
    // 触发转换
    ADC_SoftwareStartConv(ADC);
}

void Data_Acquisition_Start_Next(void)
{
    if(Acq_Done_Flag == 1)
    {
        Acq_Done_Flag = 0;
        Current_Ch_Index = 0;

        ADC_SetChannel(ADC, Channel_List[0]);

        DMA_Cmd(DMA0, DISABLE);
        DMA_SetDstAddress(DMA0, (uint32_t)&Acq_Data.Buffer[0]);
        DMA_SetCurrDataCounter(DMA0, 1);
        DMA_Cmd(DMA0, ENABLE);

        // 再次触发
        ADC_SoftwareStartConv(ADC);
    }
}

// =================================================================
//  中断服务函数
//  请务必去 startup_sc32r803.s 确认：DMA0 的中断名是 DMA0_IRQHandler 
// =================================================================
void DMA0_IRQHandler(void)
{
    if(DMA_GetFlagStatus(DMA0, DMA_FLAG_TCIF) == SET)
    {
        DMA_ClearFlag(DMA0, DMA_FLAG_TCIF); 
        Debug_ISR_Count++; 

        DMA_Cmd(DMA0, DISABLE);
        
        // 关键：清除 ADC 标志，防止下次无法启动
        ADC_ClearFlag(ADC, ADC_Flag_ADCIF);

        Current_Ch_Index++;

        if(Current_Ch_Index < DAQ_SAMPLE_DEPTH) 
        {
            ADC_SetChannel(ADC, Channel_List[Current_Ch_Index]);
            DMA_SetDstAddress(DMA0, (uint32_t)&Acq_Data.Buffer[Current_Ch_Index]);
            DMA_SetCurrDataCounter(DMA0, 1); 
            DMA_Cmd(DMA0, ENABLE);
            ADC_SoftwareStartConv(ADC);
        }
        else
        {
            Acq_Done_Flag = 1; 
        }
    }
}