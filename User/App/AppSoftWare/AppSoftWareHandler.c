/**
 * AppSoftWareHander.c APP任务函数
 * Describtion:
 * Author: qinfei 2015.04.02
 * Version: GatewayV1.0
 * Support:qf.200806@163.com
 */
#include "stm32f10x.h"
#include "AppSoftWareDriver.h"  //系统执行驱动文件
#include "myportmacro.h"        //数据类型宏定义
#include "AppDebugFunc.h"       //串口调试打印信息
#include "leds.h"	        //led控制
#include "usart.h"              //uart1控制
#include "stm32timer.h"         //获取当前系统时间
#include "mystring.h"           //
#include "AppDebugFunc.h"       //

#include "AppSoftWareHandler.h"
#include "NetHandler.h"         //W5500网络协议初始化


/*循环事件执行标志*/
uint16  LoopEvents=0x0000;

/*循环事件执行标志暂存*/
uint16  LoopEventstmp=0x0000;//在中断处理函数中会被修改

/*定时循环事件执行标志*/
static uint16  LoopTimeEvents=0x0000;

/*定时循环事件执行标志暂存*/
static uint16  LoopTimeEventstmp=0x0000;


//*********************************函数实体区**********************************
/*
 * 函数名：系统软件初始化
 * 描述  ：配置LED用到的I/O口
 * 输入  ：无
 * 输出  ：无
 */
void SoftWareInit(void)
{	
    APP_DebugSerialStrOutput("\r\nHandler SoftWareInit function ! \r\n");
    
    /*配置W5500网络*/
    NetworkInitHandler();
    
    /*设置轮询接收网络数据包事件标志 */  
    //LoopEventSetBit(APP_EVENT_GATEWAY_NETINPUT);
    
    /*接收网络协议数据事件*/
    LoopEventSetBit(APP_EVENT_GATEWAY_RECVNET);
}

/**
  * @fun    LoopEventSetBit
  * @brief  循环事件标志位置位
  * @param  None
  * @retval None
  */
void LoopEventSetBit(EventType event)
{
    LoopEventstmp|=event;
}

/**
  * @fun    LoopTimeEventSetBit
  * @brief  定时循环事件标志位置位
  * @param  None
  * @retval None
  */
void LoopTimeEventSetBit(EventType event)
{
    LoopTimeEventstmp|=event;
}


/**
  * @fun    SoftWareEventInLoop
  * @brief  事件在环执行
  * @param  None
  * @retval None
  */
void SoftWareEventInLoop(void)
{
    uint8  hdlstate=0;  /*处理状态*/
    
//*******************网络NET->GATEWAY->终端*************************************/
    //----------------------------------/*软件在环--接收网络协议数据事件*/
    if(LoopEvents & APP_EVENT_GATEWAY_RECVNET){
        APP_DebugSerialStrOutput("\r\nAPP_EVENT_GATEWAY_RECVNET!\r\n");
        
        LoopEventSetBit(APP_EVENT_GATEWAY_NETTOEDV);/*置位网络终端协议转换事件*/
        hdlstate=1;
        if(hdlstate){
            LoopEvents ^= APP_EVENT_GATEWAY_RECVNET;
            hdlstate ^= hdlstate;
        }
    }
    
    //----------------------------------/*软件在环--网络终端协议转换事件*/
    if(LoopEvents & APP_EVENT_GATEWAY_NETTOEDV){
        APP_DebugSerialStrOutput("\r\nAPP_EVENT_GATEWAY_NETTOEDV!\r\n");
        
        LoopEventSetBit(APP_EVENT_GATEWAY_SENDEDV);/*置位串口传送事件*/
        hdlstate=1;
        if(hdlstate){
            LoopEvents ^= APP_EVENT_GATEWAY_NETTOEDV;
            hdlstate ^= hdlstate;
        }
    }
    
    //----------------------------------/*软件在环--发送终端协议数据事件*/
    if(LoopEvents & APP_EVENT_GATEWAY_SENDEDV){
        APP_DebugSerialStrOutput("\r\nAPP_EVENT_GATEWAY_SENDEDV!\r\n");
        
        //LoopEventSetBit();/*不会引起其他事件*/
        hdlstate=1;
        if(hdlstate){
            LoopEvents ^= APP_EVENT_GATEWAY_SENDEDV;
            hdlstate ^= hdlstate;
        } 
    }

//*******************终端->GATEWAY->网络NET*************************************/    
    //----------------------------------/*软件在环--接收终端协议数据事件*/
    if(LoopEvents & APP_EVENT_GATEWAY_RECVEDV){
        APP_DebugSerialStrOutput("\r\nAPP_EVENT_GATEWAY_RECVEDV!\r\n");
        
        //APP_SerialRecvHandler();/*串口接收处理事件*/

        LoopEventSetBit(APP_EVENT_GATEWAY_EDVTONET);/*置位终端网络协议转换事件*/
        hdlstate=1;
        if(hdlstate){
            LoopEvents ^= APP_EVENT_GATEWAY_RECVEDV;
            hdlstate ^= hdlstate;
        }
    }
    
    //----------------------------------/*软件在环--终端网络协议转换事件*/
    if(LoopEvents & APP_EVENT_GATEWAY_EDVTONET){
        APP_DebugSerialStrOutput("\r\nAPP_EVENT_GATEWAY_EDVTONET!\r\n");
         
        LoopEventSetBit(APP_EVENT_GATEWAY_SENDNET);/*置位终端网络协议转换事件*/
        hdlstate=1;
        if(hdlstate){
            LoopEvents ^= APP_EVENT_GATEWAY_EDVTONET;
            hdlstate ^= hdlstate;
        }
    }
    
    //----------------------------------/*软件在环--发送网络协议数据事件*/
    if(LoopEvents & APP_EVENT_GATEWAY_SENDNET){
        APP_DebugSerialStrOutput("\r\nAPP_EVENT_GATEWAY_SENDNET!\r\n");
        
        //LoopEventSetBit();/*不会引起其他事件*/
        hdlstate=1;
        if(hdlstate){
            LoopEvents ^= APP_EVENT_GATEWAY_SENDNET;
            hdlstate ^= hdlstate;
        }
    }
    
//*******************网关定时**************************************************/     
    //----------------------------------/*软件在环网关定时事件*/
    if(LoopEvents & APP_EVENT_GATEWAY_TIMEING){
        APP_DebugSerialStrOutput("\r\n Gateway timed events in SoftWareEventInLoop!\r\n");
        APP_DebugSerialStrOutput("\r\nAPP_EVENT_GATEWAY_TIMEING!\r\n");
        
        APP_DebugSerialStrOutput("\r\n Handler SoftTimeingLoopEvent Function!\r\n");
        hdlstate=SoftTimeingLoopEvent();/*定时循环事件执行函数*/
        if(hdlstate){
            LoopEvents ^= APP_EVENT_GATEWAY_TIMEING;
            hdlstate ^= hdlstate;
        }
    }
    
    
//*****************************************************************************/      
    LoopEvents |= LoopEventstmp;
    LoopEventstmp ^= LoopEventstmp;
    
//*****************************************************************************/         
    //LwIP_Periodic_Handle();//轮询是否接收到数据
}

