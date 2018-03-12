#ifndef __WB01_HARDWARE_DEF_H
#define __WB01_HARDWARE_DEF_H

#include "WB01_IOTable.h"

/*****************************************************************************
 * 系统硬件资源参数
 ****************************************************************************/

//定义IO资源表
#define g_GcpIOTable                      g_WB01_IOTable       //IO资源表

#define WB01_COM_UART_NODE               (0)                   //用于通信的串口节点
#define WB01_DEBUG_UART_NODE             (1)                   //用于调试的串口节点

#define WB01_DELAY_TIIME_NODE            (2)                   //延时定时器节点

#endif /* __WB01_HARDWARE_DEF_H */
