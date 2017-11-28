
/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：AHB_DataStructDef.h

摘    要：AHB数据结构定义

运行环境： LPC43xx 

修改描述：

当前版本：

修改作者：毛军

修改日期：2015年10月16日
*/

#ifndef AHB_DATA_STRUCT_DEF_H
#define AHB_DATA_STRUCT_DEF_H

#include "..\\DataStructDef\\DataType.h"
#include "..\\DataStructDef\\SYS_DataStructDef.h"
#include "..\\DataStructDef\\CRD_DataStructDef.h"
#include "..\\DataStructDef\\AXIS_DataStructDef.h"

#include "..\\IPO\\IPO_M0_M4\\IPO_DataStructDefPte.h"

/*
//M0月M4数据交换共享内存地址定义
#define AHB_SHARED_MEM_BASE_ADDR                        (0x20000000)        //M4-M0共享内存基地址
#define AHB_SHARED_MEM_SIZE                                (32768)                //定义共享内存大小(32K)
*/

//
typedef struct _AHB_DATA_STRUCT {
    //-------------------------------系统参数--------------------------------
    SYS_CTRL_PARM                SysCtrlParm;
    CRDSYS_AXIS_MAP_TABLE        CrdAxisMapTable[CRDSYS_MAX_COUNT];
    CRDSYS_PARM                    CrdParm[CRDSYS_MAX_COUNT];
    AXIS_PARM                    AxisParm[AXIS_MAX_COUNT];
    //AXIS_PITCH_CMP_PARM            AxisPitchCmpParm[AXIS_MAX_COUNT];



    //-------------------------插补器M0-M4数据交换----------------------------
    IPO_M0_M4_DATA_TRANS IPO_M0_M4_DataTrans;
    
    //-------------------------------------------------------------------------
    //uBit8 SysUpdateDataBuf[SYS_UPDATE_DATA_BUF_LEN];

}AHB_DATA_STRUCT;






















#endif
