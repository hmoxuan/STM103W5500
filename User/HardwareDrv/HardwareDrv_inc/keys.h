/**
 * keys.h file for STM32F103.
 * Describtion:  key control module
 * Author: qinfei 2015.03.26
 * Version: GatewayV1.0
 * Support:qf.200806@163.com
 */
#ifndef __KEYS_H__
#define __KEYS_H__

#include "stm32f10x.h"
#include "sys.h"

/* KEY端口定义 */
#define KEY0  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)//读取按键0
#define KEY1  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)//读取按键1
#define KEY2  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)//读取按键2 
#define KEY3  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//读取按键3(WK_UP) 

#define KEY_UP 		4
#define KEY_LEFT	3
#define KEY_DOWN	2
#define KEY_RIGHT	1
	 
void KEY_Init(void);       /* 按键IO口初始化 */
void KEY_Module_Test(void);/* LEDS模块测试 */
		 				    
#endif  /*__KEYS_H__*/