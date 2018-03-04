#ifndef __BS08M_HARDWARE_DEF_H
#define __BS08M_HARDWARE_DEF_H

#include "BS08M_IOTable.h"

/*****************************************************************************
 * 系统硬件资源参数
 ****************************************************************************/

//定义IO资源表
#define g_GcpIOTable                      g_BS08M_IOTable       //IO资源表

#define BS08M_COM_UART_NODE               (1)                   //用于通信的串口节点
#define BS08M_DELAY_TIIME_NODE            (2)                   //延时定时器节点

#endif /* __BS08M_HARDWARE_DEF_H */
