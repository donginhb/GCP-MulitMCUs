#ifndef __BS05M_HARDWARE_DEF_H
#define __BS05M_HARDWARE_DEF_H

#include "BS05M_IOTable.h"

/*****************************************************************************
 * 系统硬件资源参数
 ****************************************************************************/

//定义IO资源表
#define g_GcpIOTable                        g_BS05M_IOTable      //IO资源表

#define BS05M_COM_UART_NODE                 (0)                 //用于通信的串口节点
#define BS05M_LIGHT_ADC_NODE                (0)                 //用于光照检测的ADC节点
#define BS05M_LIGHT_ADC_CH                  (0)                 //用于光照检测的ADC通道

#define BS05M_MOTOR_PWM_NODE                (0)                 //PWM输出节点
#define BS05M_MOTOR_PWM_CH_MASK             (0x08)              //PWM输出通道掩码


#endif /* __BS05M_HARDWARE_DEF_H */
