#ifndef __INFRARED_H
#define __INFRARED_H

// 引入头文件
#include "main.h"
#include "tim.h"

#define IR_IN HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) 		// 定义红外捕获端口

// 定义函数
void Infrared_Init(void); // 初始化函数
uint8_t IRCode_Process(void); // 红外解码处理函数

#endif
