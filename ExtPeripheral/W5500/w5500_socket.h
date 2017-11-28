#ifndef  __W5500_SOCKET_H
#define  __W5500_SOCKET_H

#include "../../DataType/DataType.h"
#include "../../PeriDrivers/W5500/w5500.h"

typedef enum
{
    SOCK_TCPS = 0,
    SOCK_TCPC,
    SOCK_UDPS,
    SOCK_DHCP,
}Socket_Status;



/*函数声明*/
uBit8 socket(SOCKET s,uBit8 protocol,uBit16 port,uBit8 flag);//设置socket
uBit16 recvfrom(SOCKET s,uBit8 *buf,uBit16 len,uBit8 *addr,uBit16 *port);//socket接收数据
uBit16 sendto(SOCKET s,const uBit8 *buf,uBit16 len,uBit8 *addr,uBit16 port);//socket发送数据




#endif
