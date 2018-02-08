#ifndef __CN_HARDWARE_DEF_H
#define __CN_HARDWARE_DEF_H

#include "CN4357_IOTable.h"
//#include "YY4357_IODef.h"


/*****************************************************************************
 * 系统硬件资源参数
 ****************************************************************************/

//定义IO资源表
#define g_GcpIOTable                    g_CN4357_IOTable    
//#define g_GcpIOTable                    g_YY4357_IOTable    


#define CN_UART_NODE                    (0)                 //串口节点
#define CN_DELAY_TIIME_NODE             (5)                 //延时定时器节点

#define CN_TEMP_SAMPLING_CH1            (0)                 //温度采集通道(ADC通道)
#define CN_TEMP_SAMPLING_CH2            (1)                 //温度采集通道(ADC通道)
#define CN_TEMP_SAMPLING_CH_MASK        ((0x1<<CN_TEMP_SAMPLING_CH1) | (0x1<<CN_TEMP_SAMPLING_CH2)) //温度采集通道掩码(ADC通道掩码)


#endif /* __CN_HARDWARE_DEF_H */
