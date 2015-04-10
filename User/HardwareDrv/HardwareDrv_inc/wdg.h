/**
 * wdg.h file for STM32F103.
 * Describtion: wdg window module
 * Author: qinfei 2015.03.27
 * Version: GatewayV1.0
 * Support:qf.200806@163.com
 */
#ifndef __WDG_H__
#define __WDG_H__

#include "sys.h"

void WWDG_Init(u8 tr,u8 wr,u32 fprer);//≥ı ºªØWWDG
void WWDG_NVIC_Init(void);

#endif  /*__WDG_H__*/
