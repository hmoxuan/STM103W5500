/**
 * exit.c file for STM32F103.
 * Describtion: exit interrupt control module
 * Author: qinfei 2015.03.27
 * Version: GatewayV1.0
 * Support:qf.200806@163.com
 */

#ifndef __EXTI_H__
#define __EXTI_H__
		   
#include "sys.h"

/*外部中断IO初始化*/
void EXTIX_Init(void);//IO初始化

/*外部中断模块状态跟踪*/
void EXTI_Module_Test(void);

#endif  /*__EXTI_H__*/