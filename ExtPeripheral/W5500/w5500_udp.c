






/***********************************<INCLUDES>**********************************/
#include "../../PeriDrivers/W5500/w5500.h"
#include "../../PeriDrivers/W5500/w5500_udp.h"
#include "../../PeriDrivers/W5500/w5500_socket.h"
#include "../../PeriDrivers/W5500/w5500_conf.h"
#include "../../DataType/DataType.h"


//UDP服务轮询
void do_udp(void)
{
    uBit16 len = 0;
    uBit8 buff[2048];//定义2KB缓存
    
    //获取socket状态
    switch(getSn_SR(SOCK_UDPS))
    {
        //socket处于关闭状态
        case SOCK_CLOSED:
            socket(SOCK_UDPS,Sn_MR_UDP,local_port,0);//初始化socket
            break;
        //socket初始化完成
        case SOCK_UDP:
            w5500_dev.delay_ms(10);
            
            if (getSn_IR(SOCK_UDPS) & Sn_IR_RECV)
            {
                setSn_IR(SOCK_UDPS,Sn_IR_RECV);//清接收中断
            }
            
            if ((len = getSn_RX_RSR(SOCK_UDPS)) > 0)
            {
                recvfrom(SOCK_UDPS,buff,len,remote_ip,&remote_port);//w5500接收发送过来的数据
                buff[len - 8] = 0x0;//添加字符串结束符
                sendto(SOCK_UDPS,buff,len - 8,remote_ip,remote_port);//w5500把接收到的数据发送给remote
            }
            break;
    }
}











