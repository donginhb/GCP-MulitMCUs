/*    
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：

摘    要：

运行环境：IAR6.6

修改描述：

当前版本：1.0 

修改作者： 

修改了内容：

修改日期： 

原 作 者：毛军

完成日期：2015年08月22日        
*/
#ifndef CAN_DEV_DATA_STRUCT_DEF_H_
#define CAN_DEV_DATA_STRUCT_DEF_H_


#include "CNC/DataStructDef/DataType.h"


//#define CAN_DEV_MAX_COUNT            (8)            //CAN总线（单个通道上的）设备的最大数量
//#define CAN_CHANNEL_MAX_COUNT    (2)            //CAN通道的最大数量

//CAN总线驱动外部依赖接口定义--------
typedef struct _can_dev_ex_interface
{
    double64 (*pf_SYS_GetSysTickCount)(void);
}CAN_DEV_EX_INTERFACE;

#endif

