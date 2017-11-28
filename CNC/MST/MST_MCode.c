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
#include <string.h>

#include "MST_MainCtrl.h"
#include "MST_DataStructDefPte.h"
#include "MST_MCode.h"
#include "MST_Interface.h"

#include "..\\DataStructDef\\MST_DataStructDef.h"
#include "..\\DataStructDef\\SYS_DataStructDef.h"
#include "..\\DataStructDef\\CRD_DataStructDef.h"

//
extern MST_INTERFACE    m_MSTInterface;


//M代码执行函数数值，M代码执行完成返回0，否则返回非0
uBit32 (*m_MST_pf_ExeMCode[MCODE_MAX_COUNT])(Bit32 iCrdSysIndex, MCODE_PARM MCodeParm);

PARA_MCODE_MAN    m_ParaMCodeMan[CRDSYS_MAX_COUNT];


//
uBit32 MST_MCodeExeDefault(Bit32 iCrdSysIndex, MCODE_PARM MCodeParm)
{
    return 0;
}

//M02-程序结束
uBit32 MST_M02(Bit32 iCrdSysIndex, MCODE_PARM MCodeParm)
{
    return 0;
}

//
uBit32 MST_M03(Bit32 iCrdSysIndex, MCODE_PARM MCodeParm)
{
    return 0;
}

//
uBit32 MST_M04(Bit32 iCrdSysIndex, MCODE_PARM MCodeParm)
{
    return 0;
}

//
uBit32 MST_M07(Bit32 iCrdSysIndex, MCODE_PARM MCodeParm)
{
    return 0;
}


//M30-程序结束并返回程序开头
uBit32 MST_M30(Bit32 iCrdSysIndex, MCODE_PARM MCodeParm)
{
    return 0;
}


//M999-IO操作M代码执行，执行完成返回0，否则返回非0值
uBit32 MST_M999_Exe(Bit32 iCrdSysIndex, MCODE_PARM* pMCodeParm, MCODE_EXE_STATE* pExeState)
{
    uBit32 ulStatus;

    switch (pExeState->nExeStep)
    {
        case 1:
            if (m_MSTInterface.pf_IO_SetOutputStatus(pMCodeParm->ulParm1, pMCodeParm->ulParm3, pMCodeParm->ulParm2))
            {
                MST_SetSysAlarmBit(iCrdSysIndex, CRDSYS_ALARM_MCODE_EXE_FAIL);
                return 1;
            }
            else
            {
                if (pMCodeParm->ulParm4 > 0)
                {
                    pExeState->dTime = m_MSTInterface.pf_SYS_GetSysTickCount();
                    pExeState->nExeStep = 2;
                    return 2;
                }
                else
                {
                    return 0;
                }
            }
            break;
        case 2:
            if (m_MSTInterface.pf_SYS_GetSysTickCount() - pExeState->dTime >= pMCodeParm->ulParm4 )
            {
                ulStatus = (~pMCodeParm->ulParm3) & pMCodeParm->ulParm2;

                if (m_MSTInterface.pf_IO_SetOutputStatus(pMCodeParm->ulParm1, ulStatus, pMCodeParm->ulParm2))
                {
                    MST_SetSysAlarmBit(iCrdSysIndex, CRDSYS_ALARM_MCODE_EXE_FAIL);
                    return 1;
                }
                else
                {
                    pExeState->nExeStep = 3;
                    return 0;
                }
            }
            break;
        default:
            break;
    }

    return 0;
}

//M999-IO操作M代码, 成功返回0，否则返回非0值
uBit32 MST_M999(Bit32 iCrdSysIndex, MCODE_PARM MCodeParm)
{
    return MST_RecordParaMCode(iCrdSysIndex, M_CODE_IO_SET, MCodeParm, MST_M999_Exe);
}


void MST_InitMCodeFun()
{
    int i;

    //
    memset(m_ParaMCodeMan, 0, sizeof(m_ParaMCodeMan));

    //
    for (i = 0; i < MCODE_MAX_COUNT; i++)
    {
        m_MST_pf_ExeMCode[i] = MST_MCodeExeDefault;
    }

    //
    m_MST_pf_ExeMCode[1] = MST_M02;
    m_MST_pf_ExeMCode[2] = MST_M03;
    m_MST_pf_ExeMCode[3] = MST_M04;
    m_MST_pf_ExeMCode[6] = MST_M07;

    m_MST_pf_ExeMCode[29] = MST_M30;

}

//取消正在执行的并行M代码
void MST_MCodeCancel(Bit32 iCrdSysIndex)
{
    memset(m_ParaMCodeMan + iCrdSysIndex, 0, sizeof(PARA_MCODE_MAN));
}


