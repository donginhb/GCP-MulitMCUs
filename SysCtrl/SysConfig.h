#ifndef __SYS_CONFIG_H
#define __SYS_CONFIG_H

/*****************************************************************************
 * 用户自定义模块配置
 ****************************************************************************/

#ifdef BOOTLOADER   //假如定义了Bootloader

#define SYS_BOOT_USAGE                  (1)     //使用Bootloader
#define SYS_SIMP_CMU_USAGE              (1)     //使用精简版本的CMU
#define SYS_FULL_CMU_USAGE              (0)     //使用完全版本的CMU
#define SYS_CNC_USAGE                   (0)     //使用CNC模块
#define SYS_PARM_MAN_USAGE              (1)     //使用系统参数
#define SYS_GPIO_SCAN_GROUP             (128)   //最大支持的IO扫描组
#define SYS_GPIO_OUTPUT_SCAN_USAGE      (0)     //使用输出IO扫描

#else 

#if defined(LPC43XX)

#define SYS_BOOT_USAGE                  (0)     //使用Bootloader
#define SYS_SIMP_CMU_USAGE              (0)     //使用精简版本的CMU
#define SYS_FULL_CMU_USAGE              (1)     //使用完全版本的CMU
#define SYS_CNC_USAGE                   (0)     //使用CNC模块

#define SYS_PARM_MAN_USAGE              (1)     //使用系统参数
#define SYS_GPIO_SCAN_GROUP             (128)   //最大支持的IO扫描组
#define SYS_GPIO_OUTPUT_SCAN_USAGE      (1)     //使用输出IO扫描
#define SYS_CAN_TX_BUFF_LEN             (32)    //设置CAN发送缓存区长度
#define SYS_CAN_RX_BUFF_LEN             (32)    //设置CAN接收缓存区长度

#elif defined(LPC17XX)

#define SYS_BOOT_USAGE                  (0)     //使用Bootloader
#define SYS_SIMP_CMU_USAGE              (0)     //使用精简版本的CMU
#define SYS_FULL_CMU_USAGE              (1)     //使用完全版本的CMU
#define SYS_CNC_USAGE                   (1)     //使用CNC模块
#define SYS_PARM_MAN_USAGE              (1)     //使用系统参数
#define SYS_GPIO_SCAN_GROUP             (128)   //最大支持的IO扫描组
#define SYS_GPIO_OUTPUT_SCAN_USAGE      (1)     //使用输出IO扫描
#define SYS_CAN_TX_BUFF_LEN             (16)    //设置CAN发送缓存区长度
#define SYS_CAN_RX_BUFF_LEN             (16)    //设置CAN接收缓存区长度

#elif defined(STM32F10X)

#define SYS_BOOT_USAGE                  (0)     //使用Bootloader
#define SYS_SIMP_CMU_USAGE              (0)     //使用精简版本的CMU
#define SYS_FULL_CMU_USAGE              (0)     //使用完全版本的CMU
#define SYS_CNC_USAGE                   (0)     //使用CNC模块
#define SYS_PARM_MAN_USAGE              (0)     //使用系统参数
#define SYS_GPIO_SCAN_GROUP             (64)    //最大支持的IO扫描组
#define SYS_GPIO_OUTPUT_SCAN_USAGE      (1)     //使用输出IO扫描
#define SYS_CAN_TX_BUFF_LEN             (16)    //设置CAN发送缓存区长度
#define SYS_CAN_RX_BUFF_LEN             (16)    //设置CAN接收缓存区长度

#elif defined(STM32F0XX)

#define SYS_BOOT_USAGE                  (0)     //使用Bootloader
#define SYS_SIMP_CMU_USAGE              (0)     //使用精简版本的CMU
#define SYS_FULL_CMU_USAGE              (0)     //使用完全版本的CMU
#define SYS_CNC_USAGE                   (0)     //使用CNC模块
#define SYS_PARM_MAN_USAGE              (0)     //使用系统参数
#define SYS_GPIO_SCAN_GROUP             (32)    //最大支持的IO扫描组
#define SYS_GPIO_OUTPUT_SCAN_USAGE      (0)     //使用输出IO扫描
#define SYS_CAN_TX_BUFF_LEN             (2)     //设置CAN发送缓存区长度
#define SYS_CAN_RX_BUFF_LEN             (2)     //设置CAN接收缓存区长度

#endif 

#endif //#ifdef BOOTLOADER ... #else ...


/*****************************************************************************
 * 默认参数定义
 ****************************************************************************/


