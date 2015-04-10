/**
 * leds.c file for STM32F103.
 * Describtion: led control module
 * Author: qinfei 2015.03.26
 * Version: GatewayV1.0
 * Support:qf.200806@163.com
 */
#include "sys.h"
#include "leds.h" 	
#include "delay.h"

/* 初始化PA8和PD2为输出口.并使能这两个口的时钟 */		    
/* LED IO初始化 */
void LED_Init(void)
{
 
    GPIO_InitTypeDef  GPIO_InitStructure;
          
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);/* 使能PB端口时钟 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;		 /* LED0-->PB.5 端口配置 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	 /* 推挽输出 */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB,GPIO_Pin_5);			 /* PB.5输出高 */


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);/* 使能PD端口时钟 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    	 /* LED1-->PE.5 端口配置 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	 /* 推挽输出 */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(GPIOE, &GPIO_InitStructure);	
    GPIO_SetBits(GPIOE,GPIO_Pin_5); 			 /* PE.5 输出高 */
}

void LED_Module_Test(void)
{
    LED0=0;         /* 这里使用了位带操作，也可以使用 GPIO_ResetBits(GPIOA,GPIO_Pin_8); */
    LED1=1;	    /* 也可以使用 GPIO_SetBits(GPIOD,GPIO_Pin_2); */
    delay_ms(1000);	
    
    LED0=1;	    /* 也可以使用 GPIO_SetBits(GPIOA,GPIO_Pin_8); */	    
    LED1=0; 	    /* 也可以使用 GPIO_ResetBits(GPIOD,GPIO_Pin_2) ; */
    delay_ms(1000);	
}
 
