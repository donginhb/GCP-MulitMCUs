#ifndef __GCP_HARDWARE_DEF_H
#define __GCP_HARDWARE_DEF_H

#include "DS1768_IOTable.h"

/*****************************************************************************
 * 系统硬件资源参数
 ****************************************************************************/

//定义IO资源表
#define g_GcpIOTable                    g_DS1768_IOTable    //使用鼎尚1768的IO资源表

#define GCP_UART_NODE                   (0)                 //串口节点
#define GCP_DELAY_TIIME_NODE            (5)                 //延时定时器节点


#endif /* __GCP_HARDWARE_DEF_H */
