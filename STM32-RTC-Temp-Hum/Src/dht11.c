#include "dht11.h"
#include "delay.h"
 
// 配置为输入
void dht11_input(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	/*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = DHT_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DHT_IN_GPIO_Port, &GPIO_InitStruct);
}

// 配置为输出
void dht11_output(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_GPIOA_CLK_ENABLE();
	
	/*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = DHT_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(DHT_IN_GPIO_Port, &GPIO_InitStruct);
}

// 复位
void dht11_reset(void)
{
	dht11_output();
	DHT11_Clr();
	delay_ms(20);
	DHT11_Set();
	delay_us(20);
	dht11_input();
}

// 读取一个bit数据
unsigned short dht11_read_bit(void)
{
	while(DHT11_IN == GPIO_PIN_RESET);
	delay_us(40);
	if(DHT11_IN == GPIO_PIN_SET)
	{
		while(DHT11_IN == GPIO_PIN_SET);
		return 1;
	}else {
		return 0;
	}
}

// 读取一个字节的数据
unsigned short dht11_read_byte(void)
{
	unsigned char i;
	unsigned short data = 0;
	for(i=0;i<8;i++)
	{
		data <<= 1;
		data |= dht11_read_bit();
	}
	return data;
}

// 读取数据
uint8_t dht11_read_data(DHT11_Data_TypeDef* DTH_Data)
{
	uint8_t checksum;
	uint16_t data_temp;
	
	dht11_reset(); // 复位
	if(DHT11_IN == GPIO_PIN_RESET)
	{
		// 监测到DTH11响应
		while(DHT11_IN == GPIO_PIN_RESET);
		while(DHT11_IN == GPIO_PIN_SET);
		// 开始读取有效数据，一共五个字节
		DTH_Data->humi_high8bit = dht11_read_byte();
		DTH_Data->humi_low8bit = dht11_read_byte();
		DTH_Data->temp_high8bit = dht11_read_byte();
		DTH_Data->temp_low8bit = dht11_read_byte();
		DTH_Data->check_sum = dht11_read_byte();
		while(DHT11_IN == GPIO_PIN_RESET);
		// 读取完毕，设置为输出
		dht11_output();
		DHT11_Set();
		/* 对数据进行处理 */
    data_temp=DTH_Data->humi_high8bit*100+DTH_Data->humi_low8bit;
    DTH_Data->humidity =(float)data_temp/100;
    
    data_temp=DTH_Data->temp_high8bit*100+DTH_Data->temp_low8bit;
    DTH_Data->temperature=(float)data_temp/100;   
		
		// 检验数据
		checksum = DTH_Data->humi_high8bit + DTH_Data->humi_low8bit + DTH_Data->temp_high8bit + DTH_Data->temp_low8bit;
		if(checksum != DTH_Data->check_sum){
			return ERROR;
		}else{
			return SUCCESS;
		}
	}else {
		return ERROR;
	}
}

