/**
 * sys.c file for STM32F103.
 * Describtion: The underlying hardware related setting function
 * Author: qinfei 2015.03.26
 * Version: GatewayV1.0
 * Support:qf.200806@163.com
 */

#include "sys.h"
 
void NVIC_Configuration(void)
{
  /* 设置NVIC中断分组2:2位抢占优先级，2位响应优先级 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}