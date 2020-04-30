/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "dht11.h"
#include "oled.h"
#include "infrared.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct
{
	char date_str[11];
	char time_str[9];
	char week_str[4];
	char hum_str[6];
	char tmp_str[6];
}DISPLAY_DATA;


/**********定义轮询任务结构�?**********/
typedef struct
{
     uint8_t Valid;									// 任务是否挂起标志位：1=挂起
		 uint8_t Run;                  // 程序运行标记�?0-不运行，1运行
    uint16_t Timer;                // 计时�?
    uint16_t ItvTime;              // 任务运行间隔时间
    void (*TaskHook)(DISPLAY_DATA* data); // 要运行的任务函数
}TASK_COMPONENTS;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TASK_NUM 		4			// 定义任务数量
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
RTC_DateTypeDef sDate ; // 日期
RTC_TimeTypeDef sTime ; // 时间
RTC_DateTypeDef setDate ; // 待设置的日期
RTC_TimeTypeDef setTime ; // 待设置时间
DHT11_Data_TypeDef dth11_data = {0};

// 显示结构体，初始化
DISPLAY_DATA display_data = {
	"2020-01-01",
	"12:00:00",
	"***",
	"00.00",
	"00.00"
}; 

char *week[7] = {"SUN","MON","TUE","WEN","THU","FRI","SAT"} ;

uint8_t ir_code=0; // 红外解码的值
uint8_t inverse_colum_idx = 0xFF; // 反显的索引
uint8_t inverse_page_idx = 0xFF; // 反显的索引

extern uint8_t OK;										// 红外引导码有效标志位：1=有效
//extern uint8_t ReadyFlag;						// 红外接收完成标志位：1=完成
extern uint16_t TimeOut; 						// 超时时间计数
extern uint16_t KeyCount;						// 按键连按次数

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void Task_Process(void); // 任务处理
void Task_Remark(void);  // 任务标记
void Task_Get_Date(DISPLAY_DATA* data);  // 更新时间任务 
void Task_Get_Time(DISPLAY_DATA* data);  // 更新时间任务  
void Task_Get_Hum(DISPLAY_DATA* data);   // 更新温湿度任务  
void Task_Display(DISPLAY_DATA* data);   // 显示任务 

// 定义任务数组
static TASK_COMPONENTS TaskComps[] = {
	{1,1,500,500,Task_Get_Time}, // 时间更新任务：刚进入就运行一次，等待1s执行�?次，后面就每�?1s执行�?�?
	{1,1,500,10000,Task_Get_Date}, // 日期更新任务：刚进入就运行一次，等待1s执行丿次，后面就每隿1s执行丿欿
	{1,1,1000,1000,Task_Display}, // 显示任务
	{1,0,1000,3000,Task_Get_Hum}  // 温湿度显示任务
};

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// 任务处理函数
void Task_Process(void)
{
	uint8_t i;
	for(i=0;i<TASK_NUM;i++)
	{
		if(TaskComps[i].Valid) // 如果任务没有被挂起
		{
			if(TaskComps[i].Run) // 如果有任务标记可运行
			{
				TaskComps[i].TaskHook(&display_data);  // 运行任务
				TaskComps[i].Run = 0 ; 		// 任务运行标记复位
			}
		}
	}
}

// 任务轮询函数，放在定时器中断中执�?
// 原理：每�?1ms进入�?次中断，然后将任务的计时器Timer减一，直到减为零�?
// 然后将Timer重新赋�?�为定时间隔ItvTime，同时将任务运行标志置为1，表示该任务可以运行�?
// 在main函数中，Task_Process会不断的判断是否有可执行的任务，然后去执�?
void Task_Remark(void)
{
	uint8_t i ;
  for(i = 0 ; i < TASK_NUM ; i++)
  {
    if(TaskComps[i].Valid)   // 如果任务没有被挂起
		{
				if(TaskComps[i].Timer)
				{
					TaskComps[i].Timer -- ;           // 不为零就每次递减1
					if(TaskComps[i].Timer == 0)       // 减到零就重置计数
					{
						TaskComps[i].Timer = TaskComps[i].ItvTime ; // 重置计数时长 
						TaskComps[i].Run = 0x01 ;    // 任务可以运行标志
					}
				}
		}
  }
}

// 更新日期任务 
void Task_Get_Date(DISPLAY_DATA* data)
{
	HAL_RTC_GetDate(&hrtc,&sDate,RTC_FORMAT_BIN);

	data->date_str[0]='2';
	data->date_str[1]='0';
	data->date_str[2]=sDate.Year/10+'0';
	data->date_str[3]=sDate.Year%10+'0';
	data->date_str[4]='-';
	data->date_str[5]=sDate.Month/10+'0';
	data->date_str[6]=sDate.Month%10+'0';
	data->date_str[7]='-';
	data->date_str[8]=sDate.Date/10+'0';
	data->date_str[9]=sDate.Date%10+'0';

	char* d = week[sDate.WeekDay%7];
	data->week_str[0]=d[0];
	data->week_str[1]=d[1];
	data->week_str[2]=d[2];
}

// 更新时间任务 
void Task_Get_Time(DISPLAY_DATA* data)
{
	HAL_RTC_GetTime(&hrtc,&sTime,RTC_FORMAT_BIN);
	
	data->time_str[0]=sTime.Hours/10+'0';
	data->time_str[1]=sTime.Hours%10+'0';
	data->time_str[2]=':';
	data->time_str[3]=sTime.Minutes/10+'0';
	data->time_str[4]=sTime.Minutes%10+'0';
	data->time_str[5]=':';
	data->time_str[6]=sTime.Seconds/10+'0';
	data->time_str[7]=sTime.Seconds%10+'0';
}

// 更新温湿度任务
void Task_Get_Hum(DISPLAY_DATA* data)
{
	uint8_t res = dht11_read_data(&dth11_data);
	if(res == SUCCESS)
	{
		data->tmp_str[0] = dth11_data.temp_high8bit/10+'0';
		data->tmp_str[1] = dth11_data.temp_high8bit%10+'0';
		data->tmp_str[2] = '.';
		data->tmp_str[3] = dth11_data.temp_low8bit/10+'0';
		data->tmp_str[4] = dth11_data.temp_low8bit%10+'0';

		data->hum_str[0] = dth11_data.humi_high8bit/10+'0' ;
		data->hum_str[1] = dth11_data.humi_high8bit%10+'0' ;
		data->hum_str[2] ='.';
		data->hum_str[3] = dth11_data.humi_low8bit/10+'0' ;
		data->hum_str[4] = dth11_data.humi_low8bit%10+'0' ;
	}
}	

// 显示任务
void Task_Display(DISPLAY_DATA* data)
{
	OLED_Show_String(0,0,data->date_str,BIG);
	OLED_Show_String(103,0,data->week_str,BIG);
	OLED_Show_String(0,2,data->time_str,BIG);
	OLED_Show_String(0,4,"TEM:",BIG) ;
	OLED_Show_String(32,4,data->tmp_str,BIG);
	OLED_Show_String(0,6,"HUM:",BIG) ;
	OLED_Show_String(32,6,data->hum_str,BIG);
}	

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_RTC_Init();
  MX_SPI1_Init();
  MX_TIM2_Init();
  MX_USART1_UART_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
	OLED_Init();
	OLED_Clear();
	Infrared_Init();
	HAL_TIM_Base_Start_IT(&htim1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		Task_Process();
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
int fputc(int ch , FILE *f)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch , 1 , 10);
    return ch;
}

/******************************************
** 定时器TIM1更新回调函数每1ms回调一次，
** 这个1ms作为任务轮询的间隔时间
********************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1) // 定时器TIM1更新中断
	{
		Task_Remark(); // 执行任务标记，定时器TIM1来做任务轮询的时钟
	}
	else if(htim->Instance == TIM2)  // 定时器TIM2更新中断
	{
		if(OK)
		{
			TimeOut = (TimeOut>20?0:TimeOut++);
			if(TimeOut==0) 
			{
				ir_code = IRCode_Process();
//				printf("code = %02X \r\n",ir_code);
				uint8_t code_num = 0xFF ;
				switch(ir_code)
				{
					case 0x68:
						inverse_colum_idx = 0;
						inverse_page_idx = 0;
						break;
					case 0xB0:
						inverse_colum_idx = 0xFF;
						inverse_page_idx = 0xFF;
						TaskComps[2].Valid = 1;
						break;					
					case 0x5A:
						inverse_colum_idx = (inverse_colum_idx > 120 ? 0 :inverse_colum_idx + 8);
						break;
					case 0x10:
						inverse_colum_idx = (inverse_colum_idx == 0 ? 120 :inverse_colum_idx - 8);
						break;
					case 0x4A:
						inverse_page_idx = (inverse_page_idx > 7? 0:inverse_page_idx + 2 );
						break;
					case 0x18:
						inverse_page_idx = (inverse_page_idx == 0 ? 6:inverse_page_idx - 2 );
						break;	
//				1(0xA2)  2(0x62)  3(0xE2)
//				4(0x22)  5(0x02)  6(0xC2)
//				7(0xE0)  8(0xA8)  9(0x90)
//				*(0x68)  0(0x98)  #(0xB0)
					case 0x98:
						code_num = 0;
						break;
					case 0xA2:
						code_num = 1;
						break;
					case 0x62:
						code_num = 2;
						break;	
					case 0xE2:
						code_num = 3;
						break;
					case 0x22:
						code_num = 4;
						break;
					case 0x02:
						code_num = 5;
						break;
					case 0xC2:
						code_num = 6;
						break;
					case 0xE0:
						code_num = 7;
						break;
					case 0xA8:
						code_num = 8;
						break;
					case 0x90:
						code_num = 9;
						break;
				}
				OLED_Set_Inverse(inverse_colum_idx,inverse_page_idx,0);
				if(code_num < 10 && inverse_page_idx == 0) 
				{
					TaskComps[2].Valid = 0;
					OLED_Show_Char(inverse_colum_idx,inverse_page_idx,code_num+'0',BIG);
				}	else if(code_num < 10 && inverse_page_idx == 2) 
				{
					TaskComps[2].Valid = 0;
					OLED_Show_Char(inverse_colum_idx,inverse_page_idx,code_num+'0',BIG);
				}				
			}
		}
	}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
