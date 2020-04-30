/***************************************
** OLED 头文件
** 作者：菜鸟江涛
** 时间：2020-03-17
** CSDN: https://blog.csdn.net/u010898329
**************************************/
#ifndef __OLED_H
#define __OLED_H

// 由于会使用到GPIO口，所以引入相关的头文件
#include "main.h"

// 常量定义
#define u8         unsigned char  // 将unsigned char 定义简写
#define MAX_LEN    128  // 列最多像素点
#define OLED_CMD   1  // 写命令
#define OLED_DATA  0  // 写数据

/**********************************
** 引脚对应关系
** SPI_NSS(PB12)  -->   OLED_CS
** SPI_SCK(PB13)  -->   OLED_SCK
** SPI_MOSI(PB15) -->   OLED_SDA
** 				PB11    -->   OLED_RST
** 				PB10    -->   OLED_DC
***********************************/

// 定义字体枚举，值为字体的高度
enum Font_Size {SMALL = 6 , MEDIA = 12 , BIG = 16};
// 定义数字枚举，值为数字的高度
enum Num_Size {SMALL_NUM = 24 , MEDIA_NUM = 32 , BIG_NUM = 40};
// 定义汉字的大小枚举，值为字体的高度
enum Zh_Size {ZH16 = 16 , ZH32 = 32};
// 滚动方向枚举
enum DIRECTION { LEFT = 0x27 , RIGHT = 0x26};

// 端口高低操纵定义
#define OLED_RST_H HAL_GPIO_WritePin(OLED_RST_GPIO_Port,OLED_RST_Pin,GPIO_PIN_SET)    // 复位脚高
#define OLED_RST_L HAL_GPIO_WritePin(OLED_RST_GPIO_Port,OLED_RST_Pin,GPIO_PIN_RESET)  // 复位脚低

#define OLED_DC_H HAL_GPIO_WritePin(OLED_DC_GPIO_Port,OLED_DC_Pin,GPIO_PIN_SET)  			// 控制脚高
#define OLED_DC_L HAL_GPIO_WritePin(OLED_DC_GPIO_Port,OLED_DC_Pin,GPIO_PIN_RESET)  		// 控制脚低

// 函数定义
// OLED的基础操作函数
void OLED_Write_Byte(u8 data , u8 cmd);
void OLED_Init(void);
void OLED_On(void);
void OLED_Off(void);
void OLCD_Set_Pos(u8 x, u8 y);

void OLED_Ascii_Test(void);

// OLED的显示操作函数
void OLED_Clear(void);
void OLED_Show_Char(u8 x, u8 y , u8 chr , enum Font_Size size);
void OLED_Show_String(u8 x, u8 y , char *chr , enum Font_Size size);
void OLED_Show_Num(u8 x, u8 y , u8 num , enum Num_Size size);
void OLED_Show_Chinese(u8 x, u8 y , u8 index , enum Zh_Size size , u8 reverse);
void OLED_Show_Icon(u8 x, u8 y , u8 index);
void OLED_Scroll_Display(u8 start,u8 end,enum DIRECTION dirct);
void OLED_Set_Inverse(u8 x , u8 y, u8 sta);

#endif

