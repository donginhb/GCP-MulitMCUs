#ifndef __BS08S_HARDWARE_DEF_H
#define __BS08S_HARDWARE_DEF_H

#include "BS08S_IOTable.h"

/*****************************************************************************
 * 系统硬件资源参数
 ****************************************************************************/

//定义IO资源表
#define g_GcpIOTable                      g_BS08S_IOTable       //使用基于WIFI时钟板子的IO资源表

#define BS08S_COM_UART_NODE               (1)                   //用于通信的串口节点
#define BS08S_DELAY_TIIME_NODE            (2)                   //延时定时器节点

#endif /* __BS08S_HARDWARE_DEF_H */
