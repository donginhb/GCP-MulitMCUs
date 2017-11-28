



/***********************************<INCLUDES>**********************************/
#include "../../PeriDrivers/W5500/w5500.h"
#include "../../PeriDrivers/W5500/w5500_conf.h"
#include "../../DataType/DataType.h"

#define	MAX_SOCK_NUM		8	/**< Maxmium number of socket  */

uBit16 SSIZE[MAX_SOCK_NUM]={0,0,0,0,0,0,0,0}; // Max Tx buffer
uBit16 RSIZE[MAX_SOCK_NUM]={0,0,0,0,0,0,0,0}; // Max Rx buffer
uBit8 txsize[MAX_SOCK_NUM] = {2,2,2,2,2,2,2,2};//tx buffer set	K bits
uBit8 rxsize[MAX_SOCK_NUM] = {2,2,2,2,2,2,2,2};//rx buffet set  K bits





/*w5500设备层函数*/
//--------------------------------//

//w5500片选控制
void w5500_cs(uBit8 val)
{
    if (LOW == val)
        w5500_dev.w5500_pin_write(w5500_pin_table[2].port,w5500_pin_table[2].pin,0);
    else 
        w5500_dev.w5500_pin_write(w5500_pin_table[2].port,w5500_pin_table[2].pin,1);
}

//w5500 spi 写 buf
void w5500_write_buf(uBit32 addr,uBit8 *buf,uBit16 len)
{
    uBit16 i = 0;

    w5500_cs(LOW);
    w5500_dev.w5500_spi_write((addr & 0x00ff0000) >> 16);
    w5500_dev.w5500_spi_write((addr & 0x0000ff00) >> 8);
    w5500_dev.w5500_spi_write((addr & 0x000000f8) + 4);
    
    for (i = 0; i < len; i++)
    {
        w5500_dev.w5500_spi_write(buf[i]);
    }
    
    w5500_cs(HIGH);
}

//w5500 spi 读 buf
void w5500_read_buf(uBit32 addr,uBit8 *buf,uBit16 len)
{
    uBit16 i = 0;
    
    w5500_cs(LOW);
    w5500_dev.w5500_spi_write((addr & 0x00ff0000) >> 16);
    w5500_dev.w5500_spi_write((addr & 0x0000ff00) >> 8);
    w5500_dev.w5500_spi_write(addr & 0x000000f8);
    
    for (i = 0; i < len; i++)
    {
        buf[i] = w5500_dev.w5500_spi_read();
    }
    
    w5500_cs(HIGH);
}

//w5500 spi 写 byte
void w5500_write_byte(uBit32 addr,uBit8 byte)
{
    w5500_cs(LOW);
    w5500_dev.w5500_spi_write((addr & 0x00ff0000) >> 16);
    w5500_dev.w5500_spi_write((addr & 0x0000ff00) >> 8);
    w5500_dev.w5500_spi_write((addr & 0x000000f8) + 4);
    w5500_dev.w5500_spi_write(byte);
    w5500_cs(HIGH);
}

//w5500 spi 读 byte
uBit8 w5500_read_byte(uBit32 addr)
{
    uBit8 byte = 0;
    
    w5500_cs(LOW);
    w5500_dev.w5500_spi_write((addr & 0x00ff0000) >> 16);
    w5500_dev.w5500_spi_write((addr & 0x0000ff00) >> 8);
    w5500_dev.w5500_spi_write(addr & 0x000000f8);
    byte = w5500_dev.w5500_spi_read();
    w5500_cs(HIGH);
    
    return byte;
}


/*w5500寄存器级配置函数*/
//--------------------------------//

//配置MAC地址
void setSHAR(uBit8 *addr)
{
    w5500_write_buf(SHAR0, addr, 6);  
}

//配置子网掩码
void setSUBR(uBit8 *addr)
{
    w5500_write_buf(SUBR0,addr,4);
}

//获取子网掩码
void getSUBR(uBit8 * addr)
{
    w5500_read_buf(SUBR0, addr, 4);
}

//配置网关
void setGAR(uBit8 *addr)
{
    w5500_write_buf(GAR0,addr,4);
}

//获取网关
void getGAR(uBit8 * addr)
{
    w5500_read_buf(GAR0, addr, 4);
}

//配置本地IP
void setSIPR(uBit8 *addr)
{
    w5500_write_buf(SIPR0,addr,4);
}

//获取本地IP地址
void getSIPR(uBit8 * addr)
{
    w5500_read_buf(SIPR0, addr, 4);
}

//获取socket状态
uBit8 getSn_SR(SOCKET s)
{
    return w5500_read_byte(Sn_SR(s));
}

//读取socket的中断状态
uBit8 getSn_IR(SOCKET s)
{
    return w5500_read_byte(Sn_IR(s));
}

//设置socket的中断状态
void setSn_IR(uBit8 s,uBit8 val)
{
    w5500_write_byte(Sn_IR(s),val);
}

//获取接收数据的长度
uBit16 getSn_RX_RSR(SOCKET s)
{
    uBit16 val = 0,val1 = 0;
    
    do
    {
        val1 = w5500_read_byte(Sn_RX_RSR0(s));
        val1 = (val1 << 8) + w5500_read_byte(Sn_RX_RSR1(s));
        
        if (val1 != 0)
        {
            val = w5500_read_byte(Sn_RX_RSR0(s));
            val = (val << 8) + w5500_read_byte(Sn_RX_RSR1(s));
        }
    }while(val != val1);
    
    return val;
}

//获取socket的发送最大值
uBit16 getIINCHIP_TxMAX(SOCKET s)
{
    return SSIZE[s];
}

//发送数据处理
void send_data_processing(SOCKET s, uBit8 *data, uBit16 len)
{
    uBit16 ptr = 0;
    uBit32 addrbsb = 0;
    
    if(len == 0)
    {
        //printf("CH: %d Unexpected1 length 0\r\n", s);//错误处理
        return;
    }
    
    ptr = w5500_read_byte(Sn_TX_WR0(s));
    ptr = ((ptr & 0x00ff) << 8) + w5500_read_byte(Sn_TX_WR1(s));
    
    addrbsb = (uBit32)(ptr<<8) + (s<<5) + 0x10;
    w5500_write_buf(addrbsb,data,len);
    
    ptr += len;
    w5500_write_byte(Sn_TX_WR0(s),(uBit8)((ptr & 0xff00) >> 8));
    w5500_write_byte(Sn_TX_WR1(s),(uBit8)(ptr & 0x00ff));
}




/*w5500 socket函数*/
//--------------------------------//

//设置每个通道的发送接收数据量大小，单位：Kbits
void socket_buf_init( uBit8 * tx_size, uBit8 * rx_size)
{
    Bit16 i = 0;
    Bit16 ssum = 0;
    Bit16 rsum = 0;
    
    for (i = 0; i < MAX_SOCK_NUM; i++)
    {
        w5500_write_byte(Sn_TXMEM_SIZE(i),tx_size[i]);//注册要发送的数据量
        w5500_write_byte(Sn_RXMEM_SIZE(i),rx_size[i]);//注册要接收的数据量
        
        SSIZE[i] = (Bit16)(0);
        RSIZE[i] = (Bit16)(0);
        
        if (ssum <= 16384)
        {
            SSIZE[i] = (Bit16)tx_size[i] * 1024;
        }
        
        if (rsum <= 16384)
        {
            RSIZE[i] = (Bit16)rx_size[i] * 1024;
        }
        
        ssum += SSIZE[i];
        rsum += RSIZE[i];
    }
}





