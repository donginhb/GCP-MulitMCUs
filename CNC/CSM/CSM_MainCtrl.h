/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：CSM_MainCtrl.h

摘    要：通道管理主控模块

运行环境：LPC43xx 

修改描述：

当前版本：1.0

修改作者：毛军

修改日期：2015年9月27日

使用说明：

*/

#ifndef CSM_MAINCTRL_H
#define CSM_MAINCTRL_H

#include "CSM_DataStructDef.h"

#include "CNC/DataStructDef/SYS_DataStructDef.h"
#include "CNC/DataStructDef/CRD_DataStructDef.h"
#include "CNC/DataStructDef/IPR_DataStructDef.h"



extern CRDSYS_STATE_DATA    m_CrdSysStateData[CRDSYS_MAX_COUNT];
extern CSM_AUTO_CTRL_DATA    m_CSM_AutoCtrl[CRDSYS_MAX_COUNT];
extern IIP_BUF                m_CSM_CurProgData[CRDSYS_MAX_COUNT];
extern uBit32                m_CSM_NCAxisData[CRDSYS_MAX_COUNT]; //Alan modified at 2016-7-22
extern Bit32                m_pIprStartLineCount[CRDSYS_MAX_COUNT];//Alan Added at 2016-7-28
extern Bit32                m_pIprFinishLineCount[CRDSYS_MAX_COUNT];//Alan Added at 2016-7-28




#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif