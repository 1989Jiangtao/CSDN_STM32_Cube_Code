#ifndef __DHT11_H__
#define __DHT11_H__

#include "main.h"
#include "gpio.h"

/************************ DHT11 数据类型定义******************************/
typedef struct
{
	uint8_t  humi_high8bit;                //原始数据：湿度高8位
	uint8_t  humi_low8bit;                 //原始数据：湿度低8位
	uint8_t  temp_high8bit;                //原始数据：温度高8位
	uint8_t  temp_low8bit;                 //原始数据：温度高8位
	uint8_t  check_sum;                     //校验和
  float    humidity;        //实际湿度
  float    temperature;     //实际温度  
} DHT11_Data_TypeDef;

#define DHT11_Clr() HAL_GPIO_WritePin(DHT_IN_GPIO_Port, DHT_IN_Pin, GPIO_PIN_RESET)
#define DHT11_Set() HAL_GPIO_WritePin(DHT_IN_GPIO_Port, DHT_IN_Pin, GPIO_PIN_SET)	 
#define DHT11_IN	HAL_GPIO_ReadPin(DHT_IN_GPIO_Port, DHT_IN_Pin)

void dht11_input(void);
void dht11_output(void);
void dht11_reset(void);
unsigned short dht11_read_bit(void);
unsigned short dht11_read_byte(void);
uint8_t dht11_read_data(DHT11_Data_TypeDef * DTH_Data);

#endif
