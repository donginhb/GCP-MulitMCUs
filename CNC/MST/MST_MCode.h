
/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：MST_MCode.h

摘    要：M代码处理模块

运行环境：LPC43xx 

修改描述：

当前版本：1.0

修改作者：毛军

修改日期：2015年10月12日

*/

#ifndef MST_MCODE_H
#define MST_MCODE_H

#include "MST_DataStructDefPte.h"
#include "CNC/DataStructDef/DataType.h"
#include "CNC/DataStructDef/MST_DataStructDef.h"
#include "CNC/DataStructDef/IPR_DataStructDef.h"




#ifdef __cplusplus
extern "C" {
#endif

    void MST_InitMCodeFun();

    void MST_MCodeCancel(Bit32 iCrdSysIndex);

    uBit32 MST_ExeMCode(Bit32 iCrdSysIndex, IIP_BUF* pProgData, uBit16 nActionPos);

    void MST_ExeParaMCode(Bit32 iCrdSysIndex);


    uBit32 MST_RecordParaMCode(Bit32 iCrdSysIndex, uBit16 nMCode, MCODE_PARM MCodeParm, uBit32 (*pfMCodeExe)(Bit32 iCrdSysIndex, MCODE_PARM* pMCodeParm, MCODE_EXE_STATE* pExeState));

    
#ifdef __cplusplus
}
#endif



#endif
