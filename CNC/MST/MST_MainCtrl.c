/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：MST_MainCtrl.c

摘    要：通道自动模式运行控制

运行环境：LPC43xx 

修改描述：

当前版本：1.0

修改作者：毛军

修改日期：2015年10月12日

使用说明：

*/
#include <string.h>

#include "MST_Interface.h"
#include "MST_MCode.h"
#include "MST_TCode.h"
#include "MST_DataStructDefPte.h"

#include "..\\DataStructDef\\IPR_DataStructDef.h"
#include "..\\DataStructDef\\SYS_DataStructDef.h"
#include "..\\DataStructDef\\MST_DataStructDef.h"
#include "..\\DataStructDef\\CRD_DataStructDef.h"

//
extern MST_INTERFACE    m_MSTInterface;

//
MST_CTRL_DATA    m_MSTCtrlData[CRDSYS_MAX_COUNT];
MST_STATE_DATA    m_MSTStateData[CRDSYS_MAX_COUNT];

/*
函 数 名：void CSM_Init()
功    能：初始化通道数据
参    数：无
返 回 值：无
调用位置：系统上电时调用
注意事项：无
*/
void MST_Init()
{
    memset(m_MSTCtrlData, 0, sizeof(m_MSTCtrlData));
    memset(m_MSTStateData, 0, sizeof(m_MSTStateData));

    //
    memset(&m_MSTInterface, 0, sizeof(m_MSTInterface));

    //
    MST_InitMCodeFun();
    MST_InitTCodeFun();
}

//
uBit32 MST_Reset(Bit32 iCrdSysIndex)
{
    MST_CTRL_DATA* pMSTCtrlData = NULL;

    //
    pMSTCtrlData = m_MSTCtrlData + iCrdSysIndex;
    pMSTCtrlData->nExeStep = MST_EXE_END;

    //清除（可清除的）报警
    m_MSTStateData[iCrdSysIndex].ulAlarm &= CRDSYS_ALARM_MST_ERROR;

    //
    MST_MCodeCancel(iCrdSysIndex);
    
    return 0;
}

//
//停止MST代码执行
void MST_Run(uBit16 nCrdSysMask)
{
    int i;

    for (i = 0; i < CRDSYS_MAX_COUNT; i++)
    {
        if (nCrdSysMask & (0x0001 << i))
        {
            m_MSTStateData[i].ulStatus &= ~CRDSYS_STATUS_PROG_MASK;
            m_MSTStateData[i].ulStatus |= CRDSYS_STATUS_PROG_RUN;
        }
    }
}


//停止MST代码执行
void MST_Stop(uBit16 nCrdSysMask)
{
    int i;

    for (i = 0; i < CRDSYS_MAX_COUNT; i++)
    {
        if (nCrdSysMask & (0x0001 << i))
        {
            m_MSTStateData[i].ulStatus &= ~CRDSYS_STATUS_PROG_MASK;
            m_MSTStateData[i].ulStatus |= CRDSYS_STATUS_PROG_STOP;
        }
    }
}

//取消MST代码
void MST_Cancel(uBit16 nCrdSysMask)
{
    int i;

    for (i = 0; i < CRDSYS_MAX_COUNT; i++)
    {
        if (nCrdSysMask & (0x0001 << i))
        {
            m_MSTStateData[i].ulStatus &= ~CRDSYS_STATUS_PROG_MASK;
            m_MSTStateData[i].ulStatus |= CRDSYS_STATUS_PROG_STOP;

            MST_MCodeCancel(i);
        }
    }
}

/*
函 数 名：int MST_CheckBeforeActionMSTExist(IIP_BUF* pProgData)
功    能：检测程序段是否存在前作用MST代码
参    数：pProgData --程序段数据地址
返 回 值：存在前作用MST代码返回1，否则返回0
调用位置：
注意事项：无
*/
Bit32 MST_CheckBeforeActionMSTExist(IIP_BUF* pProgData)
{
    if ( pProgData->nMCode == 3 || pProgData->nMCode ==4 || pProgData->nMCode == 7)
    {
        return 1;
    }

    if (pProgData->nSCode > 0)
    {
        return 1;
    }

    return 0;
}

