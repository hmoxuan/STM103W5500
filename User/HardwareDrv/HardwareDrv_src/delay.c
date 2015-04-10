/**
 * delay.c file for STM32F103.
 * Describtion: Used to produce ms us delay
 * Author: qinfei 2015.03.26
 * Version: GatewayV1.0
 * Support:qf.200806@163.com
 */

#include "delay.h"

/*******************************************************************************
 *V1.2修改说明
 *修正了中断中调用出现死循环的错误
 *防止延时不准确,采用do while结构!
********************************************************************************/	 
static u8  fac_us=0;    /* us延时倍乘数 */
static u16 fac_ms=0;    /* ms延时倍乘数 */


/*******************************************************************************
 * 初始化延迟函数
 * SYSTICK的时钟固定为HCLK时钟的1/8
 * SYSCLK:系统时钟
********************************************************************************/
void delay_init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);/*选择外部时钟HCLK/8*/
	fac_us=SystemCoreClock/8000000;//为系统时钟的1/8  		    
	fac_ms=(u16)fac_us*1000;//非ucos下,代表每个ms需要的systick时钟数
}


/*******************************************************************************
 * 延时nms
 * 注意nms的范围
 * SysTick->LOAD为24位寄存器,所以,最大延时为:
 * nms<=0xffffff*8*1000/SYSCLK
 * SYSCLK单位为Hz,nms单位为ms
 * 对72M条件下,nms<=1864 
 * 
********************************************************************************/
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;/*时间加载(SysTick->LOAD为24bit)*/	
	SysTick->VAL =0x00;           /*清空计数器*/	
	SysTick->CTRL=0x01 ;          /*开始倒数*/	
        
	do
	{
		temp=SysTick->CTRL;
	}
	while((temp&0x01)&&(!(temp&(1<<16))));/*等待时间到达*/
        
	SysTick->CTRL=0x00;     /*关闭计数器*/	
	SysTick->VAL =0X00;     /*清空计数器*/		  	    
}   

/*******************************************************************************
 * 延时nus
 * nus为要延时的us数.
********************************************************************************/	    								   
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; /*时间加载*/	  		 
	SysTick->VAL=0x00;        /*清空计数器*/
	SysTick->CTRL=0x01;       /*开始倒数*/ 	
        
	do
	{
		temp=SysTick->CTRL;
	}
	while((temp&0x01)&&(!(temp&(1<<16))));/*等待时间到达*/
        
	SysTick->CTRL=0x00;     /*关闭计数器*/
	SysTick->VAL =0X00;     /*清空计数器*/	 
}