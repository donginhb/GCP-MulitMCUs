#ifndef __CN_HARDWARE_DEF_H
#define __CN_HARDWARE_DEF_H

#include "CN4357_IOTable.h"

/*****************************************************************************
 * 系统硬件资源参数
 ****************************************************************************/

//定义IO资源表
#define g_GcpIOTable                    g_CN4357_IOTable    //使用鼎尚1768的IO资源表

#define CN_UART_NODE                   (0)                 //串口节点
#define CN_DELAY_TIIME_NODE            (5)                 //延时定时器节点


#endif /* __CN_HARDWARE_DEF_H */