/*
函 数 名：int CSM_CheckAfterActionMSTExist(IIP_BUF* pProgData)
功    能：检测程序段是否存在后作用MST代码
参    数：pProgData --程序段数据地址
返 回 值：存在后作用MST代码返回1，否则返回0
调用位置：
注意事项：无
*/
Bit32 MST_CheckAfterActionMSTExist(IIP_BUF* pProgData)
{
    if ( pProgData->nMCode > 0 && pProgData->nMCode != 3 && pProgData->nMCode !=4 && pProgData->nMCode != 7)
    {
        return 1;
    }

    if (pProgData->nTCode > 0)
    {
        return 1;
    }

    return 0;
}

//
void MST_SetSysAlarmBit(int iCrdSysIndex, unsigned long ulAlarm)
{
    m_MSTStateData[iCrdSysIndex].ulAlarm |= ulAlarm;
}

//获取MST模块状态数据
uBit32 MST_GetStateData(Bit32 iCrdSysIndex, MST_STATE_DATA* pMSTStateData)
{
    *pMSTStateData = m_MSTStateData[iCrdSysIndex];

    return 0;
}



//处理前作用MST代码，完成返回0，否则返回非0
uBit32 MST_ProcessBeforeActionMST(Bit32 iCrdSysIndex, IIP_BUF* pProgData)
{
    MST_CTRL_DATA* pMSTCtrlData = NULL;

    //
    pMSTCtrlData = m_MSTCtrlData + iCrdSysIndex;

    switch(pMSTCtrlData->nExeStep)
    {
        case MST_EXE_END:
            pMSTCtrlData->nExeStep = MST_EXE_SCODE;
        case MST_EXE_SCODE:
            if (pProgData->nSCode > 0)
            {
                m_MSTStateData[iCrdSysIndex].nSCode = pProgData->nSCode;
            }
            pMSTCtrlData->nExeStep = MST_EXE_MCODE;
        case MST_EXE_MCODE:
            if (pProgData->nMCode > 0)
            {
                if (MST_ExeMCode(iCrdSysIndex, pProgData, 0))
                {
                    return 1;
                }
            }

            //
            pMSTCtrlData->nExeStep = MST_EXE_END;
            return 0;
        default:
            MST_SetSysAlarmBit(iCrdSysIndex, CRDSYS_ALARM_MST_ERROR);
            //m_MSTInterface.pf_CSN_ProgStop(0x0001 << iCrdSysIndex);
            return 1;
    }
}


//处理后作用MST代码，完成返回0，否则返回非0
uBit32 MST_ProcessAfterActionMST(Bit32 iCrdSysIndex, IIP_BUF* pProgData)
{
    MST_CTRL_DATA* pMSTCtrlData = NULL;

    //
    pMSTCtrlData = m_MSTCtrlData + iCrdSysIndex;

    switch(pMSTCtrlData->nExeStep)
    {
        case MST_EXE_END:
            pMSTCtrlData->nExeStep = MST_EXE_MCODE;
        case MST_EXE_MCODE:
            if (pProgData->nMCode > 0)
            {
                if (MST_ExeMCode(iCrdSysIndex, pProgData, 1))
                {
                    return 1;
                }
            }

            pMSTCtrlData->nExeStep = MST_EXE_TCODE;
        case MST_EXE_TCODE:
            if (pProgData->nTCode > 0)
            {
                if (MST_ExeTCode(iCrdSysIndex, pProgData->nTCode))
                {
                    return 1;
                }
            }

            pMSTCtrlData->nExeStep = MST_EXE_END;
            return 0;
        default:
            MST_SetSysAlarmBit(iCrdSysIndex, CRDSYS_ALARM_MST_ERROR);
            //m_MSTInterface.pf_CSN_ProgStop(0x0001 << iCrdSysIndex);
            return 1;
    }
}



/*
函 数 名：void CSM_MainCtrl()
功    能：通道管理主控函数
参    数：无
返 回 值：无
调用位置：在大循环中调用
注意事项：
*/
void MST_MainCtrl()
{
    int i;
    MST_STATE_DATA* pMSTStateData;

    for (i = 0; i < CRDSYS_MAX_COUNT; i++)
    {
        pMSTStateData = m_MSTStateData + i;

        if (pMSTStateData->ulStatus & CRDSYS_STATUS_PROG_STOP)
        {
            continue;
        }

        MST_ExeParaMCode(i);
    }
}