//M代码执行函数数值，M代码执行完成返回0，否则返回非0
//iActionPos: 0-前作用M代码，1-后作用M代码
uBit32 MST_ExeMCode(Bit32 iCrdSysIndex, IIP_BUF* pProgData, uBit16 nActionPos)
{
    if (pProgData->nMCode == 0)
    {
        return 0;
    }

    if (pProgData->nMCode >= MCODE_MAX_COUNT && pProgData->nMCode != M_CODE_IO_SET)                
    {
        MST_SetSysAlarmBit(iCrdSysIndex, CRDSYS_ALARM_INVALID_MCODE);
        return 1;
    }

    //
    if (nActionPos == 0)
    { //前作用M代码
        if (pProgData->nMCode == 3                    //主轴正转
            || pProgData->nMCode == 4                //主轴反转
            || pProgData->nMCode == 7)                //冷却液打开
        {
            return m_MST_pf_ExeMCode[pProgData->nMCode - 1](iCrdSysIndex, pProgData->Data.MCodeParm);
        }
        else
        {
            return 0;
        }
    }
    else
    {
        if (pProgData->nMCode == M_CODE_IO_SET)
        {
            return MST_M999(iCrdSysIndex, pProgData->Data.MCodeParm);
        }
        else
        {
            return m_MST_pf_ExeMCode[pProgData->nMCode - 1](iCrdSysIndex, pProgData->Data.MCodeParm);
        }
    }
}


//记录并行M代码，成功返回0，否则返回非0值
//说明：调用m_MST_pf_ExeMCode执行M代码时，如果是并行M代码，则在并行M代码函数体中调用MST_RecordParaMCode即可
uBit32 MST_RecordParaMCode(Bit32 iCrdSysIndex, uBit16 nMCode, MCODE_PARM MCodeParm, uBit32 (*pfMCodeExe)(Bit32 iCrdSysIndex, MCODE_PARM* pMCodeParm, MCODE_EXE_STATE* pExeState))
{
    PARA_MCODE_MAN* pParaMCodeMan;
    int i;

    pParaMCodeMan = m_ParaMCodeMan + iCrdSysIndex;

    if (pParaMCodeMan->nCount >= PARA_MCODE_MAX_COUNT)
    {
        MST_SetSysAlarmBit(iCrdSysIndex, CRDSYS_ALARM_TOO_MANY_PARA_MCODE);
        return 1;
    }

    /*
    pParaMCodeMan->MCode[pParaMCodeMan->nWritePos].nMCode = nMCode;
    pParaMCodeMan->MCode[pParaMCodeMan->nWritePos].nExeStep = 1;
    pParaMCodeMan->nCount++;
    pParaMCodeMan->nWritePos++;
    if (pParaMCodeMan->nWritePos >= PARM_MCODE_MAX_COUNT)
    {
        pParaMCodeMan->nWritePos = 0;
    }
    */
    for (i = 0; i < PARA_MCODE_MAX_COUNT; i++)
    {
        if (pParaMCodeMan->MCode[i].nValid == 0)
        {
            pParaMCodeMan->MCode[i].nMCode = nMCode;
            pParaMCodeMan->MCode[i].MCodeParm = MCodeParm;
            pParaMCodeMan->MCode[i].MCodeExeState.nExeStep = 1;
            pParaMCodeMan->MCode[i].MCodeExeState.dTime = 0.0;
            pParaMCodeMan->MCode[i].pfMCodeExe = pfMCodeExe;
            pParaMCodeMan->MCode[i].nValid = 1;
            pParaMCodeMan->nCount++;
            break;
        }
    }

    return 0;
}

//
void MST_ExeParaMCode(Bit32 iCrdSysIndex)
{
    PARA_MCODE_MAN* pParaMCodeMan;
    int i;

    //
    pParaMCodeMan = m_ParaMCodeMan + iCrdSysIndex;

    for (i = 0; i < PARA_MCODE_MAX_COUNT; i++)
    {
        if (pParaMCodeMan->MCode[i].nValid == 1)
        {
            //执行M代码
            if (!pParaMCodeMan->MCode[i].pfMCodeExe(i, &(pParaMCodeMan->MCode[i].MCodeParm), &(pParaMCodeMan->MCode[i].MCodeExeState)))
            {
                //执行完成
                pParaMCodeMan->MCode[i].nValid = 0;
                pParaMCodeMan->MCode[i].MCodeExeState.nExeStep = 0;
                pParaMCodeMan->nCount--;
            }

        }
    }
}