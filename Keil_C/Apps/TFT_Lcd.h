//************************************************************
//  Copyright (c)
//	FileName	  : TFT_Lcd.h
//	Function	  : TFT LCD Display
//  Instructions  :
//  Date          : 2023/09/01
// 	Version		  : V1.0
//*************************************************************
#ifndef __TFT_LCD_H
#define __TFT_LCD_H

#include "SC_Init.h"
#include "SC_it.h"
#include "..\Drivers\SCDriver_list.h"
#include "HeadFiles\SysFunVarDefine.h"

//背光常开
//#define LCD_BLK_Clr() PX_BIT(n)=0
//#define LCD_BLK_Set() PX_BIT(n)=1

#define LCD_CS_Clr()   PB_BIT(4)=0
#define LCD_CS_Set()   PB_BIT(4)=1

#define LCD_DC_Clr()   PB_BIT(3)=0
#define LCD_DC_Set()   PB_BIT(3)=1

//#define LCD_RES_Clr()  PB_BIT(2)=0
//#define LCD_RES_Set()  PB_BIT(2)=1

//判断SPI标志位
#define SPI_IF  (ITStatus)(SPI1->SPI_STS & SPI_FLAG_QTWIF)
//清除SPI标志位
#define CLEAR_SPI_IF()  SPI1->SPI_STS=(uint16_t)SPI_FLAG_QTWIF         
////设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏
#define USE_HORIZONTAL 3

#define X_PIXEL_START 0   //与硬件有关,1~160
#define Y_PIXEL_START 0   //与硬件有关,1~132

//#define X_PIXEL_START 3   //与硬件有关,1~160
//#define Y_PIXEL_START 2   //与硬件有关,1~132

#define PINK   0xD2F5
#define BLUE   0x03bD
#define RED    0xF882
#define YELLOW 0xFFE0
#define WHITE  0xffff
#define BLACK  0x0000

#define SPI_GET_DMA_STATUS   DMA_SPI_FLAG==RESET

#define  DMA0_HakfWordSend()     DMA0->DMA_CFG &= (uint32_t)(~0x0000000C);\
                                 DMA0->DMA_CFG |= DMA_DataSize_HalfWord;
                                    
#define  DMA0_ByteSend()        DMA0->DMA_CFG &= (uint32_t)(~0x0000000C);\
                                DMA0->DMA_CFG |= DMA_DataSize_Byte;
#define  DMA0_SrcFIXED()   DMA0->DMA_CFG &= (uint32_t)(~DMA_SourceMode_INC_CIRC); \
                           DMA0->DMA_CFG |= DMA_SourceMode_FIXED;
#define  DMA0_SrcINC()     DMA0->DMA_CFG &= (uint32_t)(~DMA_SourceMode_INC_CIRC); \
                           DMA0->DMA_CFG |= DMA_SourceMode_INC;
													 
#define  DMA1_HakfWordSend()     DMA1->DMA_CFG &= (uint32_t)(~0x0000000C);\
                                 DMA1->DMA_CFG |= DMA_DataSize_HalfWord;
                                    
#define  DMA1_ByteSend()        DMA1->DMA_CFG &= (uint32_t)(~0x0000000C);\
                                DMA1->DMA_CFG |= DMA_DataSize_Byte;
#define  DMA1_SrcFIXED()   DMA1->DMA_CFG &= (uint32_t)(~DMA_SourceMode_INC_CIRC); \
                           DMA1->DMA_CFG |= DMA_SourceMode_FIXED;
#define  DMA1_SrcINC()     DMA1->DMA_CFG &= (uint32_t)(~DMA_SourceMode_INC_CIRC); \
                           DMA1->DMA_CFG |= DMA_SourceMode_INC;
													 



void delay_ms ( uint32_t ms );									
void LCD_WR_DATA(uint16_t  dat,uint8_t Datalength);
void LCD_WR_REG(uint16_t dat);
void Lcd_Init(void);
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
void LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color);
void LCD_Clear (uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend,volatile uint16_t color );
void LCD_DrawPoint ( uint16_t x, uint16_t y, uint16_t color );
void LCD_ShowPicture(uint16_t x,uint16_t y,uint16_t length,uint16_t width,const uint8_t pic[]);
void LCD_ShowChar ( uint16_t x, uint16_t y, uint8_t num, uint16_t fc, uint16_t bc, uint8_t sizey);
void LCD_ShowString(uint16_t x,uint16_t y,const uint8_t *p,uint16_t fc,uint16_t bc,uint8_t sizey);
void LCD_ShowIntNum ( uint16_t x, uint16_t y, uint16_t num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey );
void LCD_ShowChinese(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey);
void LCD_ScrollSet ( uint16_t Topfix, uint16_t ScrollNum, uint16_t Botfix, uint16_t xsta );
void LCD_ScrollReset ( uint16_t Topfix, uint16_t ScrollNum, uint16_t Botfix,uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color, uint16_t xsta1);
void TFT_DrawLine(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color);
void TFT_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color);
void Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r,uint16_t color);
void drawCurve(uint8_t yOffset,short int rawValue);
void LCD_ShowChinese12x12(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey);
#endif
