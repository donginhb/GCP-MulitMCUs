#ifndef __SYS_CONFIG_H
#define __SYS_CONFIG_H

#ifndef BOOTLOADER
#if defined(LPC43XX)
#define SYS_USING_CMU           (1)     //CMU模块使能开关
#define SYS_USING_CNC           (0)     //CNC模块使能开关
#elif defined(LPC17XX)
#define SYS_USING_CMU           (1)     //CMU模块使能开关
#define SYS_USING_CNC           (1)     //CNC模块使能开关
#elif defined(STM32F10X)
#define SYS_USING_CMU           (0)     //CMU模块使能开关
#define SYS_USING_CNC           (0)     //CNC模块使能开关
#endif
#endif

#ifdef BOOTLOADER
#define SYS_USING_BOOT          (1)
#endif


#ifndef SYS_USING_CMU
#define SYS_USING_CMU           (0)
#endif

#ifndef SYS_USING_CNC
#define SYS_USING_CNC           (0)
#endif

#ifndef SYS_USING_BOOT
#define SYS_USING_BOOT          (0)
#endif


/*****************************************************************************
 * CMU相关配置
 ****************************************************************************/

#if SYS_USING_CMU

//通信端口配置
#define SYS_CMU_USING_CAN       (0)     //使用CAN 作为CMU的通信接口
#define SYS_CMU_USING_ENET      (0)     //使用ENET作为CMU的通信接口
#define SYS_CMU_USING_UART      (1)     //使用UART作为CMU的通信接口
#if ((SYS_CMU_USING_CAN + SYS_CMU_USING_ENET + SYS_CMU_USING_UART) != 1)
#err There is no clear communication port   //只能选择一个通信接口作为CMU的接口
#endif

//通信端口节点配置
#if (SYS_CMU_USING_CAN || SYS_CMU_USING_UART)
#define SYS_CMU_COM_NODE        (0)
#endif

//数据缓冲区配置
#ifndef COM_TRANSMIT_BUF_SIZE
#define COM_TRANSMIT_BUF_SIZE   (1280)  //CMU TX/RX缓冲区大小
#endif
#define SYS_CMU_BUFF_SIZE       (3*COM_TRANSMIT_BUF_SIZE)   //开辟给CMU的BUFF大小,必须是2个COM_TRANSMIT_BUF_SIZE以上


#endif //SYS_USING_CMU


/*****************************************************************************
 * CNC相关配置
 ****************************************************************************/

#if SYS_USING_CNC

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

#endif




#endif //SYS_USING_CNC





#endif /* __SYS_CONFIG_H */
