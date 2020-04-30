#include "infrared.h"

/***********************************
**  红外码值表
**
**  1(0xA2)  2(0x62)  3(0xE2)
**  4(0x22)  5(0x02)  6(0xC2)
**  7(0xE0)  8(0xA8)  9(0x90)
**  *(0x68)  0(0x98)  #(0xB0)
**
**           U(0x18)  
**  L(0x10)  OK(0x38)  R(0x5A)
**  			   D(0x4A)  
**
************************************/

uint8_t OK=0;										// 红外引导码有效标志位：1=有效
//uint8_t ReadyFlag=0;						// 红外接收完成标志位：1=完成
uint16_t TimeOut=0; 						// 超时时间计数
uint16_t KeyCount=0;						// 按键连按次数
uint32_t OrderData;						// 原始的红外数据：正码+反码
uint8_t  IRCode;							// 解析后的码值

// 初始化函数，主要是开定时器中断
void Infrared_Init(void)
{
	HAL_TIM_Base_Start(&htim2);
	HAL_TIM_Base_Start_IT(&htim2);
	HAL_TIM_IC_Start(&htim2,TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&htim2,TIM_CHANNEL_1);
}

// 红外解码处理函数
uint8_t IRCode_Process(void)
{
	uint8_t t1 ,t2 ;
	t1 = OrderData >> 24 ;
	t2 = (OrderData >> 16) & 0xFF ;
//	ReadyFlag = 0;
	OK = 0 ;
	if(t1 ==(uint8_t) ~t2)
	{
		t1 = OrderData >> 8;
		t2 = OrderData ;
		if(t1 ==(uint8_t) ~ t2) return t1;
	}
	return 0;
}

// 捕获中断回调函数，每次捕获到信号就会进入这个回调函数
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	uint32_t fallingCount = 0 ; // 下降沿计数
	uint8_t temp = 0 ;
		
	// 判断是否是定时器2的外部捕获口1
	if(htim->Instance == TIM2)
	{
		// 捕获到了上升沿
		if(IR_IN)
		{
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING); // 改变捕获极性为下降沿捕获
			__HAL_TIM_SET_COUNTER(htim, 0); // 计数清零，从头开始计
		}
		else
		{
			fallingCount = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1); // 读取捕获计数，这个时间即为上升沿持续的时间
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING); // 改变捕获极性为上升沿捕获
			if((fallingCount > 4400) && (fallingCount < 4600)) OK = 1;// 4.5ms引导电平
			else if ((fallingCount > 500) && (fallingCount < 600))	temp = 0;//560 us
			else if ((fallingCount > 1600) && (fallingCount < 1750))	temp = 1;//1680 us
			else if ((fallingCount > 2000) && (fallingCount < 4000))	//2.5 ms
			{
//			  ReadyFlag = 1 ;
				KeyCount ++ ;
			}
			if(OK)
			{
				OrderData <<= 1 ;
				OrderData += temp ;
				KeyCount = 0; // 按键次数
			}
		}
	}
}

