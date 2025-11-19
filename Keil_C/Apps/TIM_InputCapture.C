#include "TIM_InputCapture.h"

static volatile uint8_t TIM_ICNum = 0;//自定义变量捕获计数

typedef struct
{
  uint16_t FirstRising;
  uint16_t FirstFailing;
  uint16_t SecondRising;
}
TIM_ICValueTypeDef;

TIM_ICValueTypeDef TIM_ICValue;

void TIM_InputCapture(void)
{
  volatile uint16_t TIM_IC_Cycle, TIM_IC_Duty;//自定义变量存放捕获周期值和捕获占空比值

  while(1)
  {
    if(TIM_ICNum == 3)//捕获完成
    {
      /* 周期为两次上升沿的差值，占空比为下降沿减去第一次上升沿的差值 */
      TIM_IC_Cycle = TIM_ICValue.SecondRising - TIM_ICValue.FirstRising;//捕获到的周期值
      TIM_IC_Duty = TIM_ICValue.FirstFailing - TIM_ICValue.FirstRising;//捕获到的占空比值

      TIM_ICNum = 0;//捕获计数清零，为下一次捕获做准备
    }
  }
}

void TIM_InputCapture_TIM1Handler(void)
{
  /* 判断TIM1中断是否置起 */
  if(TIM_GetFlagStatus(TIM1, TIM_Flag_TI) == SET)
  {
    TIM_ClearFlag(TIM1, TIM_Flag_TI);//清除TIM1中断
    TIM_ICNum = 0;//清除捕获标志值
  }
  if(TIM_GetFlagStatus(TIM1, TIM_Flag_EXR) == SET)
  {
    TIM_ClearFlag(TIM1, TIM_Flag_EXR);//清除TIM1中断
    if(TIM_ICNum == 0)//采集到第一次上升沿
    {
      TIM_ICNum ++;
      TIM_ICValue.FirstRising = TIM_GetRisingCapture(TIM1);
    }
    if(TIM_ICNum == 2)//采集到第二次上升沿
    {
      TIM_ICNum ++;
      TIM_ICValue.SecondRising = TIM_GetRisingCapture(TIM1);
    }
  }
  if(TIM_GetFlagStatus(TIM1, TIM_Flag_EXF) == SET)
  {
    TIM_ClearFlag(TIM1, TIM_Flag_EXF);//清除TIM1中断
    if(TIM_ICNum == 1)//采集到第一次下降沿
    {
      TIM_ICNum++;
      TIM_ICValue.FirstFailing = TIM_GetFailingCapture(TIM1);
    }
  }
}

