#ifndef __GCP_HARDWARE_DEF_H
#define __GCP_HARDWARE_DEF_H

#include "DS_IODef.h"
//#include "VD_MachineIO.h"

/*****************************************************************************
 * 系统硬件资源参数
 ****************************************************************************/

//定义IO资源表
#define g_GcpIOTable                        g_DS1768_IOTable           
//#define g_GcpIOTable                        g_VD1768_IOTable 

//定义CAN总线节点
#define GCP_CAN_DEF_NODE                    (0)                         //默认CAN节点
#define GCP_CAN_DEF_BAUDRATE                (500000)                    //默认CAN波特率



#endif /* __GCP_HARDWARE_DEF_H */
