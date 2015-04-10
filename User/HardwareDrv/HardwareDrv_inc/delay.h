/**
 * delay.h file for STM32F103.
 * Describtion: Used to produce ms us delay
 * Author: qinfei 2015.03.26
 * Version: GatewayV1.0
 * Support:qf.200806@163.com
 */
#ifndef __DELAY_H__
#define __DELAY_H__
		   
#include "stm32f10x.h"

/********************************************************************************	 
 *Mini STM32开发板
 *使用SysTick的普通计数模式对延迟进行管理
 *包括delay_us,delay_ms
 ********************************************************************************/

/********************************************************************************
V1.2修改说明
修正了中断中调用出现死循环的错误
防止延时不准确,采用do while结构!
 ********************************************************************************/

void delay_init(void);
void delay_ms(u16 nms);
void delay_us(u32 nus);

#endif  /*__DELAY_H__*/