/**
 * NetHander.c 网络初始化函数
 * Describtion:
 * Author: qinfei 2015.04.09
 * Version: GatewayV1.0
 * Support:qf.200806@163.com
 */
#include "NetHandler.h"
#include "usart.h"//串口
#include "spi.h"
#include "socket.h"//Just include one header for WIZCHIP
#include "dhcp.h"

uint8_t memsize[2][8] = { {2,2,2,2,2,2,2,2},{2,2,2,2,2,2,2,2}}; /* WIZCHIP SOCKET Buffer initialize */

/* Private macro -------------------------------------------------------------*/
uint8_t gDATABUF[DATA_BUF_SIZE];//获取数据的缓冲区：2048

/*默认网络IP地址配置*/
wiz_NetInfo gWIZNETINFO = { .mac = {0x00, 0x08, 0xdc,0x00, 0xab, 0xcd},//MAC地址
                            .ip = {192, 168, 0, 127},                  //IP地址
                            .sn = {255,255,255,0},                     //子网掩码
                            .gw = {192, 168, 0, 1},                    //默认网关
                            .dns = {211,161,192,13},                   //DNS服务器
                            .dhcp = NETINFO_STATIC };

/* Private functions ---------------------------------------------------------*/
static void RegisterSPItoW5500(void);/*将SPI接口函数注册到W5500的socket库中*/
static void InitW5500SocketBuf(void);/*初始化W5500网络芯片*/
static void PhyLinkStatusCheck(void);/* PHY链路状态检查*/
static void DhcpInitHandler(void);   /*DHCP初始化*/
static void my_ip_assign(void);      /*动态分配IP*/          
static void my_ip_conflict(void);    /*IP地址冲突的简单回调函数*/

/* IP地址冲突的简单回调函数 */
static void my_ip_conflict(void)
{
    printf("CONFLICT IP from DHCP\r\n");
    
    //halt or reset or any...
    while(1); // this example is halt.
}

/*******************************************************
 * @ brief Call back for ip assing & ip update from DHCP
 * 动态分配IP等信息
 *******************************************************/
static void my_ip_assign(void)
{
   getIPfromDHCP(gWIZNETINFO.ip);  //IP地址
   getGWfromDHCP(gWIZNETINFO.gw);  //默认网关
   getSNfromDHCP(gWIZNETINFO.sn);  //子网掩码
   getDNSfromDHCP(gWIZNETINFO.dns);//DNS服务器
   gWIZNETINFO.dhcp = NETINFO_DHCP;
   
   /* Network initialization */
   network_init();//应用DHCP分配的网络地址进行网络初始化
   printf("DHCP LEASED TIME : %d Sec.\r\n", getDHCPLeasetime());//打印租得的DHCP网络地址时间
}

/******************************************************************************
 * @brief  Network Init
 * Intialize the network information to be used in WIZCHIP
 *****************************************************************************/
void network_init(void)
{
    uint8_t tmpstr[6] = {0};
    wiz_NetInfo netinfo;

    /*根据gWIZNETINFO结构体设置网络信息*/
    ctlnetwork(CN_SET_NETINFO, (void*)&gWIZNETINFO);

    /*获取设置后的网络信息放入netinfo结构体*/
    ctlnetwork(CN_GET_NETINFO, (void*)&netinfo);

    /* 串口打印网络信息 */
    ctlwizchip(CW_GET_ID,(void*)tmpstr);
    if(netinfo.dhcp == NETINFO_DHCP) 
      printf("\r\n=== %s NET CONF : DHCP ===\r\n",(char*)tmpstr);
    else 
      printf("\r\n=== %s NET CONF : Static ===\r\n",(char*)tmpstr);

    printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n",netinfo.mac[0],netinfo.mac[1],netinfo.mac[2],
                    netinfo.mac[3],netinfo.mac[4],netinfo.mac[5]);
    printf("SIP: %d.%d.%d.%d\r\n", netinfo.ip[0],netinfo.ip[1],netinfo.ip[2],netinfo.ip[3]);
    printf("GAR: %d.%d.%d.%d\r\n", netinfo.gw[0],netinfo.gw[1],netinfo.gw[2],netinfo.gw[3]);
    printf("SUB: %d.%d.%d.%d\r\n", netinfo.sn[0],netinfo.sn[1],netinfo.sn[2],netinfo.sn[3]);
    printf("DNS: %d.%d.%d.%d\r\n", netinfo.dns[0],netinfo.dns[1],netinfo.dns[2],netinfo.dns[3]);
    printf("===========================\r\n");
}

