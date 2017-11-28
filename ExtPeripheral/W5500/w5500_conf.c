


/***********************************<INCLUDES>**********************************/
#include "../../SysPeripheral/GPIO/GPIO_Man.h"
#include "../../PeriDrivers/W5500/w5500.h"
#include "../../PeriDrivers/W5500/w5500_conf.h"
#include "../../DataType/DataType.h"
#include <stdio.h>
#include <string.h>

W5500_Dev w5500_dev;//w5500设备

//w5500引脚表
W5500_Pin_Table w5500_pin_table[3] = 
{
    {0,3,GPIO_MODE_OUPUT},//reset pin
    {0,12,GOIO_MODE_INPUT},//int pin
    {1,12,GPIO_MODE_OUPUT},//cs pin
};

Config_Msg config_msg;//w5500配置
uBit8 mac[6] = {0x00,0x08,0xdc,0x11,0x11,0x11};//定义MAC地址，如果多块w5500网络芯片工作在同一现场，请使用不同的MAC地址
uBit8 local_ip[4] = {192,168,114,114};//w5500默认IP地址
uBit8 subnet[4] = {255,255,255,0};//w5500默认子网掩码
uBit8 gateway[4] = {192,168,1,1};//w5500默认网关
uBit8 dns_server[4] = {114,114,114,114};//w5500默认DNS

uBit16 local_port = 5001;//w5500本地端口号

uBit8 remote_ip[4] = {192,168,1,162};//远端IP地址
uBit16 remote_port = 5007;//远端端口号

/*w5500底层函数*/
//--------------------------------//

//w5500设备注册
void w5500_dev_register(void    (*w5500_spi_init)    (void),
                        void    (*w5500_spi_write)   (uBit8 data),
                        uBit8   (*w5500_spi_read)    (void),
                        void    (*w5500_pin_init)    (uBit16 port,uBit16 pin,uBit8 mode),
                        void    (*w5500_pin_write)   (uBit16 port,uBit16 pin,uBit8 state),
                        uBit8   (*w5500_pin_read)    (uBit16 port,uBit16 pin),
                        void    (*delay_us)          (uBit16 us),
                        void    (*delay_ms)          (uBit16 ms))
{
    w5500_dev.w5500_spi_init = w5500_spi_init;
    w5500_dev.w5500_spi_read = w5500_spi_read;
    w5500_dev.w5500_spi_write = w5500_spi_write;
    
    w5500_dev.w5500_pin_init= w5500_pin_init;
    w5500_dev.w5500_pin_read = w5500_pin_read;
    w5500_dev.w5500_pin_write = w5500_pin_write;
    
    w5500_dev.delay_us = delay_us;
    w5500_dev.delay_ms = delay_ms;
}

//w5500相关引脚初始化
void w5500_hw_init(void)
{
    w5500_dev.w5500_spi_init();

    w5500_dev.w5500_pin_init(w5500_pin_table[0].port,w5500_pin_table[0].pin,w5500_pin_table[0].mode);
    w5500_dev.w5500_pin_write(w5500_pin_table[0].port,w5500_pin_table[0].pin,1);

    w5500_dev.w5500_pin_init(w5500_pin_table[1].port,w5500_pin_table[1].pin,w5500_pin_table[1].mode);
    w5500_dev.w5500_pin_write(w5500_pin_table[1].port,w5500_pin_table[1].pin,1);
}

//w5500复位
void w5500_reset(void)
{
    w5500_dev.w5500_pin_write(w5500_pin_table[0].port,w5500_pin_table[0].pin,0);
    w5500_dev.delay_us(2);
    w5500_dev.w5500_pin_write(w5500_pin_table[0].port,w5500_pin_table[0].pin,1);
    w5500_dev.delay_ms(1600);
}

//w5500配置MAC地址
void w5500_set_mac(void)
{
    memcpy(config_msg.mac,mac,6);
    setSHAR(config_msg.mac);
}

//w5500配置IP地址
void w5500_set_ip(void)
{
    //复制定义的配置信息到配置结构体
    memcpy(config_msg.lip,local_ip,4);
    memcpy(config_msg.sub,subnet,4);
    memcpy(config_msg.gw,gateway,4);
    memcpy(config_msg.dns,dns_server,4);
    
    //将IP配置信息写入w5500寄存器
    setSUBR(config_msg.sub);
    setGAR(config_msg.gw);
    setSIPR(config_msg.lip);
    
    //读取w5500寄存器,判断配置是否正确
    getSUBR(subnet);
    getGAR(gateway);
    getSIPR(local_ip);
}

/*w5500应用接口*/
//--------------------------------//

//w5500初始化
void w5500_init(void)
{
    w5500_hw_init();
    w5500_reset();
    w5500_set_mac();
    w5500_set_ip();
    
    socket_buf_init(txsize, rxsize);
}
