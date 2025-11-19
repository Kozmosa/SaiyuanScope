//************************************************************
//  Copyright (c)
//	FileName	  : TFT_Lcd.C
//	Function	  : TFT LCD Init
//  Instructions  :
//  Date          : 2025/9/10
// 	Version		  : V1.0
//*************************************************************
#include "TFT_Lcd.h"
#include "lcdfont.h"

void delay_ms ( uint32_t ms )
{
    while ( ms-- );
}
/**************************************************
*函数名称：void LCD_WR_DATA ( uint16_t  dat,uint8_t Datalength)
*函数功能：向LCD发送一个BYTE函数
*入口参数：
uint16_t  dat：数据
uint8_t Datalength：数据长度
*出口参数：void
*功能说明：
**************************************************/
void LCD_WR_DATA ( uint16_t  dat,uint8_t Datalength)
{
    if(Datalength==0x08)
		{			
	     SPI_DataSizeConfig(SPI1,SPI_DataSize_8B);
		}
		else
		{
			SPI_DataSizeConfig(SPI1,SPI_DataSize_16B);
		}
		
	  LCD_CS_Clr();
    delay_ms ( 5 );
    SPI_SendData ( SPI1, dat );
    while ( !SPI_IF );
    CLEAR_SPI_IF();
    delay_ms ( 5 );
    LCD_CS_Set();
}
/**************************************************
*函数名称：void LCD_WR_REG ( uint16_t dat )
*函数功能：向LCD发送一个命令
*入口参数：uint16_t dat:命令数据
*出口参数：void
*功能说明：
**************************************************/
void LCD_WR_REG ( uint16_t dat )
{
    LCD_DC_Clr();//打开写命令
    delay_ms ( 5 );   
 	  LCD_WR_DATA ( dat,8);
    delay_ms ( 5 );
    LCD_DC_Set();//返回默认写数据
}
/**************************************************
*函数名称：void Lcd_Init ( void )
*函数功能：初始化LCD屏
*入口参数：
*出口参数：void
*功能说明：
**************************************************/
void Lcd_Init ( void )
{
//    LCD_RES_Clr();
//    delay_ms ( 50 );
//    LCD_RES_Set();
//		GPIO_SetBits(GPIOA,GPIO_Pin_15);
		GPIO_SetBits(GPIOB,GPIO_Pin_5);
		GPIO_SetBits(GPIOB,GPIO_Pin_2);
    delay_ms ( 50 );
    //************* Start Initial Sequence **********//
    LCD_WR_REG ( 0x11 ); //Sleep out
		//-----------------------ST7789V Frame rate setting-----------------//
		LCD_WR_REG(0x3A);        //65k mode
    LCD_WR_DATA(0x05,0x08);
    LCD_WR_REG(0xC5); 		//VCOM
    LCD_WR_DATA(0x1A,0x08);
    LCD_WR_REG(0x36);                 // 屏幕显示方向设置
    LCD_WR_DATA(0x60,0x08);
		//-------------ST7789V Frame rate setting-----------//
		LCD_WR_REG(0xB2);  //Porch Setting
		LCD_WR_DATA(0x05,0x08);
		LCD_WR_DATA(0x05,0x08);
		LCD_WR_DATA(0x00,0x08);
		LCD_WR_DATA(0x33,0x08);
		LCD_WR_DATA(0x33,0x08);
	
    LCD_WR_REG ( 0xB7 ); //GCTRL (Gate Control)
    LCD_WR_DATA ( 0x05,0x08 ); //12.2v   -10.43v
    //------------------------------------ST7735S Power Sequence-----------------------------------------//
		LCD_WR_REG(0xBB);//VCOM
    LCD_WR_DATA(0x3F,0x08);

    LCD_WR_REG(0xC0); //Power control
    LCD_WR_DATA(0x2c,0x08);

    LCD_WR_REG(0xC2);		//VDV and VRH Command Enable
    LCD_WR_DATA(0x01,0x08);

    LCD_WR_REG(0xC3);			//VRH Set
    LCD_WR_DATA(0x0F,0x08);		//4.3+( vcom+vcom offset+vdv)

    LCD_WR_REG(0xC4);			//VDV Set
    LCD_WR_DATA(0x20,0x08);				//0v

    LCD_WR_REG(0xC6);				//Frame Rate Control in Normal Mode
    LCD_WR_DATA(0X01,0x08);			//111Hz

    LCD_WR_REG(0xd0);				//Power Control 1
    LCD_WR_DATA(0xa4,0x08);
    LCD_WR_DATA(0xa1,0x08);

    LCD_WR_REG(0xE8);				//Power Control 1
    LCD_WR_DATA(0x03,0x08);

    LCD_WR_REG(0xE9);				//Equalize time control
    LCD_WR_DATA(0x09,0x08);
    LCD_WR_DATA(0x09,0x08);
    LCD_WR_DATA(0x08,0x08);
    //---------------------------------End ST7789V Power Sequence---------------------------------------//

    //------------------------------------ST7789V Gamma Sequence-----------------------------------------//
    LCD_WR_REG ( 0xE0 );
    LCD_WR_DATA(0xD0,0x08);
		LCD_WR_DATA(0x05,0x08);
		LCD_WR_DATA(0x09,0x08);
		LCD_WR_DATA(0x09,0x08);
		LCD_WR_DATA(0x08,0x08);
		LCD_WR_DATA(0x14,0x08);
		LCD_WR_DATA(0x28,0x08);
		LCD_WR_DATA(0x33,0x08);
		LCD_WR_DATA(0x3F,0x08);
		LCD_WR_DATA(0x07,0x08);
		LCD_WR_DATA(0x13,0x08);
		LCD_WR_DATA(0x14,0x08);
		LCD_WR_DATA(0x28,0x08);
		LCD_WR_DATA(0x30,0x08);

    LCD_WR_REG ( 0xE1 );
		LCD_WR_DATA(0xD0,0x08);
		LCD_WR_DATA(0x05,0x08);
		LCD_WR_DATA(0x09,0x08);
    LCD_WR_DATA(0x09,0x08);
    LCD_WR_DATA(0x08,0x08);
    LCD_WR_DATA(0x03,0x08);
    LCD_WR_DATA(0x24,0x08);
    LCD_WR_DATA(0x32,0x08);
    LCD_WR_DATA(0x32,0x08);
    LCD_WR_DATA(0x3B,0x08);
    LCD_WR_DATA(0x14,0x08);
    LCD_WR_DATA(0x13,0x08);
    LCD_WR_DATA(0x28,0x08);
    LCD_WR_DATA(0x2F,0x08);

    //------------------------------------End ST7789V Gamma Sequence-----------------------------------------//
		LCD_WR_REG(0x20);  //反显

		LCD_WR_REG(0x11); 
		delay_ms (120); 

		LCD_WR_REG(0x29); //Display on
}
/**************************************************
*函数名称：void LCD_Address_Set ( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2 )
*函数功能：设置数据写入范围
*入口参数：
uint16_t x1：起点横坐标
uint16_t y1：起点纵坐标
uint16_t x2：终点横坐标
uint16_t y2：终点纵坐标
*出口参数：void
*功能说明：
**************************************************/
void LCD_Address_Set ( uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2 )
{
    LCD_WR_REG ( 0x2a ); //列地址设置
    LCD_WR_DATA ( x1+X_PIXEL_START  ,16);
    LCD_WR_DATA ( x2+X_PIXEL_START  ,16);
    LCD_WR_REG ( 0x2b ); //行地址设置
    LCD_WR_DATA ( y1+Y_PIXEL_START ,16);
    LCD_WR_DATA ( y2+Y_PIXEL_START ,16);
    LCD_WR_REG ( 0x2c ); //储存器写
}
/**************************************************
*函数名称：void LCD_Fill ( uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color )
*函数功能：向指定地方填充指定颜色
*入口参数：
uint16_t xsta：起点横坐标
uint16_t ysta：起点纵坐标
uint16_t xend：终点横坐标
uint16_t yend：终点纵坐标
uint16_t color：填充的颜色
*出口参数：void
*功能说明：
**************************************************/
void LCD_Fill ( uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color )
{
    uint16_t i, j;
    LCD_Address_Set ( xsta, ysta, xend - 1, yend - 1 ); //设置显示范围
	  
    for ( i = ysta; i < yend; i++ )
    {
        for ( j = xsta; j < xend; j++ )
        {
            LCD_WR_DATA ( color,16 );
        }
    }
}

/**************************************************
*函数名称：void LCD_DrawPoint ( uint16_t x, uint16_t y, uint16_t color )
*函数功能：擦除一个像素点的颜色
*入口参数：
uint16_t x：起点横坐标
uint16_t y：起点纵坐标
uint16_t color：填充的颜色
*出口参数：void
*功能说明：
**************************************************/
void LCD_DrawPoint ( uint16_t x, uint16_t y, uint16_t color )
{
    LCD_Address_Set ( x, y, x, y ); //设置光标位置
    LCD_WR_DATA ( color ,16);
}
/**************************************************
*函数名称：void LCD_ShowChar ( uint16_t x, uint16_t y, uint8_t num, uint16_t fc, uint16_t bc, uint8_t sizey)
*函数功能：在指定地方显示一个字符
*入口参数：uint16_t x:横坐标
uint16_t y：纵坐标
int8_t num：字符地址
uint16_t fc：数字颜色
uint16_t bc：背景颜色
uint8_t sizey：字体库（16或32）
*出口参数：void
*功能说明：
**************************************************/
void LCD_ShowChar ( uint16_t x, uint16_t y, uint8_t num, uint16_t fc, uint16_t bc, uint8_t sizey)
{
    uint8_t temp, sizex, t;
    uint16_t i, TypefaceNum; //一个字符所占字节大小
    sizex = sizey / 2;
    TypefaceNum = sizex / 8 * sizey;
    num = num - ' '; //得到偏移后的值
    LCD_Address_Set ( x, y, x + sizex - 1, y + sizey - 1 ); //设置光标位置
    for ( i = 0; i < TypefaceNum; i++ )
    {
        if ( sizey == 16 ) 
				{
					  temp = ascii_1608[num][i];       //调用8x16字体
				}
        else if ( sizey == 32 ) 
				{
					temp = ascii_3216[num][i]; //调用16x32字体
				}
        else 
				{
					return;
				}
        for ( t = 0; t < 8; t++ )
        {
           
							if ( temp & ( 0x01 << t ) ) 
							{
								 LCD_WR_DATA ( fc,16 );
							}
							else 
							{
								 LCD_WR_DATA ( bc ,16);
							}
            
        }
    }
}
/**************************************************
*函数名称：void LCD_ShowString(uint16_t x,uint16_t y,const uint8_t *p,uint16_t fc,uint16_t bc,uint8_t sizey)
*函数功能：在指定地方显示一串字符
*入口参数：uint16_t x:横坐标
uint16_t y：纵坐标
const uint8_t *p：字符地址
uint16_t fc：数字颜色
uint16_t bc：背景颜色
uint8_t sizey：字体库（16或32）
*出口参数：void
*功能说明：
**************************************************/
void LCD_ShowString(uint16_t x,uint16_t y,const uint8_t *p,uint16_t fc,uint16_t bc,uint8_t sizey)
{         
	while(*p!='\0')
	{       
		LCD_ShowChar(x,y,*p,fc,bc,sizey);
		x+=sizey/2;
		p++;
	}  
}
/**************************************************
*函数名称：void mypow ( uint8_t m, uint8_t n )
*函数功能：显示数字
*入口参数：
uint8_t m：底数
uint8_t n：指数
*出口参数：void
*功能说明：
**************************************************/
uint32_t mypow ( uint8_t m, uint8_t n )
{
    uint32_t result = 1;
    while ( n-- ) result *= m;
    return result;
}
/**************************************************
*函数名称：void LCD_ShowIntNum ( uint16_t x, uint16_t y, uint16_t num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey )
*函数功能：在指定地方显示一串数字
*入口参数：uint16_t x:横坐标
uint16_t y：纵坐标
uint16_t num：数字
uint8_t len：数字长度
uint16_t fc：数字颜色
uint16_t bc：背景颜色
uint8_t sizey：字体库（16或32）
*出口参数：void
*功能说明：
**************************************************/
void LCD_ShowIntNum ( uint16_t x, uint16_t y, uint16_t num, uint8_t len, uint16_t fc, uint16_t bc, uint8_t sizey )
{
    uint8_t t, temp;
    uint8_t enshow = 0;
    uint8_t sizex = sizey / 2;
    for ( t = 0; t < len; t++ )
    {
        temp = ( num / mypow ( 10, len - t - 1 ) ) % 10;
        if ( enshow == 0 && t < ( len - 1 ) )
        {
            if ( temp == 0 )
            {
                LCD_ShowChar ( x + t * sizex, y, ' ', fc, bc, sizey);
                continue;
            } 
						else 
						{
							enshow = 1;
						}

        }
        LCD_ShowChar ( x + t * sizex, y, temp + 48, fc, bc, sizey);
    }
}
/**************************************************
*函数名称：void LCD_ShowChinese16x16(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey）
*函数功能：显示一个汉字
*入口参数：
uint16_t x：起点横坐标
uint16_t u：起点纵坐标
uint8_t *s：汉字数组地址
uint16_t fc：字体颜色
uint16_t bc：背景颜色
uint8_t sizey：字体大小
*出口参数：void
*功能说明：
**************************************************/
void LCD_ShowChinese12x12(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey)
{
	uint8_t i,j;
	uint16_t k;
	uint16_t HZnum;//汉字数目
	uint16_t TypefaceNum;//一个字符所占字节大小
	TypefaceNum = 24;
	
	HZnum=sizeof(tfont16)/sizeof(typFNT_GB16);	//统计汉字数目
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont16[k].Index[0]==*(s))&&(tfont16[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{		
            if(i%2 != 0)	
						{
							  if(j >= 16 - sizey)
								{
									break; //跳出多余的像素
								}
						}							
						if(tfont16[k].Msk[i]&(0x01<<j))
						{
							 LCD_WR_DATA(fc,16);
						}
						else
						{
							LCD_WR_DATA(bc,16);
						}
									
				}
			}
			break;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
		}				  	
		
	}
} 
/**************************************************
*函数名称：void LCD_ShowChinese16x16(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey）
*函数功能：显示一个汉字
*入口参数：
uint16_t x：起点横坐标
uint16_t u：起点纵坐标
uint8_t *s：汉字数组地址
uint16_t fc：字体颜色
uint16_t bc：背景颜色
uint8_t sizey：字体大小
*出口参数：void
*功能说明：
**************************************************/
void LCD_ShowChinese16x16(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey)
{
	uint8_t i,j;
	uint16_t k;
	uint16_t HZnum;//汉字数目
	uint16_t TypefaceNum;//一个字符所占字节大小
	TypefaceNum = sizey/8*sizey;//此算法只适用于字宽等于字高，且字高是8的倍数的字，
	                          //也建议用户使用这样大小的字,否则显示容易出问题！
	HZnum=sizeof(tfont16)/sizeof(typFNT_GB16);	//统计汉字数目
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont16[k].Index[0]==*(s))&&(tfont16[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{						
						if(tfont16[k].Msk[i]&(0x01<<j))
						{
							 LCD_WR_DATA(fc,16);
						}
						else
						{
							LCD_WR_DATA(bc,16);
						}
									
				}
			}
			break;  //查找到对应点阵字库立即退出，防止多个汉字重复取模带来影响
		}				  	
		
	}
}
/**************************************************
*函数名称：void LCD_ShowChinese(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey)
*函数功能：显示多个汉字
*入口参数：
uint16_t x：起点横坐标
uint16_t u：起点纵坐标
uint8_t *s：汉字数组地址
uint16_t fc：字体颜色
uint16_t bc：背景颜色
uint8_t sizey：字体大小
*出口参数：void
*功能说明：
**************************************************/
void LCD_ShowChinese(uint16_t x,uint16_t y,uint8_t *s,uint16_t fc,uint16_t bc,uint8_t sizey)
{
	while(*s!=0)
	{
		if(sizey == 16)
		{
		   LCD_ShowChinese16x16(x,y,s,fc,bc,sizey);	
		}
		else 
		{
			 LCD_ShowChinese12x12(x,y,s,fc,bc,sizey);	
		}			
		s+=2;
		x+=sizey;
	}
}
/**************************************************
*函数名称：void LCD_ScrollReset ( uint16_t Topfix, uint16_t ScrollNum, uint16_t Botfix,uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color, uint16_t xsta1)
*函数功能：滚动显示并在滚动过程中清除图像（边滚动边清除）
*入口参数：
uint16_t Topfix：顶部固定区域
uint16_t ScrollNum：滚动区域
uint16_t Botfix：底部固定区域
uint16_t xsta：清除区域开始行
uint16_t ysta：清除区域开始列
uint16_t xend：清除区域结束行
uint16_t yend：清除区域结束列
uint16_t color：清除区域填充颜色
uint16_t xsta1：指定行滚动到顶部区域
*出口参数：void
*功能说明：
**************************************************/
void LCD_ScrollReset ( uint16_t Topfix, uint16_t ScrollNum, uint16_t Botfix,uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color, uint16_t xsta1)
{
	  LCD_WR_REG ( 0x33 ); //滚动区域限制
    LCD_WR_DATA ( Topfix,16);
    LCD_WR_DATA ( ScrollNum,16);
	  LCD_WR_DATA ( Botfix,16);		
	  LCD_Fill(xsta,ysta,xend,yend,color);//设置刷新区域的行	  
	  LCD_WR_REG ( 0x37 );//设置滚动到顶部的行
	  LCD_WR_DATA ( xsta1,16 );
}