/*将SPI接口函数注册到W5500的socket库中*/
static void RegisterSPItoW5500(void)
{
  /* 1.注册临界区函数 */
  reg_wizchip_cris_cbfunc(SPI_CrisEnter, SPI_CrisExit);
  
  /* 2.注册SPI片选信号函数 */
#if _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_VDM_
  reg_wizchip_cs_cbfunc(SPI_CS_Select, SPI_CS_Deselect);
#elif _WIZCHIP_IO_MODE_ == _WIZCHIP_IO_MODE_SPI_FDM_
  reg_wizchip_cs_cbfunc(SPI_CS_Select, SPI_CS_Deselect);// CS must be tried with LOW.
#else
   #if (_WIZCHIP_IO_MODE_ & _WIZCHIP_IO_MODE_SIP_) != _WIZCHIP_IO_MODE_SIP_
      #error "Unknown _WIZCHIP_IO_MODE_"
   #else
      reg_wizchip_cs_cbfunc(wizchip_select, wizchip_deselect);
   #endif
#endif
      
  /* 3.注册读写函数 */
  reg_wizchip_spi_cbfunc(SPI_ReadByte, SPI_WriteByte);
}

/*初始化W5500网络芯片:直接调用官方提供的初始化库*/
static void InitW5500SocketBuf(void)
{
  /* WIZCHIP SOCKET Buffer initialize */
  if(ctlwizchip(CW_INIT_WIZCHIP,(void*)memsize) == -1){
      printf("WIZCHIP Initialized fail.\r\n");
      while(1);
  }
}

/* PHY链路状态检查*/
static void PhyLinkStatusCheck(void)
{
  uint8_t tmp;
  do{
    if(ctlwizchip(CW_GET_PHYLINK, (void*)&tmp) == -1){
      printf("Unknown PHY Link stauts.\r\n");
    }
  }while(tmp == PHY_LINK_OFF);
}

/*DHCP初始化*/
static void DhcpInitHandler(void)
{
   /* >> DHCP Client 				  */
  /************************************************/
  
  //must be set the default mac before DHCP started.
  setSHAR(gWIZNETINFO.mac);

  DHCP_init(SOCK_DHCP, gDATABUF);//gDATABUF获取数据的缓冲区
  
  // if you want defiffent action instead defalut ip assign,update, conflict,
  // if cbfunc == 0, act as default.
  //参数:分配的IP、更新的IP、冲突的IP
  reg_dhcp_cbfunc(my_ip_assign, my_ip_assign, my_ip_conflict);
}

/*动态分配IP地址*/
void DhcpRunInLoop(void)
{
  static uint8_t my_dhcp_retry = 0;
  switch(DHCP_run())
  {
    case DHCP_IP_ASSIGN://DHCP IP分配
    case DHCP_IP_CHANGED://DHCP IP更新
        /* If this block empty, act with default_ip_assign & default_ip_update */
        // This example calls my_ip_assign in the two case.
        break;
    
    case DHCP_IP_LEASED://TO DO YOUR NETWORK APPs.
        //在租得的DHCP网络地址时间内执行网络App
        break;
    
    case DHCP_FAILED://动态IP分配失败
        /* ===== Example pseudo code =====  */
        // The below code can be replaced your code or omitted.
        // if omitted, retry to process DHCP
        my_dhcp_retry++;
        if(my_dhcp_retry > MY_MAX_DHCP_RETRY)//动态IP分配失败大于十次，将使用静态的IP
        {
            printf(">> DHCP %d Failed\r\n", my_dhcp_retry);
            my_dhcp_retry = 0;
            DHCP_stop();// if restart, recall DHCP_init()
            network_init();// apply the default static network and print out netinfo to serial
        }
        break;
        
    default:
        break;
  }  
}

/*配置W5500网络*/
void NetworkInitHandler(void)
{
    RegisterSPItoW5500();/*将SPI接口函数注册到W5500的socket库中*/
    InitW5500SocketBuf();/*初始化W5500网络芯片:直接调用官方提供的初始化库*/
    PhyLinkStatusCheck();/* PHY链路状态检查*/
    DhcpInitHandler();   /*DHCP初始化*/
}




