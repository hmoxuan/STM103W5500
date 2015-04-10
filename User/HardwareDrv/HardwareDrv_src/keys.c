/**
 * keys.c file for STM32F103.
 * Describtion: key control module
 * Author: qinfei 2015.03.26
 * Version: GatewayV1.0
 * Support:qf.200806@163.com
 */
#include "sys.h"
#include "leds.h" 
#include "beep.h" 
#include "delay.h"
#include "keys.h" 

/* 按键扫描函数 */	
static u8 KEY_Scan(u8 mode);

/* IO初始化 */
void KEY_Init(void) 
{ 
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);//使能PORTA,PORTE时钟
        
    /*初始化KEY0-->GPIOE.2,KEY1-->GPIOE.3,KEY2-->GPIOE.4上拉输入 */

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;//PE2~4
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
    GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE2,3,4

    /* 初始化 WK_UP-->GPIOA.0 下拉输入 */
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0设置成输入，默认下拉	  
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.0
}

/**
 * 按键处理函数
 * 返回按键值
 * mode:0,不支持连续按;1,支持连续按;
 * 0，没有任何按键按下
 * 1，KEY0按下
 * 2，KEY1按下
 * 3，KEY2按下
 * 4，KEY3按下 WK_UP
 * 此函数有响应优先级,KEY0>KEY1>KEY2>KEY3!!
 */
static u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY0==0||KEY1==0||KEY2==0||KEY3==1))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY0==0)return KEY_RIGHT;
		else if(KEY1==0)return KEY_DOWN;
		else if(KEY2==0)return KEY_LEFT;
		else if(KEY3==1)return KEY_UP;
	}else if(KEY0==1&&KEY1==1&&KEY2==1&&KEY3==0)key_up=1; 	    
 	return 0;// 无按键按下
}

/* LEDS模块测试 */
void KEY_Module_Test(void)
{
  u8 t;
  LED0=0;//先点亮红灯
  
  while(1)
  {
      t=KEY_Scan(0);//得到键值
      if(t)
      {						   
          switch(t)
          {				 
              case KEY_UP://控制蜂鸣器
                      BEEP=!BEEP;
                      break;
              case KEY_LEFT://控制LED0翻转
                      LED0=!LED0;
                      break;
              case KEY_DOWN://控制LED1翻转	 
                      LED1=!LED1;
                      break;
              case KEY_RIGHT://同时控制LED0,LED1翻转 
                      LED0=!LED0;
                      LED1=!LED1;
                      break;  
              default:   break;              
          }
      }else delay_ms(10); 
  }	 
}