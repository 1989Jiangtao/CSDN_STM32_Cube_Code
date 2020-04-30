#include "oled.h"
#include "spi.h"
#include "ascii.h"
#include "number.h"
#include "hanzi.h"
#include "icon.h"
#include "delay.h"

// 反显位置初始化
u8 inverse_x = 0xFF , inverse_y = 0xFF ,inverse_sta = 0 ;

/****************
** 向OLED写一个字节的数据或是命令
** cmd: 1=命令，0=数据
** data: 待写入的字节（命令/数据）
*******************/
void OLED_Write_Byte( u8 data , u8 cmd )
{
    // 这里直接使用三目运算符，不使用if/else写法，看起来简洁些
    cmd ? OLED_DC_L : OLED_DC_H ; 				// 写命令DC输出低，写数据DC输出高
    // 硬件SPI数据传输
    HAL_SPI_Transmit(&hspi1,&data,sizeof(data),1000); // SPI写数据或者命令
    OLED_DC_H;														// 控制脚拉高，置成写数据状态
}


/*******************
** OLED初始化
********************/
void OLED_Init(void)
{
    // 制造一个先低后高的电平变换，达到复位的效果
    OLED_RST_L;
    delay_ms(500);
    OLED_RST_H;

    OLED_Write_Byte(0xAE,OLED_CMD);//--turn off oled panel
    OLED_Write_Byte(0x02,OLED_CMD);//---set low column address
    OLED_Write_Byte(0x10,OLED_CMD);//---set high column address
    OLED_Write_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    OLED_Write_Byte(0x81,OLED_CMD);//--set contrast control register
    OLED_Write_Byte(0xff,OLED_CMD);// Set SEG Output Current Brightness
    OLED_Write_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    OLED_Write_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    OLED_Write_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
    OLED_Write_Byte(0x3f,OLED_CMD);//--1/64 duty
    OLED_Write_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    OLED_Write_Byte(0x00,OLED_CMD);//-not offset
    OLED_Write_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
    OLED_Write_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
    OLED_Write_Byte(0xD9,OLED_CMD);//--set pre-charge period
    OLED_Write_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    OLED_Write_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
    OLED_Write_Byte(0x12,OLED_CMD);
    OLED_Write_Byte(0xDB,OLED_CMD);//--set vcomh
    OLED_Write_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
    OLED_Write_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
    OLED_Write_Byte(0x02,OLED_CMD);//
    OLED_Write_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
    OLED_Write_Byte(0x14,OLED_CMD);//--set(0x10) disable
    OLED_Write_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
    OLED_Write_Byte(0xA6,OLED_CMD);// 开启正常显示 (0xA6 = 正常 / 0xA7 = 反显)
    OLED_Write_Byte(0xAF,OLED_CMD);//--turn on oled panel

    OLED_Clear(); // 清屏
}

/******************
**  开启OLED显示
*******************/
void OLED_On(void)
{
    OLED_Write_Byte(0X8D,OLED_CMD);  //SET DCDC命令
    OLED_Write_Byte(0X14,OLED_CMD);  //DCDC ON
    OLED_Write_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}

/******************
**  关闭OLED显示
*******************/
void OLED_Off(void)
{
    OLED_Write_Byte(0X8D,OLED_CMD);  //SET DCDC命令
    OLED_Write_Byte(0X10,OLED_CMD);  //DCDC OFF
    OLED_Write_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}


/****************************
** 设置坐标点
** x: 行坐标 0~127
** y: 页坐标 0~7
*****************************/
void OLCD_Set_Pos(u8 x, u8 y)
{
    OLED_Write_Byte(0xb0+y,OLED_CMD);
    OLED_Write_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
    OLED_Write_Byte((x&0x0f)|0x01,OLED_CMD);
}

/********************
** OLED清屏操作
*********************/
void OLED_Clear(void)
{
    u8 i,n;
    for(i=0; i<8; i++)
    {
        OLED_Write_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
        OLED_Write_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
        OLED_Write_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址
        for(n=0; n<128; n++)OLED_Write_Byte(0,OLED_DATA); // 每一列都置零
    }
}

