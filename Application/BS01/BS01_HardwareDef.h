#ifndef __BS01_HARDWARE_DEF_H
#define __BS01_HARDWARE_DEF_H

#include "BS01_IOTable.h"

/*****************************************************************************
 * 系统硬件资源参数
 ****************************************************************************/

//定义IO资源表
#define g_GcpIOTable                        g_BS01_IOTable      //使用基于WIFI时钟板子的IO资源表

#define BS01_ESP_UART_NODE                  (1)                 //用于ESP通信的串口节点
#define BS01_DELAY_TIIME_NODE               (2)                 //延时定时器节点

#endif /* __BS01_HARDWARE_DEF_H */
