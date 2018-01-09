#ifndef __GCP_HARDWARE_DEF_H
#define __GCP_HARDWARE_DEF_H

#include "VD_MachineIO.h"


/*****************************************************************************
 * 系统硬件资源参数
 ****************************************************************************/

//定义IO资源表
#define g_GcpIOTable                        g_VD1768_IOTable            //使用图书机V3(1768)的IO资源表

//定义CAN总线节点
#define GCP_CAN_DEF_NODE                    (0)                         //默认CAN节点
#define GCP_CAN_DEF_BAUDRATE                (500000)                    //默认CAN波特率

//定义温度传感器ADC采集通道            
#define GCP_TEMP_SAMPLING_CH                (3)                         //温度采集通道(ADC通道)
#define GCP_TEMP_SAMPLING_CH_MASK           (0x1<<GCP_TEMP_SAMPLING_CH) //温度采集通道掩码(ADC通道掩码)
    
//定义货道电机资源  
#define GCP_AISLE_MAX_ROW                   (10)                        //货道最大行数
#define GCP_AISLE_MAX_COL                   (10)                        //货道最大列数
    
//设置升降电机限位信号电平          
#define GCP_LIFT_MOTOR_IDLE_LEVEL           (false)                     //升降电机空闲电平
#define GCP_LIFT_MOTOR_TRIGGER_LEVEL        (true)                      //升降电机触发电平

#endif /* __GCP_HARDWARE_DEF_H */
