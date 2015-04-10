/**
 * ZigBeeInterface.h file for STM32F103.
 * Describtion: 网关与ZigBee协调器接口的数据结构
 * Author: qinfei 2015.04.02
 * Version: GatewayV1.0
 * Support:qf.200806@163.com
 */

#ifndef __ZIGBEE_INTERFACE_H__
#define __ZIGBEE_INTERFACE_H__
#include "myportmacro.h"

/**
  * @brief 网络参数
  * @brief 信道、发射功率、panID
  */
typedef struct{
    uint8  Channel;//信道
    uint8  Txpower;//发射功率
    uint16 panID;  //panID
}NetParamt_t;

/**
  * @brief 节点信息
  * @brief 节点类型、节点应用类型
  */
typedef struct {
    uint8  NodeType;	//NodeType
    uint8  NodeAppType; //Node App Type
    uint8  Reserved[2];	//保留位-供需使用
}NodeInfo_t;

/**
  * @brief 父节点信息
  * @brief 父节点类型、父节点应用类型、父节点短地址
  */
typedef struct {
    uint8  pNodeType;   //父节点类型
    uint8  pNodeAppType;//父节点应用类型
    uint16 pNwkSrc;	//父节点Nwk
}pNodeInfo_t;

/**
  * @brief 绑定控制和管理相关参数
  * @brief 短地址和clusterID
  */
typedef struct{
    uint16 NwkDest;
    uint16 ClusterID;
}BindCtrl_t;

/**
  * @brief 灯参数和变量
  */
typedef struct
{
    uint8  CtrlMode;  //控制灯的模式: 0-瞬间变亮 ; 1-逐渐变亮；
    uint8  CtrlSelect;//冷暖LED选择: 0-暖光LED(P17默认) ; 1-冷光LED(P16可选);
    uint8  CtrlVal;   //控制灯的值
    uint8  Reserved;  //保留位-可以作为亮度调节
}LState_t;

/**
  * @brief 窗帘参数和变量
  */
typedef struct{
    uint8  CtrlVal;    //控制灯的模式: (val=100)窗帘开0x01；(val=0)窗帘关0x02；(val=50)窗帘停0x03；
    uint8  Reserved[3];//保留位-供需使用 
}Curtain_t; 

/**
  * @brief 彩灯参数和变量
  * @brief 以类似结构向后扩展
  */
typedef struct{
    uint8  Red;	    //红色值
    uint8  Green;   //绿色值
    uint8  Blue;    //蓝色值
    uint8  Reserved;//保留位-可以作为亮度调节
}ColorL_t;

/**
  * @brief 插座参数和变量
  * @brief 以类似结构向后扩展
  */
typedef struct{
    uint8  Reserved[4];	//保留位-
}Socket_t;

/**
  * @brief 需要传递的信息，包括节点信息和地址信息等，长度为4个字节
  */
typedef union{
    NetParamt_t NetParamt;  //网络参数
    NodeInfo_t  NodeInfo;   //节点信息
    pNodeInfo_t pNodeInfo;  //父节点信息
    BindCtrl_t  BindCtrl;   //绑定控制和管理相关参数
    LState_t    LState;     //灯参数和变量
    Curtain_t   Curtain;    //窗帘参数和变量
    ColorL_t    ColorL;     //彩灯参数和变量
    Socket_t    Socket;     //插座
}UnionInfo_t;

/**
  * @brief 节点控制信息 & mac地址信息
  */
typedef union{
    struct{
        UnionInfo_t info;
        uint8       Reserved[4];
    }U_info;
    uint8           mac[8];
}UnionInfo_mac;

/**
  * @brief 节点信息
  */
typedef struct{
  UnionInfo_mac Info_mac;/*需要传递的信息，包括节点信息和地址信息等*/
  uint16        NwkDest; /*NwkDest目的地址*/
}SendNode_t;

/**
  * @brief 发送和接收的命令串数据结构
  * @brief 最终使用的12字节数据结构
  */
typedef struct{
  uint8            CMD;       /*命令Command    */
  uint8            ClusterID; /*ClusterID Flag */
  SendNode_t       SendNode;  /*节点信息       */
}Send_Recv_CMDA_t;

//-----------------------------------------------------------------------------

/*定义zigbee网络内的数据结构*/
typedef UnionInfo_mac ZigbeeData_t;

//-----------------------------------------------------------------------------

/**
  * @brief Zigbee网络内所有节点的节点应用类型定义
  */
#define NODE_APP_TYPE_LIGHT             0x01  //灯类型
#define NODE_APP_TYPE_CLIGHT            0x02  //彩灯类型
#define NODE_APP_TYPE_CURTAIN           0x03  //窗帘类型
#define NODE_APP_TYPE_GETEWAY           0x04  //网关类型


#endif /*__ZIGBEE_INTERFACE_H__*/
