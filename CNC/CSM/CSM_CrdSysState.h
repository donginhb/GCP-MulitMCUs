/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：CSM_CrdSyState.h

摘    要：通道状态管理

运行环境：LPC43xx 

修改描述：

当前版本：3.0

修改作者：毛军

修改日期：2015年9月25日
*/

//
#ifndef CSM_CRDSYS_STATE_H
#define CSM_CRDSYS_STATE_H

#include "CNC/DataStructDef/DataType.h"


#ifdef __cplusplus
extern "C" {
#endif

    uBit32 CSM_GetCtrlMode(Bit32 iCrdSysIndex);

    void CSM_SetCrdSysAlarmBit(int iCrdSysIndex, unsigned long ulAlarmBit);

    Bit32 CSM_CheckAxisAlarm(Bit32 iCrdSysIndex);

    //清除程序执行状态
    void CSM_ClearProgExeState(Bit32 iCrdSysIndex);


#ifdef __cplusplus
}
#endif


#endif