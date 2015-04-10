/**
 * myportmacro.h file for STM32F103.
 * Describtion: 数据类型定义移植
 * Author: qinfei 2015.04.02
 * Version: GatewayV1.0
 * Support:qf.200806@163.com
 */

#ifndef __MY_PORT_MACRO_H__
#define __MY_PORT_MACRO_H__

#define BOOL_TRUE  0xff	//定义布尔真值
#define BOOL_FALSE 0x00 //定义布尔假值

typedef unsigned short int uint16;
typedef unsigned short int UINT16;
typedef unsigned short int WORD;
typedef unsigned short int word;

typedef signed short int int16;
typedef signed short int INT16;

typedef unsigned int uint32;
typedef unsigned int UINT32;
typedef unsigned int DWORD;
typedef unsigned int dword;

typedef signed int int32;
typedef signed int INT32;

typedef signed char int8;
typedef signed char INT8;

typedef unsigned char byte;
typedef unsigned char BYTE;
typedef unsigned char uchar;
typedef unsigned char UINT8;
typedef unsigned char uint8;
typedef unsigned char BOOL;

#endif /*__MY_PORT_MACRO_H__*/