/**
  * @fun    SoftWareTimeingEventInLoop
  * @brief  定时事件在环执行
  * @param  None
  * @retval None
  */
void SoftWareTimeingEventInLoop(void)
{
    static uint32 HeartBeatLt=0; /*定义心跳事件上一次状态的时间 */
 
    static uint8  HeartBeatLtNowtimeStr[10];//打印心跳当前时间
    uint32  newTime=0;//范围：0-4294967295(4 Bytes)
    newTime=GetSystemNowtime();/*获得当前运行时间*/  
    
    //------------------------------------------------------------------------
    //********网关心跳包事件***************************************************
    if(TimeDifference(newTime,HeartBeatLt)>=HEART_BEAT_TIME){
        HeartBeatLt=newTime;
        {//调试输出：当前心跳处于的时间。
          LongIntToStr(HeartBeatLt,HeartBeatLtNowtimeStr);//长整形数转成字符串
          APP_DebugSerialStrOutput("\r\nHeartBeatLt Nowtime=");   
          APP_DebugSerialStrOutput(HeartBeatLtNowtimeStr);//输出当前时间
          APP_DebugSerialStrOutput("\r\n");//回车换行     
        }
        APP_DebugSerialStrOutput("\r\n Gateway heartbeat time arrived !\r\n");
       
        LoopEventSetBit(APP_EVENT_GATEWAY_TIMEING); /*置位软件在环网关定时事件*/
        APP_DebugSerialStrOutput("\r\n Set Gateway timed events into SoftWareEventInLoop!\r\n");
        
        LoopTimeEventSetBit(APP_T_EVENT_HEARTBEAT);/*置位心跳包事件*/
        APP_DebugSerialStrOutput("\r\n Set Gateway heartbeat timed events into SoftTimeingLoopEvent!\r\n");
    }
    
    LoopTimeEvents|=LoopTimeEventstmp;
    LoopTimeEventstmp^=LoopTimeEventstmp;
}


/**
  * @fun    SoftTimeingLoopEvent
  * @brief  定时循环事件执行函数
  * @param  None
  * @retval state of handle event
  */
uint8 SoftTimeingLoopEvent(void)
{
    uint8 state=0;
    uint8 hdlstate=0;
    
    //------------------------------------------------------------------------
    //********网关心跳包事件***************************************************
    if(LoopTimeEvents & APP_T_EVENT_HEARTBEAT){
        APP_DebugSerialStrOutput("\r\nAPP_T_EVENT_HEARTBEAT!\r\n");
        HeartBeatHandler();
      
        hdlstate=1;
        if(hdlstate){
            LoopTimeEvents ^= APP_T_EVENT_HEARTBEAT;
            hdlstate ^= hdlstate;
        }
    }
    
   
    if(!LoopTimeEvents) state=1;
    return state;
}


/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
