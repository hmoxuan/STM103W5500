/**
 * wdg.c file for STM32F103.
 * Describtion: wdg window module
 * Author: qinfei 2015.03.27
 * Version: GatewayV1.0
 * Support:qf.200806@163.com
 */
#include "wdg.h"
#include "leds.h"

/*保存WWDG计数器的设置值,默认为最大.*/
u8 WWDG_CNT = 0x7f;

/**
 * 初始化窗口看门狗
 * tr: T[6:0] 用于存储计数器的值
 * wr: W[6:0] 用于存窗口值
 * fprer: 窗口看门狗的实际设置，低2位有效
 * Fwwdg = PCLK1/4095/2^fprer
 */
void WWDG_Init(u8 tr,u8 wr,u32 fprer)
{ 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);//WWDG时钟使能
    WWDG_CNT=tr&WWDG_CNT;/*初始化WWDG_CNT. */

    WWDG_SetPrescaler(fprer);//设置IWDG预分频值
    WWDG_SetWindowValue(wr);//设置窗口值95

    WWDG_Enable(WWDG_CNT);//使能看门狗,设置counter.:127                 
    WWDG_ClearFlag();// Clear EWI flag 

    WWDG_NVIC_Init();//初始化窗口看门狗 NVIC
    WWDG_EnableIT();//开启窗口看门狗中断
} 

/*初始化窗口看门狗 NVIC*/
void WWDG_NVIC_Init()
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;//WWDG中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//抢占2，子优先级3，组2	
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;//抢占2，子优先级3，组2	
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE; 
    NVIC_Init(&NVIC_InitStructure);//NVIC初始化
}

/*窗口看门狗中断服务程序*/
void WWDG_IRQHandler(void)
{
    WWDG_SetCounter(WWDG_CNT);//窗口看门狗喂狗
    WWDG_ClearFlag();
    //LED1 = !LED1;
}