/**************************************************
*函数名称：void LCD_ScrollSet ( uint16_t Topfix, uint16_t ScrollNum, uint16_t Botfix,uint16_t xsta)
*函数功能：滚动显示，不清除图像
*入口参数：
uint16_t Topfix：顶部固定区域
uint16_t ScrollNum：滚动区域
uint16_t Botfix：底部固定区域
uint16_t xsta：指定行滚动到顶部区域
*出口参数：void
*功能说明：
**************************************************/
void LCD_ScrollSet ( uint16_t Topfix, uint16_t ScrollNum, uint16_t Botfix, uint16_t xsta)
{
	  LCD_WR_REG ( 0x33 ); //滚动区域限制
    LCD_WR_DATA ( Topfix,16);
    LCD_WR_DATA ( ScrollNum,16);
	  LCD_WR_DATA ( Botfix,16);	
	  LCD_WR_REG ( 0x37 );//设置滚动到顶部的行
	  LCD_WR_DATA ( xsta,16 );
}

/*
*   函数内容：  画线
*   函数参数：  x1,y1   起始坐标
*               x2,y2   终止坐标
*               color   要填充的颜色
*   返回值：    无
*/
void TFT_DrawLine(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t color)
{ 
	uint16_t t=0; 
	int xerr=0,yerr=0,delta_x=0,delta_y=0,distance=0;
	int incx=0,incy=0,uRow=0,uCol=0;
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1;
	uRow=x1;//画线起点坐标
	uCol=y1;
	if(delta_x>0){
        incx=1; //设置单步方向 
    }
	else if (delta_x==0){
        incx=0;//垂直线 
    }
	else {
        incx=-1;
        delta_x=-delta_x;
    }
	if(delta_y>0){
        incy=1;
    }
	else if (delta_y==0){
        incy=0;//水平线 
    }
	else {
        incy=-1;
        delta_y=-delta_y;
    }
	if(delta_x>delta_y){
        distance=delta_x; //选取基本增量坐标轴 
    }
	else {
        distance=delta_y;
    }
	for(t=0;t<distance+1;t++)
	{
		LCD_DrawPoint(uRow,uCol,color);//画点       
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}

/*
*   函数内容：  画矩形
*   函数参数：  x1,y1   起始坐标
*               x2,y2   终止坐标
*               color   矩形的颜色
*   返回值：    无
*/
void TFT_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint16_t color)
{
	TFT_DrawLine(x1,y1,x2,y1,color);
	TFT_DrawLine(x1,y1,x1,y2,color);
	TFT_DrawLine(x1,y2,x2,y2,color);
	TFT_DrawLine(x2,y1,x2,y2,color);
}

