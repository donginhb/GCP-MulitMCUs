





/***********************************<INCLUDES>**********************************/
#include "../../PeriDrivers/W5500/w5500_socket.h"
#include "../../PeriDrivers/W5500/w5500.h"
#include "../../PeriDrivers/W5500/w5500_conf.h"
#include "../../DataType/DataType.h"


//关闭socket
void close(SOCKET s)
{
    w5500_write_byte(Sn_SR(s),Sn_CR_CLOSE);
    while(w5500_read_byte(Sn_CR(s)))
    {
        ;//等待命令处理完
    }
    
    w5500_write_byte(Sn_IR(s),0xff);
}

//设置socket
uBit8 socket(SOCKET s,uBit8 protocol,uBit16 port,uBit8 flag)
{
    uBit8 ret = 0;
    
    if (((protocol & 0x0f) == Sn_MR_TCP) ||
        ((protocol & 0x0f) == Sn_MR_UDP) ||
        ((protocol & 0x0f) == Sn_MR_IPRAW) ||
        ((protocol & 0x0f) == Sn_MR_MACRAW) ||
        ((protocol & 0x0f) == Sn_MR_PPPOE))
    {
        close(s);
        w5500_write_byte(Sn_MR(s),protocol | flag);
        
        if (port != 0)
        {
            w5500_write_byte(Sn_PORT0(s),(uBit8)((port & 0xff00) >> 8));
            w5500_write_byte(Sn_PORT1(s),(uBit8)(port & 0x00ff));
        }
        else 
        {
            //如果没设置源端口，设置成本地端口
            local_port++;
            w5500_write_byte(Sn_PORT0(s),(uBit8)((local_port & 0xff00) >> 8));
            w5500_write_byte(Sn_PORT1(s),(uBit8)(local_port & 0x00ff));
        }
        
        w5500_write_byte(Sn_CR(s),Sn_CR_OPEN);//运行sockinit Sn_CR
        while(w5500_read_byte(Sn_CR(s)))
        {
            ;//等待命令处理完
        }
        ret = 1;
    }
    else 
    {
        ret = 0;
    }
              
    return ret;
}

//socket接收数据
uBit16 recvfrom(SOCKET s,uBit8 *buf,uBit16 len,uBit8 *addr,uBit16 *port)
{
    uBit8 head[8];
    uBit16 data_len = 0;
    uBit16 ptr = 0;
    uBit32 addrbsb = 0;
    
    if (len > 0)
    {
        ptr = w5500_read_byte(Sn_RX_RD0(s));
        ptr = ((ptr & 0x00ff) << 8) + w5500_read_byte(Sn_RX_RD1(s));
        addrbsb = (uBit32)(ptr << 8) + (s << 5) + 0x18;
        
        switch(w5500_read_byte(Sn_MR(s)) & 0x07)
        {
        case Sn_MR_UDP:
            w5500_read_buf(addrbsb,head,0x08);
            ptr += 8;
            //读IP地址，端口号
            addr[0]  = head[0];
            addr[1]  = head[1];
            addr[2]  = head[2];
            addr[3]  = head[3];
            *port    = head[4];
            *port    = (*port << 8) + head[5];
            data_len = head[6];
            data_len = (data_len << 8) + head[7];
            
            addrbsb  = (uBit32)(ptr << 8) + (s << 5) + 0x18;
            w5500_read_buf(addrbsb,buf,data_len);
            ptr += data_len;
            
            w5500_write_byte(Sn_RX_RD0(s),(uBit8)((ptr & 0xff00) >> 8));
            w5500_write_byte(Sn_RX_RD1(s),(uBit8)(ptr & 0x00ff));
            break;
            
        case Sn_MR_IPRAW:
            w5500_read_buf(addrbsb,head,0x06);
            ptr += 6;
            addr[0]  = head[0];
            addr[1]  = head[1];
            addr[2]  = head[2];
            addr[3]  = head[3];
            data_len = head[4];
            data_len = (data_len << 8) + head[5];
            
            addrbsb  = (uBit32)(ptr<<8) +  (s<<5) + 0x18;
            
            w5500_read_buf(addrbsb,buf,data_len);
            
            ptr += data_len;
            
            w5500_write_byte(Sn_RX_RD0(s),(uBit8)((ptr & 0xff00) >> 8));
            w5500_write_byte(Sn_RX_RD1(s),(uBit8)(ptr & 0x00ff));
            break;
            
        case Sn_MR_MACRAW:
            w5500_read_buf(addrbsb,head,0x02);
            ptr += 2;
            data_len = head[0];
            data_len = (data_len<<8) + head[1] - 2;
             
            if(data_len > 1514)
            {
                //printf("data_len over 1514\r\n");//超出数据长度处理，此处根据实际情况修改
            }
            addrbsb  = (uBit32)(ptr<<8) +  (s<<5) + 0x18;
            w5500_read_buf(addrbsb,buf,data_len);
            ptr += data_len;
            
            w5500_write_byte(Sn_RX_RD0(s),(uBit8)((ptr & 0xff00) >> 8));
            w5500_write_byte(Sn_RX_RD1(s),(uBit8)(ptr & 0x00ff));
            break;
            
        default:
            break;
        }
        w5500_write_byte(Sn_CR(s),Sn_CR_RECV);
        while(w5500_read_byte(Sn_CR(s)))
        {
            ;
        }
    }
      
    return data_len;
}

//socket发送数据
uBit16 sendto(SOCKET s,const uBit8 *buf,uBit16 len,uBit8 *addr,uBit16 port)
{
    uBit16 ret = 0;
    
    if (len > getIINCHIP_TxMAX(s)) 
    {
        ret = getIINCHIP_TxMAX(s);//检查发送最大值，如果超过则设为最大值
    }
    else
    {
        ret = len;
    }
    
    if(((addr[0] == 0x00) && (addr[1] == 0x00) && (addr[2] == 0x00) && (addr[3] == 0x00)) || ((port == 0x00)))
    {
        ret = 0;
    }
    else 
    {
        w5500_write_byte(Sn_DIPR0(s),addr[0]);
        w5500_write_byte(Sn_DIPR1(s),addr[1]);
        w5500_write_byte(Sn_DIPR2(s),addr[2]);
        w5500_write_byte(Sn_DIPR3(s),addr[3]);
        w5500_write_byte(Sn_DPORT0(s),(uBit8)((port & 0xff00) >> 8));
        w5500_write_byte(Sn_DPORT1(s),(uBit8)(port & 0x00ff));
        
        //复制数据
        send_data_processing(s,(uBit8 *)buf,ret);
        w5500_write_byte(Sn_CR(s),Sn_CR_SEND);
        while(w5500_read_byte(Sn_CR(s)))
        {
            ;
        }
        
        while((w5500_read_byte(Sn_IR(s)) & Sn_IR_SEND_OK) != Sn_IR_SEND_OK )
        {
            if (w5500_read_byte(Sn_IR(s)) & Sn_IR_TIMEOUT)
            {
                //清除中断
                w5500_write_byte(Sn_IR(s),(Sn_IR_SEND_OK | Sn_IR_TIMEOUT));
                
                return 0;
            }
        }
        
        w5500_write_byte(Sn_IR(s),Sn_IR_SEND_OK);
    }
    
    return ret;
}