/***********************
** 测试ASCII字符
************************/
void OLED_Ascii_Test()
{
    u8 i,j;
    for(j=0; j<160; j++) {
        OLCD_Set_Pos(j*6%120,j/20); // 设置显示位置
        for(i=0; i<6; i++)
        {
            OLED_Write_Byte(F6x8[j%90][i],OLED_DATA);
        }
    }

}

/****************************************
** 设置反显位置
** x: 列坐标位置
** y: 页坐标位置
** sta: 1=反显，0=正常
*****************************************/
void OLED_Set_Inverse(u8 x , u8 y, u8 sta)
{
	if(x<MAX_LEN)inverse_x = x;
	if(y<8) inverse_y = y;
	inverse_sta = sta; 
}

/**************************************
** 显示一个字符
** x: 列坐标位置
** y: 页坐标位置
** chr: 待显示的字符
***************************************/
void OLED_Show_Char(u8 x, u8 y , u8 chr , enum Font_Size size)
{
    u8 i=0  ,c=0;
    c = chr - ' '; // 从空字符串算起，得到偏移后的值

    switch(size)
    {
    case SMALL:
        OLCD_Set_Pos(x,y); // 设置显示位置
        for(i=0; i< size ; i++)
            OLED_Write_Byte(F6x8[c][i],OLED_DATA);
        break;
    case MEDIA:
        OLCD_Set_Pos(x,y); // 设置显示位置
        for(i=0; i< 6 ; i++)
            OLED_Write_Byte(F12X6[c][i],OLED_DATA);
        OLCD_Set_Pos(x,y+1); // 设置显示位置
        for(i=6; i< 12 ; i++)
            OLED_Write_Byte(F12X6[c][i],OLED_DATA);
        break;
    case BIG:
        OLCD_Set_Pos(x,y); // 设置显示位置
				if(x==inverse_x && y==inverse_y) 
				{
					for(i=0; i< 8 ; i++)
            OLED_Write_Byte(~F16x8[c][i],OLED_DATA);
					OLCD_Set_Pos(x,y+1); // 设置显示位置
					for(i=8; i< 16 ; i++)
							OLED_Write_Byte(~F16x8[c][i],OLED_DATA);
				}else {
					for(i=0; i< 8 ; i++)
							OLED_Write_Byte(F16x8[c][i],OLED_DATA);
					OLCD_Set_Pos(x,y+1); // 设置显示位置
					for(i=8; i< 16 ; i++)
							OLED_Write_Byte(F16x8[c][i],OLED_DATA);
				}
        break;
    }
}

/****************************************
** 显示字符串
** x: 起始列坐标
** y: 起始页
** chr: 待显示的字符串
*****************************************/
void OLED_Show_String(u8 x, u8 y , char *chr , enum Font_Size size)
{
    u8 i = 0;
    u8 page =  (size>8 ? size >> 3 : 1 );  // 字体大小决定翻几页
    u8 len = (size>8 ? size >>1 : size ) ;   // 字体宽度，最后结果对应了字体表中的每行长度索引
    u8 limit = MAX_LEN/len - 1 ;
    while(chr[i] != '\0') {
        OLED_Show_Char(x,y,chr[i],size);
        x+=len; // 字体宽高 6*8
        if(x>(len*limit-1))    // 如果大于了极限值 128
        {
            x = 0 ;  //  列从零开始
            y += page;  //  另起一页
        }
        i ++ ;    // 循环继续，直到字符串都写完了为止
    }
}

