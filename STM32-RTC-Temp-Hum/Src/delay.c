#include "delay.h"

 /****************************
 ** 定时us
 ** nus: 需要定时的微秒数
 *****************************/
void delay_us(int32_t nus) 
 {
  int32_t temp; 
  SysTick->LOAD = nus*4 ; //32MHz
  SysTick->VAL=0X00;//清空计数
  SysTick->CTRL=0X01;//使能，减到零是无动作，采用外部时钟源 
  do 
  { 
    temp=SysTick->CTRL;//读取当前倒计时
  }
  while((temp&0x01)&&(!(temp&(1<<16))));//等待时间到达 
     
  SysTick->CTRL=0x00; //关闭计数
  SysTick->VAL =0X00; //清空计数
 }

 /****************************
 ** 定时ms
 ** nms: 需要定时的毫秒数
 *****************************/
 void delay_ms(int32_t nms) 
 {
  int32_t temp; 
  SysTick->LOAD = (nms<<2)*1000 ; //32MHz
  SysTick->VAL=0X00;//清空计数
  SysTick->CTRL=0X01;//使能，减到零是无动作，采用外部时钟源 
  do 
  { 
    temp=SysTick->CTRL;//读取当前倒计时
  }
  while((temp&0x01)&&(!(temp&(1<<16))));//等待时间到达 
     
  SysTick->CTRL=0x00; //关闭计数
  SysTick->VAL =0X00; //清空计数
 }

