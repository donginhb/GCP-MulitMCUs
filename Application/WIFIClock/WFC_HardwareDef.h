#ifndef __WFC_HARDWARE_DEF_H
#define __WFC_HARDWARE_DEF_H

#include "WFC103_IOTable.h"

/*****************************************************************************
 * 系统硬件资源参数
 ****************************************************************************/

//定义IO资源表
#define g_GcpIOTable                    g_WFC103_IOTable    //使用鼎尚1768的IO资源表

#define WFC_UART_NODE                   (0)                 //串口节点
#define WFC_DELAY_TIIME_NODE            (5)                 //延时定时器节点


#endif /* __WFC_HARDWARE_DEF_H */
