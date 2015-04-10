/**
 * leds.h file for STM32F103.
 * Describtion:  led control module
 * Author: qinfei 2015.03.26
 * Version: GatewayV1.0
 * Support:qf.200806@163.com
 */
#ifndef __LEDS_H__
#define __LEDS_H__

#include "stm32f10x.h"

/* LED端口定义 */
#define LED0 PBout(5)   /* PB5 */
#define LED1 PEout(5)   /* PE5 */	

void LED_Init(void);    /* 初始化 */
void LED_Module_Test(void);/* LEDS模块测试 */
		 				    
#endif  /*__LEDS_H__*/