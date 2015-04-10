/**
 * AppSoftWareDriver.h APP任务驱动函数
 * Describtion:
 * Author: qinfei 2015.04.02
 * Version: GatewayV1.0
 * Support:qf.200806@163.com
 */

/*命令处理区的应用*/
#ifndef __APP_SOFT_WARE_DRIVER_H__
#define __APP_SOFT_WARE_DRIVER_H__

#include "myportmacro.h"

/*时间差*/
extern uint32 TimeDifference(uint32 t1,uint32 t2);

/*心跳处理函数*/
extern void HeartBeatHandler(void);


#endif  /*__APP_SOFT_WARE_DRIVER_H__*/
