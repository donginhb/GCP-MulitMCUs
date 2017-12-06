
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

#ifndef __AHB_DATA_STRUCT_DEF_H
#define __AHB_DATA_STRUCT_DEF_H


#include "CNC/DataStructDef/DataType.h"
#include "CNC/DataStructDef/SYS_DataStructDef.h"
#include "CNC/DataStructDef/CRD_DataStructDef.h"
#include "CNC/DataStructDef/AXIS_DataStructDef.h"

#include "../IPO/IPO_DataStructDefPte.h"


typedef struct _AHB_DATA_STRUCT {
    //-------------------------------系统参数--------------------------------
    SYS_CTRL_PARM                SysCtrlParm;
    CRDSYS_AXIS_MAP_TABLE        CrdAxisMapTable[CRDSYS_MAX_COUNT];
    CRDSYS_PARM                    CrdParm[CRDSYS_MAX_COUNT];
    AXIS_PARM                    AxisParm[AXIS_MAX_COUNT];
    //AXIS_PITCH_CMP_PARM            AxisPitchCmpParm[AXIS_MAX_COUNT];
    //-------------------------插补器M0-M4数据交换----------------------------
    IPO_M0_M4_DATA_TRANS IPO_M0_M4_DataTrans;
    
}AHB_DATA_STRUCT;




#endif /* __AHB_DATA_STRUCT_DEF_H */
