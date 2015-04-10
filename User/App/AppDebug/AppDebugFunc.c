/**
 * AppDebugFunc.c file for STM32F103.
 * Describtion: Debug file
 * Author: qinfei 2015.04.02
 * Version: GatewayV1.0
 * Support:qf.200806@163.com
 */

/*调试使用的函数集*/
#include "AppDebugCfg.h"
#include "AppDebugFunc.h"
#include "mystring.h"     //
#include "myportmacro.h"  //数据类型宏定义
#include <stdio.h>

/*调试打印字符串输出*/
void APP_DebugSerialStrOutput(const uint8 *str)
{
    #if (APP_DEBUG_STATE)
    printf("%s",str);
    #else
    ;
    #endif
}
