#ifndef __VM_HARDWARE_DEF_H
#define __VM_HARDWARE_DEF_H

#include "VD_MachineIO.h"


/*****************************************************************************
 * 系统硬件资源参数
 ****************************************************************************/

//定义IO资源表
#define g_GcpIOTable                    g_VD1768_IOTable    //使用图书机V3(1768)的IO资源表


//定义货道电机资源
#define VM_AISLE_MAX_ROW                (10)                //货道最大行数
#define VM_AISLE_MAX_COL                (10)                //货道最大列数

#define VM_LIFT_MOTOR_IDLE_LEVEL        (false)             //升降电机空闲电平
#define VM_LIFT_MOTOR_TRIGGER_LEVEL     (true)              //升降电机触发电平




#endif /* __VM_HARDWARE_DEF_H */
