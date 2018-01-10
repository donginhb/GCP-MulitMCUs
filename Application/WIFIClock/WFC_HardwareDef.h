#ifndef __WFC_HARDWARE_DEF_H
#define __WFC_HARDWARE_DEF_H

#include "WFC103_IOTable.h"

/*****************************************************************************
 * 系统硬件资源参数
 ****************************************************************************/

//定义IO资源表
#define g_GcpIOTable                    g_WFC103_IOTable    //使用基于WIFI时钟板子的IO资源表

#define WFC_ESP_UART_NODE               (1)                 //用于ESP通信的串口节点
#define WFC_DEBUG_UART_NODE             (2)                 //用于调试的串口节点
#define WFC_DELAY_TIIME_NODE            (2)                 //延时定时器节点

#endif /* __WFC_HARDWARE_DEF_H */
