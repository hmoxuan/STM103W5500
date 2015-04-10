/**
 * stm32timer.h file for STM32F103.
 * Describtion:  STM32 定时器
 * Author: qinfei 2015.04.02
 * Version: GatewayV1.0
 * Support:qf.200806@163.com
 */
#ifndef __STM32TIMER_H__
#define __STM32TIMER_H__

#include "sys.h"
#include "myportmacro.h"//数据类型宏定义

/*系统时钟*/
void TIM6_Int_Init(u16 arr,u16 psc);

/*系统时间计数*/
void Timing_Increase(void);

/*获取系统当前时间*/
uint32 GetSystemNowtime(void);

#endif/*__STM32TIMER_H__*/