/*********************************************************
** 自定义数字显示函数
** x: 列起始位置 0~(127-len) 取值从0到最后一个字符的起始位置
** y: 页起始位置 0~7
** num: 待显示的数字 0~9
** size: 字体大小枚举值
*********************************************************/
void OLED_Show_Num(u8 x, u8 y , u8 num , enum Num_Size size)
{
    u8 i,j ;
    u8 len = size >> 1 ;  // 字体宽度，这里直接处以2，因为字体宽度正好是高度的一半
    u8 page = size >> 3 ; // 这里处以8直接用移位操作，高度大于8就要翻页

    for(i = 0 ; i < page ; i ++)
    {
        OLCD_Set_Pos(x,y+i); // 设置显示位置
        switch(size)
        {
        case SMALL_NUM:
            for(j=0; j<len; j++)
                OLED_Write_Byte(N24x12[num*page + i][j],OLED_DATA);  // 写数据
            break;
        case MEDIA_NUM:
            for(j=0; j<len; j++)
                OLED_Write_Byte(N32x16[num*page + i][j],OLED_DATA);  // 写数据
            break;
        case BIG_NUM:
            for(j=0; j<len; j++)
                OLED_Write_Byte(N40x20[num*page + i][j],OLED_DATA);  // 写数据
            break;
        }
    }
}

/*********************************************************
** 自定义汉字显示函数
** x: 列起始位置 0~(127-len) 取值从0到最后一个字符的起始位置
** y: 页起始位置 0~7
** index: 待显示的汉字序号
** size: 字体大小枚举值
** reverse： 是否反显 1=是，0=否
*********************************************************/
void OLED_Show_Chinese(u8 x, u8 y , u8 index , enum Zh_Size size , u8 reverse)
{
    u8 i,j ;
    u8 len = size  ;  // 字体宽度，等于字体高度
    u8 page = size >> 3 ; // 这里处以8直接用移位操作，高度大于8就要翻页

    for(i = 0 ; i < page ; i ++)
    {
        OLCD_Set_Pos(x,y+i); // 设置显示位置
        switch(size)
        {
        case ZH16:
            for(j=0; j<len; j++)
                reverse?OLED_Write_Byte(~ZH16x16[index*page + i][j],OLED_DATA):OLED_Write_Byte(ZH16x16[index*page + i][j],OLED_DATA);  // 写数据
            break;
        case ZH32:
            for(j=0; j<len; j++)
                reverse?OLED_Write_Byte(~ZH32x32[index*page + i][j],OLED_DATA):OLED_Write_Byte(ZH32x32[index*page + i][j],OLED_DATA);  // 写数据
            break;
        }
    }
}

/*********************************************************
** 自定义图标显示
** x: 列起始位置 0~(127-len) 取值从0到最后一个字符的起始位置
** y: 页起始位置 0~7
** index: 待显示的图标序号，图标大小为16*16
*********************************************************/
void OLED_Show_Icon(u8 x, u8 y , u8 index)
{
    u8 i,j ;
    u8 len = 16  ;
    u8 page = 2 ;

    for(i = 0 ; i < page ; i ++)
    {
        OLCD_Set_Pos(x,y+i); // 设置显示位置
        for(j=0; j<len; j++)
            OLED_Write_Byte(ICON16[index*page + i][j],OLED_DATA);  // 写数据
    }
}

/************************************************************
** 滚动显示某一页
** start: 开始页 0 - 7
** end  : 结束页 0 - 7
** dirct：方向枚举，left向左，right向右
**************************************************************/
void OLED_Scroll_Display(u8 start,u8 end,enum DIRECTION dirct)
{
    if(start > 7 || end > 7) return; // 页码超出
    OLED_Write_Byte(0x2E ,OLED_CMD); // 关闭滚动
    OLED_Write_Byte(dirct,OLED_CMD); // 向左/右滚动
    OLED_Write_Byte(0x00 ,OLED_CMD); // 空制令
    OLED_Write_Byte(start,OLED_CMD); // 起始页
    OLED_Write_Byte(0x0F ,OLED_CMD); // 滚动间隔，0=不滚动，值越大，滚动越快
    OLED_Write_Byte(end  ,OLED_CMD); // 结束页
    OLED_Write_Byte(0x00 ,OLED_CMD); // 空指令
    OLED_Write_Byte(0xFF ,OLED_CMD); // 空指令，加两条空指令，不然不会生效
    OLED_Write_Byte(0x2F ,OLED_CMD); // 开启滚动
}