#ifndef SYS_PARM_MAN_USAGE
#define SYS_PARM_MAN_USAGE              (0)         //是否使用系统参数管理
#endif


#ifndef SYS_GPIO_SCAN_GROUP 
#define SYS_GPIO_SCAN_GROUP             (32)        //最大支持的IO扫描组(需是32的倍数)
#endif


#ifndef SYS_GPIO_OUTPUT_SCAN_USAGE
#define SYS_GPIO_OUTPUT_SCAN_USAGE      (0)         //是否使能输出IO的信号检测
#endif


#ifndef SYS_KEY_SCAN_GROUP
#define SYS_KEY_SCAN_GROUP              (4)         //最大支持扫描的按键
#endif  

#ifndef SYS_CAN_TX_BUFF_LEN
#define SYS_CAN_TX_BUFF_LEN             (8)         //CAN发送缓存区的长度
#endif

#ifndef SYS_CAN_RX_BUFF_LEN
#define SYS_CAN_RX_BUFF_LEN             (8)         //CAN接收缓存区的长度
#endif



/*****************************************************************************
 * 默认配置定义及配置校验
 ****************************************************************************/

#ifndef SYS_SIMP_CMU_USAGE
#define SYS_SIMP_CMU_USAGE      (0)         //使用精简版本的CMU
#endif

#ifndef SYS_FULL_CMU_USAGE
#define SYS_FULL_CMU_USAGE      (0)         //使用完全版本的CMU
#endif

#ifndef SYS_CNC_USAGE
#define SYS_CNC_USAGE           (0)         //使用CNC模块
#endif

#ifndef SYS_BOOT_USAGE
#define SYS_BOOT_USAGE          (0)         //使用Bootloader
#endif

#if (SYS_SIMP_CMU_USAGE&&SYS_FULL_CMU_USAGE)
#error 精简版本的指令与完全版本的CMU,只能选择一个
#endif


/*****************************************************************************
 * CMU相关配置
 ****************************************************************************/

//通信端口节点配置
#define SYS_CMU_DEF_UART_NODE       (0)
#define SYS_CMU_DEF_UART_BAUDRATE   (115200)

#define SYS_CMU_DEF_CAN_NODE        (0)
#define SYS_CMU_DEF_CAN_BAUDRATE    (500000)

#if (SYS_FULL_CMU_USAGE) || (SYS_SIMP_CMU_USAGE)

//通信端口配置
#define SYS_CMU_USING_CAN           (1)     //使用CAN 作为CMU的通信接口
#define SYS_CMU_USING_ENET          (0)     //使用ENET作为CMU的通信接口
#define SYS_CMU_USING_UART          (1)     //使用UART作为CMU的通信接口

#if (SYS_FULL_CMU_USAGE)    //对完整版本的CMU,需要对数据缓冲区配置
#ifndef COM_TRANSMIT_BUF_SIZE
#define COM_TRANSMIT_BUF_SIZE       (1280)  //CMU TX/RX缓冲区大小
#endif
#define SYS_CMU_BUFF_SIZE           (3*COM_TRANSMIT_BUF_SIZE)   //开辟给CMU的BUFF大小,必须是2个COM_TRANSMIT_BUF_SIZE以上
#endif 

#endif //#if (SYS_FULL_CMU_USAGE) || (SYS_SIMP_CMU_USAGE) ....


/*****************************************************************************
 * CNC相关配置
 ****************************************************************************/

#if SYS_CNC_USAGE

#include "SYS_MemoryDef.h"

#if defined(LPC17XX)

//CNC 缓冲区地址定义
#define CNC_SRAM_BUFF_ADDR      ((unsigned long int)AHB_SRAM0_ADDR)
#define CNC_SRAM_BUFF_LEN       (AHB_SRAM0_SIZE)

//外部SRAM使能
#define CNC_USING_EXT_SRAM      (0)

#elif defined(LPC43XX)

//CNC 缓冲区地址定义
#define CNC_SRAM_BUFF_ADDR      (0)
#define CNC_SRAM_BUFF_LEN       (0)
     
//外部SRAM使能
#define CNC_USING_EXT_SRAM      (0)

#elif defined(STM32F10X)

#define CNC_SRAM_BUFF_ADDR      (0)

#elif defined(STM32F0XX)

#define CNC_SRAM_BUFF_ADDR      (0)

#endif



#endif //SYS_CNC_USAGE





#endif /* __SYS_CONFIG_H */
