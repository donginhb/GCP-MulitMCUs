/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：CSM_DataStructDef.h 

摘    要：通道管理内部数据结构定义

运行环境：LPC43xx

修改描述：

当前版本：1.0

修改作者：毛军

修改日期：2015年10月08日

*/

#ifndef CSM_DATA_STRUCT_DEFINE_H
#define CSM_DATA_STRUCT_DEFINE_H

#include "CNC/DataStructDef/DataType.h"
#include "CNC/DataStructDef/SYS_ConfigDef.h"

#pragma pack(push)
#pragma pack(8)        

//
#define CSM_AUTO_EXE_END                                    (0)                //通道自动模式-程序运行结束
#define CSM_AUTO_EXE_GET_PROG_DATA                            (1)                //通道自动模式-获取程序段数据
#define CSM_AUTO_EXE_WAIT_G_CODE_FINISH                        (2)                //通道自动模式-等待G代码执行完成
#define CSM_AUTO_EXE_PROCESS_BEFORE_ACTION_MST                (3)                //通道自动模式-处理前作用MST
#define CSM_AUTO_EXE_PROCESS_G_CODE                            (4)                //通道自动模式-处理G代码
#define CSM_AUTO_EXE_WAIT_CUR_LINE_G_CODE_FINISH            (5)                //通道自动模式-等待当前行G代码执行完成
#define CSM_AUTO_EXE_PROCESS_AFTER_ACTION_MST                (6)                //通道自动模式-处理后作用MST

//
typedef struct _CSM_AUTO_CTRL_DATA {
    uBit16    nExeStep;

    uBit32  ulCurProcessLineNO;                //当前处理的程序行号
    uBit32  ulReleaseLineNO;                //已释放的程序行号

    double64    dCrd[CRDSYS_AXIS_MAX_COUNT];        //当前处理的程序行终点坐标（轴绝对坐标），单位：um

}CSM_AUTO_CTRL_DATA;

#pragma pack(pop)

#endif