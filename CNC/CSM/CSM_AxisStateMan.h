/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：CSM_AxisStateMan.h

摘    要：通道轴状态管理

运行环境：LPC43xx 

修改描述：

当前版本：2.0

修改作者：毛军

修改日期：2015年08月13日
*/

//
#ifndef CSM_AXISSTATEMAN_H
#define CSM_AXISSTATEMAN_H

#include "CNC/DataStructDef/DataType.h"

#ifdef __cplusplus
extern "C" {
#endif

    double64 CSM_GetAxisCmdPos(Bit32 iCrdSysIndex, Bit32 iAxisIndex);

#ifdef __cplusplus
}
#endif


#endif