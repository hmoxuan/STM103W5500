/**
 * ZigBeeCmdDefine.h file for STM32F103.
 * Describtion: 网关与ZigBee协调器的命令宏集合
 * Author: qinfei 2015.04.02
 * Version: GatewayV1.0
 * Support:qf.200806@163.com
 */

/*用户自定义宏区域*/
#ifndef __ZIGBEE_CMD_DEFINE_H__
#define __ZIGBEE_CMD_DEFINE_H__

//*****节点信息查询*******************
//************************************
#define CMD_NODE_INFO_LOOKUP      0x01  //CMD

//------------------------------------

#define CID_NODE_TYPE_LOOKUP      0x01  //节点类型查询
#define CID_NODE_ALLNWK_LOOKUP    0x02  //所有节点地址信息查询
#define CID_NODE_NWK_LOOPUP       0x03  //节点nwk地址查询-根据mac地址
#define CID_NODE_MAC_LOOPUP       0x04  //节点mac地址查询-根据nwk地址
#define CID_NODE_PANID_LOOPUP     0x05  //节点PANID信息查询
#define CID_NODE_EXTPANID_LOOPUP  0x06  //节点extPANID信息查询
#define CID_NODE_CHANNEL_LOOPUP   0x07  //节点信道查询
#define CID_NODE_P_NWK_LOOPUP     0x08  //父节点nwk地址查询
#define CID_NODE_P_MAC_LOOPUP     0x09  //父节点mac地址查询
#define CID_NODE_APPTYPE_LOOKUP   0x0A  //节点应用类型查询
#define CID_NODE_ALLMAC_LOOKUP    0x0B  //所有节点mac地址信息查询--2014/09/12
#define CID_NODE_OFF_LINE         0x0C  //节点掉线--2014/09/15
#define CID_NODE_ON_LINE          0x0D  //节点上线--2014/09/17

//*****网络操作***********************
//************************************
#define CMD_NETWORK_OPRT          0x02  //CMD

//------------------------------------

#define CID_NETWORK_FORMAT_REQ    0x01  //网络格式化，重启网络使用
#define CID_NETWORK_PERMIT_JOIN   0x02  //允许加入网络
#define CID_NETWORK_PERMIT_LEAVE  0x03  //允许离开网络
#define CID_NETWORK_REMOVE_CHILD  0x04  //移除子节点
#define CID_NETWORK_FOUND         0x05  //网络发现
#define CID_NETWORK_GETPARAMET    0x06  //获取网络参数

//*****灯控制操作*********************
//************************************
#define CMD_LIGHT_OPRT            0x03  //CMD

//------------------------------------

#define CID_LIGHT_TURN_ON         0x01  //灯开启
#define CID_LIGHT_TURN_OFF        0x02  //灯关闭
#define CID_GET_LIGHT_STATE       0x03  //获取灯状态
#define CID_LIGHT_SELECT_STATE    0x04  //灯切换(亮度保持不变)


//*****窗帘控制操作*******************
//************************************
#define CMD_CURTAIN_OPRT          0x04   //CMD

//------------------------------------

#define CID_CURTAIN_OPEN          0x01  //窗帘打开
#define CID_CURTAIN_CLOSE         0x02  //窗帘关闭
#define CID_CURTAIN_STOP          0x03  //窗帘停止

//*****RGB LED控制操作****************
//************************************
#define CMD_RGBLED_OPRT           0x05   //CMD

//------------------------------------

#define CID_SET_RGBLED_STATE      0x01  //设置 RGB LED 的色彩
#define CID_GET_RGBLED_STATE      0x02  //获取 RGB LED 的状态

//*********插座控制操作****************
//************************************
#define CMD_SOCKET_OPRT           0x06   //CMD

//------------------------------------

#define CID_SOCKET_TURN_ON        0x01  //插座开
#define CID_SOCKET_TURN_OFF       0x02  //插座关


//*****系统层处理功能集***************
//************************************
#define CMD_SYSTEM_FUNC           0x0F   //CMD

//------------------------------------

