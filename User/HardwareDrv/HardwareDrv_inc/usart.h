/**
 * usart.h file for STM32F103.
 * Describtion: usart control module
 * Author: qinfei 2015.03.26
 * Version: GatewayV1.0
 * Support:qf.200806@163.com
 */
#ifndef __USART_H__
#define __USART_H__

#include "stdio.h"

/******************************************************************************
 * V1.3修改说明 
 * 支持适应不同频率下的串口波特率设置.
 * 加入了对printf的支持,增加了串口接收命令功能.
 * 修正了printf第一个字符丢失的bug.
 ******************************************************************************/ 
extern u8 USART_RX_BUF[64];/* 接收缓冲,最大63个字节.末字节为换行符 */ 
extern u8 USART_RX_STA;    /* 接收状态标记 */	

void uart1_init(u32 bound);

/*串口1模块测试程序 */
void USART1_Module_Test(void);
		 				    
#endif  /*__USART_H__*/