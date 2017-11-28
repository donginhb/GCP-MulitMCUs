/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：CSM_ParmMan.h

摘    要：通道系统参数管理

运行环境：LPC43xx 

修改描述：

当前版本：1.0

修改作者：毛军

修改日期：2015年9月25日
*/

//
#ifndef CSM_PARM_MAN_H
#define CSM_PARM_MAN_H


#include "..\\DataStructDef\\DataType.h"
#include "..\\DataStructDef\\SYS_ConfigDef.h"


extern int m_CSM_CrdSysMotorIndex[CRDSYS_MAX_COUNT][AXIS_MAX_COUNT];        //坐标系中的坐标轴对应的电机索引


#ifdef __cplusplus
extern "C" {
#endif
  
    void CSM_ParmManInit();

    unsigned long CSM_InitCrdSysMotorIndex();
    int CSM_GetCrdSysCount();
    int CSM_CheckCrdSysInterrelation();
    unsigned long CSM_GetAllCrdSysMask();
    int CSM_GetCrdSysAxisCount(int iCrdSysIndex);
    int CSM_GetAxisMapMotorNO(int iCrdSysIndex, int iAxisIndex, int iFirstMotor);
    uBit32 CSM_GetAxisMapMotorMask(int iCrdSysIndex, int iAxisIndex);
    uBit32 CSM_GetCrdSysMapMotorMask(int iCrdSysIndex);
    int CSM_GetMotorMapCrdSysIndex(int iMotorNO);


#ifdef __cplusplus
}
#endif


#endif
