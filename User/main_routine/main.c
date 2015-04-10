/**
 * main.c file for STM32F103.
 * Describtion: main routine
 * Author: qinfei 2015.03.26
 * Version: GatewayV1.0
 * Support:qf.200806@163.com
 */
#include "stm32f10x.h"
#include "delay.h"      //延时函数
#include "sys.h"        //STM32 IO 口宏定义及中断分组
#include "leds.h"       //LED
#include "beep.h"       //蜂鸣器
#include "keys.h"       //按键
#include "usart.h"      //串口
#include "exti.h"       //外部中断
#include "wdg.h"        //窗口看门狗
#include "stm32timer.h" //系统当前时间
#include "spi.h"        //W5500的SPI驱动

#include "AppDebugFunc.h"       //串口调试打印
#include "AppSoftWareHandler.h"	//APP任务函数
#include "NetHandler.h"	        //APP任务函数



/* 底层硬件驱动初始化 */
static void Hardware_Init(void);

/* 底层硬件驱动模块测试 */
//static void Hardware_module_test(void);
	   
/* 主函数 */
int main(void)
{
//    char value[16]={0};
//    int32_t t=0;
  
    /* 底层硬件驱动初始化 */
    Hardware_Init();
    
    /*打印调试信息*/
    APP_DebugSerialStrOutput("\r\nWelcome To Gateway App ! \r\n");
    
    /*初始化应用软件*/
    SoftWareInit();
    
    while(1)
    {
      /* 底层硬件驱动模块测试 */
      //Hardware_module_test();
      
      /*动态分配IP地址*/
      DhcpRunInLoop();
      
      /*事件在环执行*/
      SoftWareEventInLoop();
        
      /*定时事件循环执行*/
      SoftWareTimeingEventInLoop();
      
     
//      //yeelink_get("19610","34409",value);//×￠òaDèòaDT??3é×??oyeelinkéè±?μ?ID--LED1
//      yeelink_get("19657","34484",value);//×￠òaDèòaDT??3é×??oyeelinkéè±?μ?ID--LED2
//      printf("%s\n\r",value);	//char value[16]={0};
//      printf("\n\r");
//      for(t=0;t<11;t++){
//        delay_ms(1000);
//      } 
    }
}


/* 底层硬件驱动初始化 */
static void Hardware_Init(void)
{
    SystemInit(); 	    /* 系统时钟初始化为72M: SYSCLK_FREQ_72MHz */
    delay_init();           /* 延时函数初始化 */	
    TIM6_Int_Init(1000,719);/* 100Khz计数频率，计数到1000为10ms */
    NVIC_Configuration();   /* 设置NVIC中断分组2:2位抢占优先级，2位响应优先级 */
    LED_Init();		    /* LED端口初始化 */
    BEEP_Init();            /* 初始化蜂鸣器端口 */
    KEY_Init();             /* 按键端口初始化 */
    uart1_init(9600);       /* 串口1初始化：波特率9600 */
    //EXTIX_Init();         /* 外部中断初始化:不需要时一定要注释掉 */
    SPI_Configuration();    /* 配置W5500的SPI1驱动的函数 */
    Timer_Config();         /*初始化DNS所需要的定时器*/
    WWDG_Init(0X7F,0X5F,WWDG_Prescaler_8);/*计数器值为7f,窗口寄存器为5f,分频数为8*/  
    
    /*底层硬件驱动初始化完成*/
    APP_DebugSerialStrOutput("\r\nHardware Init sucessfully ! \r\n");

    printf("\x0c");printf("\x0c");//超级终端清屏
    printf("\033[1;40;32m");//设置超级终端背景为黑色，字符为绿色
    printf("\r\n*******************************************************************************");
    printf("\r\n************************ Copyright 2015-2050, hatress *************************");
    printf("\r\n*************************** http://www.embed-net.com **************************");
    printf("\r\n***************************** All Rights Reserved *****************************");
    printf("\r\n*******************************************************************************");
    printf("\r\n");

}

///* 底层硬件驱动模块测试 */
//static void Hardware_module_test(void)
//{
//    /* 1.LEDS模块测试 */ 
//    //LED_Module_Test();
//  
//    /* 2.BEEP模块测试 */ 
//    //BEEP_Module_Test();
//    
//    /* 3.按键模块测试 */
//    //KEY_Module_Test();
//
//    /* 4.串口1模块测试 */
//    //USART1_Module_Test();
//  
//    /* 5.外部中断模块测试 */
//    //EXTI_Module_Test();
//}

