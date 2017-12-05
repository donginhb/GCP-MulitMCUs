
/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：MST_TCode.c

摘    要：T代码处理模块

运行环境：LPC43xx 

修改描述：

当前版本：1.0

修改作者：毛军

修改日期：2015年10月12日

*/

#include "MST_MainCtrl.h"

#include "CNC/DataStructDef/MST_DataStructDef.h"
#include "CNC/DataStructDef/CRD_DataStructDef.h"


//M代码执行函数数值，M代码执行完成返回0，否则返回非0
uBit32 (*m_MST_pf_ExeTCode[TCODE_MAX_COUNT])(Bit32 iCrdSysIndex);


//
uBit32 MST_TCodeExeDefault(Bit32 iCrdSysIndex)
{
    return 1;
}


void MST_InitTCodeFun()
{
    int i;

    //
    for (i = 0; i < TCODE_MAX_COUNT; i++)
    {
        m_MST_pf_ExeTCode[i] = MST_TCodeExeDefault;
    }

}


//T代码执行函数数值，T代码执行完成返回0，否则返回非0
uBit32 MST_ExeTCode(Bit32 iCrdSysIndex, uBit16 nTCode)
{
    if (nTCode == 0)
    {
        return 0;
    }

    if (nTCode >= TCODE_MAX_COUNT)                
    {
        MST_SetSysAlarmBit(iCrdSysIndex, CRDSYS_ALARM_INVALID_TCODE);
        return 1;
    }

    return m_MST_pf_ExeTCode[nTCode - 1](iCrdSysIndex);
}

