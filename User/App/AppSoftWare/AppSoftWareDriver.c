/**
 * AppSoftWareDriver.c APP任务驱动函数
 * Describtion:
 * Author: qinfei 2015.04.02
 * Version: GatewayV1.0
 * Support:qf.200806@163.com
 */

#include "myportmacro.h"
#include "APPDebugFunc.h"
#include "AppSoftWareDriver.h"  //系统执行驱动文件


/*定义局部网络参数*/

//*********************************函数实体区**********************************

/**
  * @fun    TimeDifference
  * @brief  时间差
  * @param  t1
  * @param  t2
  * @retval time difference
  */
uint32 TimeDifference(uint32 t1,uint32 t2)
{
    if(t1>t2) return t1-t2;
    else      return t2-t1;
}


/*心跳处理函数*/
void HeartBeatHandler(void)
{
    APP_DebugSerialStrOutput("\r\nRun into HeartBeatHandler function !\r\n");
}
