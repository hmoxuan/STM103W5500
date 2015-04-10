/**
 * NetHander.c 网络初始化函数
 * Describtion:
 * Author: qinfei 2015.04.09
 * Version: GatewayV1.0
 * Support:qf.200806@163.com
 */
#include "NetHandler.h"
#include "delay.h"//延时函数
#include "usart.h"//串口
#include "stm32timer.h"
#include "spi.h"
#include "socket.h"//Just include one header for WIZCHIP
#include "dhcp.h"
#include "dns.h"
#include <string.h>

uint8_t memsize[2][8] = {{2,2,2,2,2,2,2,2},{2,2,2,2,2,2,2,2}}; /* WIZCHIP SOCKET Buffer initialize */

uint8_t domain_ip[4]={0};/*域名IP*/
uint8_t domain_name[]="yeelink.net";/*域名*/

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
void DNS_Analyse(void);/*DNS解析*/

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

   DNS_Analyse();//域名解析
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
    //DNS服务在gWIZNETINFO变量中必须设置正确，否则可能无法正确分析域名。
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
  char value[16]={0};
  int32_t t=0;
  
  
  switch(DHCP_run())
  {
    case DHCP_IP_ASSIGN://DHCP IP分配
    case DHCP_IP_CHANGED://DHCP IP更新
        /* If this block empty, act with default_ip_assign & default_ip_update */
        // This example calls my_ip_assign in the two case.
        break;
    
    case DHCP_IP_LEASED://TO DO YOUR NETWORK APPs.
        //在租得的DHCP网络地址时间内执行网络App
        //yeelink_get("19610","34409",value);//×￠òaDèòaDT??3é×??oyeelinkéè±?μ?ID--LED1
        yeelink_get("19657","34484",value);//×￠òaDèòaDT??3é×??oyeelinkéè±?μ?ID--LED2
        printf("%s\n\r",value);	//char value[16]={0};
        printf("\n\r");
        for(t=0;t<11;t++){
          delay_ms(1000);
        } 
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
            DNS_Analyse();//域名解析
        }
        break;
        
    default:
        break;
  }  
}

/*DNS解析*/
void DNS_Analyse(void)
{
    int32_t ret = 0;
  
    /*初始化DNS解析程序，并通过调用相关函数获取到machtalk服务器的真实IP地址*/
    /* DNS client initialization */
    DNS_init(SOCK_DNS, gDATABUF);
    Timer_Start();/*启动Timer3*/
    /* DNS procssing */
    if ((ret = DNS_run(gWIZNETINFO.dns, domain_name, domain_ip)) > 0){ // try to 1st DNS
        printf("> 1st DNS Reponsed\r\n");
    }else if(ret == -1){
        printf("> MAX_DOMAIN_NAME is too small. Should be redefined it.\r\n");
        Timer_Stop();/*关闭Timer3*/
        while(1);
    }else{
        printf("> DNS Failed\r\n");
        Timer_Stop();/*关闭Timer3*/
        while(1);
    }

    //DNS解析成功：IP地址存储在domain_ip变量中，后面的socket编程会用到这个变量值。
    if(ret > 0){
        printf("> Translated %s to %d.%d.%d.%d\r\n",domain_name,domain_ip[0],domain_ip[1],domain_ip[2],domain_ip[3]);
    }
    Timer_Stop();
}



/*配置W5500网络*/
void NetworkInitHandler(void)
{
    RegisterSPItoW5500();/*将SPI接口函数注册到W5500的socket库中*/
    InitW5500SocketBuf();/*初始化W5500网络芯片:直接调用官方提供的初始化库*/
    PhyLinkStatusCheck();/* PHY链路状态检查*/
    DhcpInitHandler();   /*DHCP初始化*/
}

/* 循环获取开关量的值，并将获取到的结果提交出来并串口打印输出，当然也可以根据这个值来
 * 控制LED灯，循环获取间隔时间必须大于10s，这个是yeelink限制的。
 */
