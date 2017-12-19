#ifndef __SYS_CONFIG_H
#define __SYS_CONFIG_H

/*****************************************************************************
 * 用户自定义模块配置
 ****************************************************************************/

#ifdef BOOTLOADER   //假如定义了Bootloader

#define SYS_USING_BOOT          (1)     //使用Bootloader
#define SYS_USING_SIMP_CMU      (1)     //使用精简版本的CMU
#define SYS_USING_FULL_CMU      (0)     //使用完全版本的CMU
#define SYS_USING_CNC           (0)     //使用CNC模块

#else 

#if defined(LPC43XX)

#define SYS_USING_BOOT          (0)     //使用Bootloader
#define SYS_USING_SIMP_CMU      (0)     //使用精简版本的CMU
#define SYS_USING_FULL_CMU      (1)     //使用完全版本的CMU
#define SYS_USING_CNC           (0)     //使用CNC模块

#elif defined(LPC17XX)

#define SYS_USING_BOOT          (0)     //使用Bootloader
#define SYS_USING_SIMP_CMU      (0)     //使用精简版本的CMU
#define SYS_USING_FULL_CMU      (1)     //使用完全版本的CMU
#define SYS_USING_CNC           (1)     //使用CNC模块

#elif defined(STM32F10X)

#define SYS_USING_BOOT          (0)     //使用Bootloader
#define SYS_USING_SIMP_CMU      (0)     //使用精简版本的CMU
#define SYS_USING_FULL_CMU      (0)     //使用完全版本的CMU
#define SYS_USING_CNC           (0)     //使用CNC模块

#endif 

#endif //#ifdef BOOTLOADER ... #else ...


/*****************************************************************************
 * 默认配置定义及配置校验
 ****************************************************************************/

#ifndef SYS_USING_SIMP_CMU
#define SYS_USING_SIMP_CMU      (0)         //使用精简版本的CMU
#endif

#ifndef SYS_USING_FULL_CMU
#define SYS_USING_FULL_CMU      (0)         //使用完全版本的CMU
#endif

#ifndef SYS_USING_CNC
#define SYS_USING_CNC           (0)         //使用CNC模块
#endif

#ifndef SYS_USING_BOOT
#define SYS_USING_BOOT          (0)         //使用Bootloader
#endif

#if (SYS_USING_SIMP_CMU&&SYS_USING_FULL_CMU)
#error 精简版本的指令与完全版本的CMU,只能选择一个
#endif


/*****************************************************************************
 * CMU相关配置
 ****************************************************************************/

#if (SYS_USING_FULL_CMU) || (SYS_USING_SIMP_CMU)

//通信端口配置
#define SYS_CMU_USING_CAN       (0)     //使用CAN 作为CMU的通信接口
#define SYS_CMU_USING_ENET      (0)     //使用ENET作为CMU的通信接口
#define SYS_CMU_USING_UART      (1)     //使用UART作为CMU的通信接口

#if ((SYS_CMU_USING_CAN + SYS_CMU_USING_ENET + SYS_CMU_USING_UART) != 1)
#error 只能选择一个通信接口作为CMU的接口
#endif

//通信端口节点配置
#if (SYS_CMU_USING_CAN || SYS_CMU_USING_UART)
#define SYS_CMU_COM_NODE        (0)
#endif

#if (SYS_USING_FULL_CMU)    //对完整版本的CMU,需要对数据缓冲区配置
#ifndef COM_TRANSMIT_BUF_SIZE
#define COM_TRANSMIT_BUF_SIZE   (1280)  //CMU TX/RX缓冲区大小
#endif
#define SYS_CMU_BUFF_SIZE       (3*COM_TRANSMIT_BUF_SIZE)   //开辟给CMU的BUFF大小,必须是2个COM_TRANSMIT_BUF_SIZE以上
#endif 

#endif //#if (SYS_USING_FULL_CMU) || (SYS_USING_SIMP_CMU) ....


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
