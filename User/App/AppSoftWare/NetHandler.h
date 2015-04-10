/**
 * NetHander.h 网络初始化函数
 * Describtion:
 * Author: qinfei 2015.04.09
 * Version: GatewayV1.0
 * Support:qf.200806@163.com
 */
#ifndef __NET_HANDLER_H__
#define	__NET_HANDLER_H__
#include "stm32f10x_it.h"
#include "wizchip_conf.h"

/* Private define ------------------------------------------------------------*/
#define SOCK_TCPS        	0
#define SOCK_DHCP		0
#define SOCK_DNS	        1
#define MY_MAX_DHCP_RETRY	10   /*DHCP重连最大次数*/
#define DATA_BUF_SIZE           2048 /*获取数据的最大字节数*/

/* Private functions ---------------------------------------------------------*/
void network_init(void);      /*网络初始化*/
void NetworkInitHandler(void);/*配置W5500网络*/
void DhcpRunInLoop(void);     /*DHCP running*/
uint8_t yeelink_get(const char *device_id,const char *sensors_id,char *value);

#endif /* __NET_HANDLER_H__ */