#ifndef __MD_HARDWARE_DEF_H
#define __MD_HARDWARE_DEF_H

#include "MD103_IODef.h"


/*****************************************************************************
 * 系统硬件资源参数
 ****************************************************************************/

//定义IO资源表
#define g_GcpIOTable                        g_MD103_IOTable            //使用图书机V3(1768)的IO资源表

//定义CAN总线节点
#define MD_CAN_DEF_NODE                    (0)                         //默认CAN节点
#define MD_CAN_DEF_BAUDRATE                (500000)                    //默认CAN波特率



#endif /* __MD_HARDWARE_DEF_H */
