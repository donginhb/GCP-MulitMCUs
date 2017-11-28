#ifndef  __W5500_CONF_H
#define  __W5500_CONF_H

#include "../../DataType/DataType.h"

#define HIGH	           	 			1
#define LOW		             			0


//w5500 设备函数指针
typedef struct
{
    void    (*w5500_spi_init)   (void);
    void    (*w5500_spi_write)  (uBit8 data);
    uBit8   (*w5500_spi_read)   (void);
    void    (*w5500_pin_init)   (uBit16 port,uBit16 pin,uBit8 mode);
    void    (*w5500_pin_write)  (uBit16 port,uBit16 pin,uBit8 state);
    uBit8   (*w5500_pin_read)   (uBit16 port,uBit16 pin);
    void    (*delay_us)         (uBit16 us);
    void    (*delay_ms)         (uBit16 ms);
}W5500_Dev;

//w5500 引脚表
typedef struct 
{
    uBit16 port;
    uBit16 pin;
    uBit8 mode;
}W5500_Pin_Table;

//w5500 配置参数
typedef struct 
{
    uBit8 mac[6];//MAC地址
    uBit8 lip[4];//本地IP
    uBit8 sub[4];//子网掩码
    uBit8 gw[4];//网关
    uBit8 dns[4];//DNS服务器地址
    uBit8 rip[4];//远程IP
    uBit8 sw_ver[2];
}Config_Msg;


/*外部引用声明*/
extern W5500_Dev w5500_dev;//w5500设备

extern W5500_Pin_Table w5500_pin_table[3];//w5500引脚表

extern uBit8 mac[6];//定义MAC地址，如果多块w5500网络芯片工作在同一现场，请使用不同的MAC地址
extern uBit8 local_ip[4];//w5500默认IP地址
extern uBit8 subnet[4];//w5500默认子网掩码
extern uBit8 gateway[4];//w5500默认网关
extern uBit8 dns_server[4];//w5500默认DNS

extern uBit16 local_port;//w5500本地端口号

extern uBit8 remote_ip[4];//远端IP地址
extern uBit16 remote_port;//远端端口号


/*函数声明*/
//w5500设备注册
void w5500_dev_register(void    (*w5500_spi_init)    (void),
                        void    (*w5500_spi_write)   (uBit8 data),
                        uBit8   (*w5500_spi_read)    (void),
                        void    (*w5500_pin_init)    (uBit16 port,uBit16 pin,uBit8 mode),
                        void    (*w5500_pin_write)   (uBit16 port,uBit16 pin,uBit8 state),
                        uBit8   (*w5500_pin_read)    (uBit16 port,uBit16 pin),
                        void    (*delay_us)          (uBit16 us),
                        void    (*delay_ms)          (uBit16 ms));

//w5500初始化
void w5500_init(void);


#endif