/*
*   函数内容：  画圆
*   函数参数：  x0,y0   圆心坐标
*               r       半径
*               color   矩形的颜色
*   返回值：    无
*/
void Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r,uint16_t color)
{
	uint16_t a=0,b=0;
	a=0;b=r;	  
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a,color);             //3           
		LCD_DrawPoint(x0+b,y0-a,color);             //0           
		LCD_DrawPoint(x0-a,y0+b,color);             //1                
		LCD_DrawPoint(x0-a,y0-b,color);             //2             
		LCD_DrawPoint(x0+b,y0+a,color);             //4               
		LCD_DrawPoint(x0+a,y0-b,color);             //5
		LCD_DrawPoint(x0+a,y0+b,color);             //6 
		LCD_DrawPoint(x0-b,y0+a,color);             //7
		a++;
		if((a*a+b*b)>(r*r))//判断要画的点是否过远
		{
			b--;
		}
	}
}


static uint16_t lastX=0,lastY=0;
static uint8_t firstPoint = 1;

/*
*   函数内容：画折线
*   函数参数：short int rawValue--Y轴参数值
*   返回值：  无
*/
void drawCurve(uint8_t yOffset,short int rawValue)  
{
	uint16_t x=0,y=0,i=0;
    
	if((rawValue >= 50)||(rawValue < 0)){
			rawValue=50;
	}
    
	y = yOffset - rawValue;  	//data processing code
	if(firstPoint)//如果是第一次画点，则无需连线，直接描点即可
	{
		LCD_DrawPoint(0,y,BLUE);   
		lastX=0;
		lastY=y;
		firstPoint=0; 
	}
	else
	{
		x=lastX+1;
        
		if(x<280)  //不超过屏幕宽度
		{
			TFT_DrawLine(lastX,lastY,x,y,BLUE);
			//for(i=30;i<=80;i++)
			//{
			//		TFT_DrawPoint(x+1,i,BLACK);//画点
			//}
			LCD_Address_Set(x+1,40,x+1,240);
			for(i=40;i<=240;i++)
			{
					LCD_WR_DATA(BLACK,16);
			}	
			lastX=x;
			lastY=y;
		}
		else  //超出屏幕宽度，清屏，从第一个点开始绘制，实现动态更新效果
		{         
			for(i=40;i<=240;i++)
			{
					LCD_DrawPoint(1,i,BLACK);
			}
			LCD_DrawPoint(0,y,BLUE);
			lastX=0;
			lastY=y;
		}
  }
}

