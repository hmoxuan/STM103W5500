/**
 * beep.c file for STM32F103.
 * Describtion: beep control module
 * Author: qinfei 2015.03.30
 * Version: GatewayV1.0
 * Support:qf.200806@163.com
 */
#include "beep.h"
#include "delay.h"
#include "leds.h" 

//初始化PB8为输出口.并使能这个口的时钟		    
//蜂鸣器初始化
void BEEP_Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能GPIOB端口时钟
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;//BEEP-->PB.8 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);//根据参数初始化GPIOB.8
 
 GPIO_ResetBits(GPIOB,GPIO_Pin_8);//输出0，关闭蜂鸣器输出
}

void BEEP_Module_Test(void)
{
    LED0=0;
    BEEP=0;		  
    delay_ms(300);//延时300ms
    LED0=1;	  
    BEEP=1;  
    delay_ms(300);//延时300ms
}