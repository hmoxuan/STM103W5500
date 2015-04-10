/**
 * usart.c file for STM32F103.
 * Describtion: usart control module
 * Author: qinfei 2015.03.26
 * Version: GatewayV1.0
 * Support:qf.200806@163.com
 */
#include "sys.h"
#include "delay.h"
#include "leds.h"
#include "usart.h"

/* usart1 接收缓冲最大63个字节,末字节为换行符 */
u8 USART_RX_BUF[64];

/**
 * 接收状态
 * bit7，接收完成标志
 * bit6，接收到0x0d
 * bit5~0，接收到的有效字节数目
 */
u8 USART_RX_STA=0;/* 接收状态标记 */

/*
 * 函数名：fputc
 * 描述  ：重定向c库函数printf到USART1
 * 输入  ：无
 * 输出  ：无
 * 调用  ：由printf调用
 */
int fputc(int ch, FILE *f)
{
	/* 将printf内容发往串口 */
	USART_SendData(USART1, (unsigned char) ch);
	while (!(USART1->SR & USART_FLAG_TXE));
	
	return (ch);
}

void uart1_init(u32 bound)
{
    /* GPIO端口设置 */
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
     
    /* config USART1 clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
    
    /* USART1_TX  PA.9 */
    /* Configure USART1 Tx (PA.09) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* USART1_RX  PA.10 */
    /* Configure USART1 Rx (PA.10) as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

    /* Usart1 NVIC 配置 */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;/* IRQ通道使能 */
    NVIC_Init(&NVIC_InitStructure);/* 根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART1 */

    /* USART 初始化设置 */
    USART_InitStructure.USART_BaudRate = bound;/* 一般设置为9600; */
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    
    
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);/* 开启中断 */

    USART_Cmd(USART1, ENABLE);/* 使能串口 */ 
}

/*串口1中断服务程序 */
void USART1_IRQHandler(void)                	
{
    u8 Res;
    
    /* 接收中断(接收到的数据必须是0x0d 0x0a结尾) */
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        Res=USART_ReceiveData(USART1);/* (USART1->DR);读取接收到的数据 */
        
        if((USART_RX_STA&0x80)==0)/* 接收未完成 */
        {
            /*0X0D-0X0A表示回车键*/
            if(USART_RX_STA&0x40)/* 接收到了0x0d */
            {
                if(Res!=0x0a)
                  USART_RX_STA=0;/* 接收错误,重新开始 */
                else 
                  USART_RX_STA|=0x80;/* 接收完成了 */
            }
            else /* 还没收到0X0D */
            {	
                if(Res==0x0d)
                {
                  USART_RX_STA|=0x40;
                }
                else
                {
                    USART_RX_BUF[USART_RX_STA&0X3F]=Res ;
                    USART_RX_STA++;
                    if(USART_RX_STA>63)
                      USART_RX_STA=0;/*接收数据错误,重新开始接收*/	  
                }		 
            }
        }   		 
     } 
} 


/*串口1模块测试程序 */
void USART1_Module_Test(void)
{
    u8 t;
    u8 len;	
    static u16 times=0;/*定义为static，主函数下次调用保证times还是上次的值*/
    
    /*usart1接收数据完成*/
    if(USART_RX_STA&0x80)/*接收状态标记*/
    {					   
        len=USART_RX_STA&0x3f;/*得到此次接收到的数据长度*/
        printf("\n您发送的消息为:\n");
        for(t=0;t<len;t++)
        {
            USART1->DR=USART_RX_BUF[t];
            while((USART1->SR&0X40)==0);/*等待发送结束*/
        }
        printf("\n\n");/*插入换行*/
        printf("\n请发送下条消息!\n");
        USART_RX_STA=0;/*接收状态标记清0*/
    }
    /*usart1接收数据未完成*/
    else
    {
        times++;
        if(times%5000==0)
        {
            printf("\nMiniSTM32开发板 串口实验\n");
            printf("正点原子@ALIENTEK\n\n\n");
        }
        if(times%200==0)
          printf("请输入数据,以回车键结束\n");  
        if(times%30==0)
          LED0=!LED0;/* 闪烁LED,提示系统正在运行. */
        delay_ms(10);   
    }
}