uint8_t yeelink_get(const char *device_id,const char *sensors_id,char *value)
{
    int ret;
    char* presult;
    char remote_server[] = "api.yeelink.net";
    char str_tmp[128] = {0};

    // 请求缓冲区和响应缓冲区
    static char http_request[DATA_BUF_SIZE] = {0};	//声明为静态变量，防止堆栈溢出
    static char http_response[DATA_BUF_SIZE] = {0};	//声明为静态变量，防止堆栈溢出
    sprintf(str_tmp,"/v1.0/device/%s/sensor/%s/datapoints",device_id,sensors_id);

    // 确定 HTTP请求首部
    // 例如POST /v1.0/device/98d19569e0474e9abf6f075b8b5876b9/1/1/datapoints/add HTTP/1.1\r\n
    sprintf( http_request , "GET %s HTTP/1.1\r\n",str_tmp);


    // 增加属性 例如 Host: api.machtalk.net\r\n
    sprintf( str_tmp , "Host:%s\r\n" , remote_server);
    strcat( http_request , str_tmp);

    // 增加密码 例如 APIKey: d8a605daa5f4c8a3ad086151686dce64
    //sprintf( str_tmp , "U-ApiKey:%s\r\n" , "d8a605daa5f4c8a3ad086151686dce64");//需要替换为自己的APIKey
    sprintf( str_tmp , "U-ApiKey:%s\r\n" , "e5da11d13d2e5f540ef1a99b3506e081");//APIKey--qinfei
    strcat( http_request , str_tmp);
    //
    strcat( http_request , "Accept: */*\r\n");
    // 增加表单编码格式 Content-Type:application/x-www-form-urlencoded\r\n
    strcat( http_request , "Content-Type: application/x-www-form-urlencoded\r\n");
    strcat( http_request , "Connection: close\r\n");
    // HTTP首部和HTTP内容 分隔部分
    strcat( http_request , "\r\n");

    //将数据通过TCP发送出去
    //新建一个Socket并绑定本地端口5000
    ret = socket(SOCK_TCPS,Sn_MR_TCP,5000,0x00);
    if(ret != SOCK_TCPS){
        printf("%d:Socket Error\r\n",SOCK_TCPS);
        while(1);
    }
    //连接TCP服务器
    ret = connect(SOCK_TCPS,domain_ip,80);
    if(ret != SOCK_OK){
        printf("%d:Socket Connect Error\r\n",SOCK_TCPS);
        while(1);
    }	
    //发送请求
    ret = send(SOCK_TCPS,(unsigned char *)http_request,strlen(http_request));
    if(ret != strlen(http_request)){
        printf("%d:Socket Send Error\r\n",SOCK_TCPS);
        while(1);
    }

    // 获得响应
    ret = recv(SOCK_TCPS,(unsigned char *)http_response,DATA_BUF_SIZE);
    if(ret <= 0){
        printf("%d:Socket Get Error\r\n",SOCK_TCPS);
        while(1);
    }
    http_response[ret] = '\0';
    //判断是否收到HTTP OK
    presult = strstr( (const char *)http_response , (const char *)"200 OK\r\n");
    if( presult != NULL ){
        static char strTmp[DATA_BUF_SIZE]={0};//声明为静态变量，防止堆栈溢出
        sscanf(http_response,"%*[^{]{%[^}]",strTmp);
        //提取返回信息
        char timestamp[64]={0};
        char timestampTmp[64]={0};
        char valueTmp[64]={0};
        sscanf(strTmp,"%[^,],%[^,]",timestampTmp,valueTmp);
        strncpy(timestamp,strstr(timestampTmp,":")+2,strlen(strstr(timestampTmp,":"))-3);
        strncpy(value,strstr(valueTmp,":")+1,strlen(strstr(valueTmp,":"))-1);
    }else{
        printf("Http Response Error\r\n");
        printf("%s",http_response);
    }
    close(SOCK_TCPS);
    return 0;
}
/*********************************END OF FILE**********************************/