#define CID_HEARTBEAT             0x01  //心跳独立事件ID
#define CID_VERSION_LOOKUP        0x02  //版本查看
#define CID_HEARTBEAT_RSP         0x03  //心跳应答

//*******************************公共ClusterID*********************************
//Zigbee网络内使用的控制ClusterID
//-----------------------------------------------------------------------------

#define EXEAPP_CID_NODE_TYPE_LOOKUP      0x0101  //节点类型查询
#define EXEAPP_CID_NODE_ALLNWK_LOOKUP    0x0102  //所有节点地址信息查询
#define EXEAPP_CID_NODE_NWK_LOOPUP       0x0103  //节点nwk地址查询-根据mac地址
#define EXEAPP_CID_NODE_MAC_LOOPUP       0x0104  //节点mac地址查询-根据nwk地址
#define EXEAPP_CID_NODE_PANID_LOOPUP     0x0105  //节点PANID信息查询
#define EXEAPP_CID_NODE_EXTPANID_LOOPUP  0x0106  //节点extPANID信息查询
#define EXEAPP_CID_NODE_CHANNEL_LOOPUP   0x0107  //节点信道查询
#define EXEAPP_CID_NODE_P_NWK_LOOPUP     0x0108  //父节点nwk地址查询
#define EXEAPP_CID_NODE_P_MAC_LOOPUP     0x0109  //父节点mac地址查询
#define EXEAPP_CID_NODE_APPTYPE_LOOKUP   0x010A  //节点应用类型查询
#define EXEAPP_CID_NODE_ALLMAC_LOOKUP    0x010B  //所有节点mac地址信息查询--2014/09/12
#define EXEAPP_CID_NODE_OFF_LINE         0x010C  //节点掉线--2014/09/15

//-----------------------------------------------------------------------------

#define EXEAPP_CID_NETWORK_FORMAT_REQ    0x0201  //网络格式化，重启网络使用
#define EXEAPP_CID_NETWORK_PERMIT_JOIN   0x0202  //允许加入网络
#define EXEAPP_CID_NETWORK_PERMIT_LEAVE  0x0203  //允许离开网络
#define EXEAPP_CID_NETWORK_REMOVE_CHILD  0x0204  //移除子节点
#define EXEAPP_CID_NETWORK_FOUND         0x0205  //网络发现
#define EXEAPP_CID_NETWORK_GETPARAMET    0x0206  //获取网络参数

//-----------------------------------------------------------------------------

#define EXEAPP_CID_LIGHT_TURN_ON         0x0301  //灯开启
#define EXEAPP_CID_LIGHT_TURN_OFF        0x0302  //灯关闭
#define EXEAPP_CID_GET_LIGHT_STATE       0x0303  //获取灯状态
#define EXEAPP_CID_LIGHT_SELECT_STATE    0x0304  //获取灯切换(亮度保持不变)

//-----------------------------------------------------------------------------

#define EXEAPP_CID_CURTAIN_OPEN          0x0401  //窗帘打开
#define EXEAPP_CID_CURTAIN_CLOSE         0x0402  //窗帘关闭
#define EXEAPP_CID_CURTAIN_STOP          0x0403  //窗帘停止

//-----------------------------------------------------------------------------

#define EXEAPP_CID_SET_RGBLED_STATE      0x0501  //设置 RGB LED 状态
#define EXEAPP_CID_GET_RGBLED_STATE      0x0502  //获取 RGB LED 状态

//-----------------------------------------------------------------------------

#define EXEAPP_CID_SOCKET_TURN_ON        0x0601  //插座开
#define EXEAPP_CID_SOCKET_TURN_OFF       0x0602  //插座关

//-----------------------------------------------------------------------------

#define EXEAPP_CID_HEARTBEAT             0x0F01  //心跳独立事件ID
#define EXEAPP_CID_VERSION_LOOKUP        0x0F02  //版本查看
#define EXEAPP_CID_HEARTBEAT_RSP         0x0F03  //心跳应答

#endif /*__ZIGBEE_CMD_DEFINE_H__*/
