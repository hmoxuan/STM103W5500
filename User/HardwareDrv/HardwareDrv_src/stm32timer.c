/**
 * stm32timer.c file for STM32F103.
 * Describtion:  STM32 定时器
 * Author: qinfei 2015.04.02
 * Version: GatewayV1.0
 * Support:qf.200806@163.com
 */

#include "stm32timer.h"
#include "leds.h"
#include "usart.h"

#include "myportmacro.h"//数据类型宏定义


static uint32 SystemNowtime;//系统当前时间单位10ms
u32 uip_timer=0;//uip计时器，每10ms增加1.


/* Timer6 用于系统时----------------------------------------------------------*/
/* 定时器6中断服务程序 */ 
void TIM6_IRQHandler(void)
{ 
    /*检查指定的TIM中断发生与否:TIM 中断源 */
    if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
    {
      uip_timer++;//uip计时器增加1
      Timing_Increase();
    } 
    
    /*清除TIMx的中断待处理位:TIM 中断源*/
    TIM_ClearITPendingBit(TIM6, TIM_IT_Update);	    		  			    	    
}

/**
 * 基本定时器6中断初始化
 * 这里时钟选择为APB1的2倍，而APB1为36M
 * arr：自动重装值。
 * psc：时钟预分频数
 * 这里使用的是定时器3!
 * TIM6_Int_Init(1000,719); 100Khz计数频率，计数到1000为10ms 
 */
void TIM6_Int_Init(u16 arr,u16 psc)
{	
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);//时钟使能

    TIM_TimeBaseStructure.TIM_Period = arr;//设置在下一个更新事件装入活动的自动重装载寄存器周期的值,计数到5000为500ms
    TIM_TimeBaseStructure.TIM_Prescaler = psc;//设置用来作为TIMx时钟频率除数的预分频值,10Khz的计数频率  
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;//设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM向上计数模式
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
    TIM_ITConfig( TIM6,TIM_IT_Update|TIM_IT_Trigger,ENABLE);//使能定时器6更新触发中断

    TIM_Cmd(TIM6, ENABLE);//使能TIMx外设

    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;//TIM6中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//先占优先级0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;//从优先级3级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 								 
}

/*
 * 函数名：Timing_Increase
 * 描述  ：系统时间计数，
 * 输入  ：无
 * 输出  ：系统当前时间
 * 调用  ：在 SysTick 中断函数 SysTick_Handler()调用
 */  
void Timing_Increase(void)
{   
    /*范围：0-4294967295(4 Bytes)累加到溢出大约需要3.6小时*/
    SystemNowtime++;
}

/*
 * 函数名：GetSystemNowtime
 * 描述  ：获取系统当前时间
 * 输入  ：无
 * 输出  ：系统当前时间
 * 调用  ：在 SysTick 中断函数 SysTick_Handler()调用
 */  
uint32 GetSystemNowtime(void)
{  
    return SystemNowtime;//返回系统当前时间
}


/* Timer3：DNS所需要的定时器---------------------------------------------------*/
/**
  * @brief  初始化定时器中断
  * @param  None
  * @retval None
  */
void Timer_Interrupts_Config(void)
{
    NVIC_InitTypeDef  NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  初始化定时器 
  * @param  None
  * @retval None
  */
void Timer_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 

    TIM_DeInit(TIM3);//复位TIM3定时器
                    
    /* TIM3 configuration */
    TIM_TimeBaseStructure.TIM_Period = 200;// 100ms    
    TIM_TimeBaseStructure.TIM_Prescaler = 36000;// 分频36000      
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;// 时钟分频 
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//计数方向向上计数
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    /* Clear TIM3 update pending flag[清除TIM3溢出中断标志] */
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);

    /* Enable TIM3 Update interrupt [TIM3溢出中断允许]*/
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); 
    /* TIM3计数器清零*/
    TIM3->CNT=0;
    /* TIM3 enable counter [允许TIM3计数]*/
    TIM_Cmd(TIM3, DISABLE);  
    /*Config interrupts*/
    Timer_Interrupts_Config();
}

/**
  * @brief  清除定时器计数器寄存器计数值并启动定时器
  * @param  None
  * @retval None
  */
void Timer_Start(void)
{
    TIM3->CNT=0;//清除计数器寄存器的值，可以减小丢帧的情况
    /* Enable the TIM Counter */
    TIM_Cmd(TIM3, ENABLE); 
}

/**
  * @brief  停止定时器并清除定时器的计数值
  * @param  None
  * @retval None
  */
void Timer_Stop(void)
{ 
    /* Disable the TIM Counter */
    TIM_Cmd(TIM3, DISABLE);
}


