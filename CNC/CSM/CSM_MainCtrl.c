/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：CSM_MainCtrl.c

摘    要：通道管理主控模块

运行环境：LPC43xx 

修改描述：

当前版本：1.0

修改作者：毛军

修改日期：2015年9月27日

使用说明：

*/

#include "string.h"
#include "CSM_Interface.h"
#include "CSM_DataStructDef.h"
#include "CSM_ParmMan.h"
#include "CSM_ProgBuf.h"
#include "CSM_AutoCtrl.h"
#include "CSM_CrdSysState.h"
#include "CSM_MainCtrl.h"

#include "..\\DataStructDef\\SYS_DataStructDef.h"
#include "..\\DataStructDef\\CRD_DataStructDef.h"
#include "..\\DataStructDef\\IPO_DataStructDefPub.h"
#include "..\\DataStructDef\\MST_DataStructDef.h"


extern CSM_INTERFACE    m_CSMInterface;
extern unsigned short    m_nCSMInterfaceInit;

CRDSYS_STATE_DATA        m_CrdSysStateData[CRDSYS_MAX_COUNT];

CSM_AUTO_CTRL_DATA        m_CSM_AutoCtrl[CRDSYS_MAX_COUNT];
IIP_BUF                    m_CSM_CurProgData[CRDSYS_MAX_COUNT];


//(解释器专用数据,标示逻辑轴已配置，配置坐标系轴映射表时设置Alan???)
//uBit32 m_CSM_NCAxisData[CRDSYS_MAX_COUNT][CRDSYS_AXIS_MAX_COUNT];
uBit32 m_CSM_NCAxisData[CRDSYS_MAX_COUNT]; //Alan modified at 2016-7-22
Bit32 m_pIprStartLineCount[CRDSYS_MAX_COUNT];//Alan Added at 2016-7-28
Bit32 m_pIprFinishLineCount[CRDSYS_MAX_COUNT];//Alan Added at 2016-7-28


//获取通道的NC轴缓冲区地址(供解释器使用)
uBit32* CSM_GetNcAxisAddr(Bit32 iCrdSysIndex)
{
    return &m_CSM_NCAxisData[iCrdSysIndex];
}


/*
函 数 名：void CSM_Init()
功    能：初始化通道数据
参    数：无
返 回 值：无
调用位置：系统上电时调用
注意事项：无
*/
void CSM_InitData()
{
    int i;

    //
    memset(m_CrdSysStateData, 0, sizeof(m_CrdSysStateData));

    //
    for (i = 0; i < CRDSYS_MAX_COUNT; i++)
    {
        m_CrdSysStateData[i].ulStatus |= CRDSYS_STATUS_FREE | CRDSYS_STATUS_PROG_STOP;

        //
        if (m_nCSMInterfaceInit == 0)
        {
            m_CrdSysStateData[i].ulAlarm |= CRDSYS_ALARM_CSM_INIT_ERROR;
        }
    }

    //
    memset(m_CSM_AutoCtrl, 0, sizeof(m_CSM_AutoCtrl));
    for (i = 0; i < CRDSYS_MAX_COUNT; i++)
    {
        m_CSM_AutoCtrl[i].ulCurProcessLineNO = 1;
        m_CSM_AutoCtrl[i].ulReleaseLineNO = 0;
        m_pIprStartLineCount[i]=0;
        m_pIprFinishLineCount[i]=0;
    }

    //
    CSM_ParmManInit();

    //
    CSM_InitProgBuf();

    //
    CSM_InitUserBuf();

    //
    memset(m_CSM_NCAxisData, 0, sizeof(m_CSM_NCAxisData));
}

//坐标系映射表改变时，必须调用本函数,成功返回0，否则返回非0值
uBit32 CSM_UpdateCrdSysAxisMapTable()
{
    return CSM_InitCrdSysMotorIndex();
}


//检测系统报警
void CSM_MonitorSysAlarm()
{
    if (m_CSMInterface.pf_SYS_GetAlarmStatus())
    {
        CSM_ProgEStop(CSM_GetAllCrdSysMask());
    }
}

//
void CSM_ProcessCtrlStatus(int iCrdSysIndex)
{
    uBit32 i;
    IPO_STATE_DATA IPOStateData;
    IIP_BUF        PastProgData;
    CRDSYS_STATE_DATA* pCrdSysState = NULL;

    //
    pCrdSysState = m_CrdSysStateData + iCrdSysIndex;

    m_CSMInterface.pf_IPO_GetStateData(iCrdSysIndex, &IPOStateData);

    //单段模式
    if (pCrdSysState->ulCtrl & CRDSYS_CTRL_ENABLE_SGL_BLK)
    {
        if ((IPOStateData.ulRunningStatus & CRDSYS_STATUS_SGL_BLK) && !(IPOStateData.ulCtrlStatus & CRDSYS_CTRL_ENABLE_SGL_BLK))
        {
            pCrdSysState->ulCtrl &= ~CRDSYS_CTRL_ENABLE_SGL_BLK;
            pCrdSysState->ulStatus |= CRDSYS_STATUS_SGL_BLK;
        }
    }

    if (pCrdSysState->ulCtrl & CRDSYS_CTRL_DISABLE_SGL_BLK)
    {
        if (!(IPOStateData.ulRunningStatus & CRDSYS_STATUS_SGL_BLK) && !(IPOStateData.ulCtrlStatus & CRDSYS_CTRL_DISABLE_SGL_BLK))
        {
            pCrdSysState->ulCtrl &= ~CRDSYS_CTRL_DISABLE_SGL_BLK;
            pCrdSysState->ulStatus &= ~CRDSYS_STATUS_SGL_BLK;
        }
    }

    //循环启动
    if (pCrdSysState->ulCtrl & CRDSYS_CTRL_PROG_RUN)
    {
        if ((IPOStateData.ulRunningStatus & CRDSYS_STATUS_PROG_RUN) && !(IPOStateData.ulCtrlStatus & CRDSYS_CTRL_PROG_RUN))
        {
            if (!(pCrdSysState->ulStatus & CRDSYS_STATUS_PROG_HOLD))
            { //如果程序从停止状态下启动，则程序从头开始执行（不能直接从上次停止行开始执行，因为起点已改变，非直线类轨迹也随之改变）
                m_CSM_AutoCtrl[iCrdSysIndex].ulCurProcessLineNO = 1;
                m_CSM_AutoCtrl[iCrdSysIndex].ulReleaseLineNO = 0;

                pCrdSysState->ulCurProgLineNO = 0;
                //pCrdSysState->ulUserLineNO = 0;\\Alan del at 2016-8-6
                pCrdSysState->ulStartLineCount = 0;
                pCrdSysState->ulFinishLineCount = 0;  
                m_pIprStartLineCount[iCrdSysIndex] = 0;       
                m_pIprFinishLineCount[iCrdSysIndex] = 0;
            }

            pCrdSysState->ulCtrl &= ~CRDSYS_CTRL_PROG_RUN;
            pCrdSysState->ulStatus &= ~CRDSYS_STATUS_PROG_MASK;
            pCrdSysState->ulStatus |= CRDSYS_STATUS_PROG_RUN;


            m_CSM_AutoCtrl[iCrdSysIndex].nExeStep = CSM_AUTO_EXE_GET_PROG_DATA;
        }
    }

    //进给保持
    if (pCrdSysState->ulCtrl & CRDSYS_CTRL_PROG_HOLD)
    {
        if ((IPOStateData.ulRunningStatus & CRDSYS_STATUS_PROG_HOLD) && !(IPOStateData.ulCtrlStatus & CRDSYS_CTRL_PROG_HOLD))
        {
            pCrdSysState->ulCtrl &= ~CRDSYS_CTRL_PROG_HOLD;
            pCrdSysState->ulStatus &= ~CRDSYS_STATUS_PROG_MASK;
            pCrdSysState->ulStatus |= CRDSYS_STATUS_PROG_HOLD;
        }
    }

    //程序停止
    if (pCrdSysState->ulCtrl & CRDSYS_CTRL_PROG_STOP)
    {
        if ((IPOStateData.ulRunningStatus & CRDSYS_STATUS_PROG_STOP) && !(IPOStateData.ulCtrlStatus & CRDSYS_CTRL_PROG_STOP))
        {
            pCrdSysState->ulCtrl &= ~CRDSYS_CTRL_PROG_STOP;
            pCrdSysState->ulStatus &= ~CRDSYS_STATUS_PROG_MASK;
            pCrdSysState->ulStatus |= CRDSYS_STATUS_PROG_STOP;
            pCrdSysState->ulStatus |= CRDSYS_STATUS_FREE;
            
            //最后刷新一次当前执行行-------Alan Added at 2016-8-25
            for (i = pCrdSysState->ulCurProgLineNO; i < IPOStateData.ulCurProgLineNO; i++)
            {
                pCrdSysState->ulCurProgLineNO++;

                if (CSM_GetProgData(iCrdSysIndex, pCrdSysState->ulCurProgLineNO, &PastProgData)==0)
                { 
                    if (PastProgData.nFlag & IFLAG_BLK_BEG)
                    {
                        pCrdSysState->ulStartLineCount++;
                        pCrdSysState->ulStartLineCount += PastProgData.nNullCnt;
                    }

                    if (PastProgData.nFlag & IFLAG_BLK_END)
                    {
                        pCrdSysState->ulFinishLineCount++;
                        pCrdSysState->ulFinishLineCount += PastProgData.nNullCnt;
                    }                    
                }
            }

            m_pIprStartLineCount[iCrdSysIndex] = pCrdSysState->ulStartLineCount;        //Alan Added at 2016-8-25
            m_pIprFinishLineCount[iCrdSysIndex] = pCrdSysState->ulFinishLineCount;
            
            if ((pCrdSysState->ulCtrl & CRDSYS_CTRL_IPR_PAUSE)==0)//如果是IPR执行暂停的停止，则不清用户自定义行号
            {
                pCrdSysState->ulUserLineNO = 0;
            }else
            {
                pCrdSysState->ulCtrl &= ~CRDSYS_CTRL_IPR_PAUSE;
            }

            if (!(CSM_GetProgBufStatus(iCrdSysIndex) & DATA_BUF_STATUS_STATIC))
            { //动态模式下，清除程序缓冲区数据
                CSM_ClearProgData(iCrdSysIndex, 2);
            }
        }
    }

    //程序急停
    if (pCrdSysState->ulCtrl & CRDSYS_CTRL_PROG_ESTOP)
    {
        if ((IPOStateData.ulRunningStatus & CRDSYS_STATUS_PROG_STOP) && !(IPOStateData.ulCtrlStatus & CRDSYS_CTRL_PROG_ESTOP))
        {
            pCrdSysState->ulCtrl &= ~CRDSYS_CTRL_PROG_ESTOP;
            pCrdSysState->ulStatus &= ~CRDSYS_STATUS_PROG_MASK;
            pCrdSysState->ulStatus |= CRDSYS_STATUS_PROG_STOP;

            pCrdSysState->ulStatus |= CRDSYS_STATUS_FREE;

            //最后刷新一次当前执行行-------Alan Added at 2016-8-25
            for (i = pCrdSysState->ulCurProgLineNO; i < IPOStateData.ulCurProgLineNO; i++)
            {
                pCrdSysState->ulCurProgLineNO++;

                if (CSM_GetProgData(iCrdSysIndex, pCrdSysState->ulCurProgLineNO, &PastProgData)==0)
                { 
                    if (PastProgData.nFlag & IFLAG_BLK_BEG)
                    {
                        pCrdSysState->ulStartLineCount++;
                        pCrdSysState->ulStartLineCount += PastProgData.nNullCnt;
                    }

                    if (PastProgData.nFlag & IFLAG_BLK_END)
                    {
                        pCrdSysState->ulFinishLineCount++;
                        pCrdSysState->ulFinishLineCount += PastProgData.nNullCnt;
                    }                    
                }
            }

            m_pIprStartLineCount[iCrdSysIndex] = pCrdSysState->ulStartLineCount;
            m_pIprFinishLineCount[iCrdSysIndex] = pCrdSysState->ulFinishLineCount;
            
            //
            if (!(CSM_GetProgBufStatus(iCrdSysIndex) & DATA_BUF_STATUS_STATIC))
            { //动态模式下，清除程序缓冲区数据
                CSM_ClearProgData(iCrdSysIndex, 2);
            }
        }
    }

    //程序取消
    if (pCrdSysState->ulCtrl & CRDSYS_CTRL_PROG_CANCEL)
    {
        if ((IPOStateData.ulRunningStatus & CRDSYS_STATUS_PROG_STOP) && !(IPOStateData.ulCtrlStatus & CRDSYS_CTRL_PROG_STOP))
        {
            pCrdSysState->ulCtrl &= ~CRDSYS_CTRL_PROG_CANCEL;
            pCrdSysState->ulStatus &= ~CRDSYS_STATUS_PROG_MASK;
            pCrdSysState->ulStatus |= CRDSYS_STATUS_PROG_CANCEL;
            pCrdSysState->ulStatus |= CRDSYS_STATUS_PROG_STOP;
            pCrdSysState->ulStatus |= CRDSYS_STATUS_FREE;

            //最后刷新一次当前执行行-------Alan Added at 2016-8-25
            for (i = pCrdSysState->ulCurProgLineNO; i < IPOStateData.ulCurProgLineNO; i++)
            {
                pCrdSysState->ulCurProgLineNO++;

                if (CSM_GetProgData(iCrdSysIndex, pCrdSysState->ulCurProgLineNO, &PastProgData)==0)
                { 
                    if (PastProgData.nFlag & IFLAG_BLK_BEG)
                    {
                        pCrdSysState->ulStartLineCount++;
                        pCrdSysState->ulStartLineCount += PastProgData.nNullCnt;
                    }

                    if (PastProgData.nFlag & IFLAG_BLK_END)
                    {
                        pCrdSysState->ulFinishLineCount++;
                        pCrdSysState->ulFinishLineCount += PastProgData.nNullCnt;
                    }                    
                }
            }
            

            m_pIprStartLineCount[iCrdSysIndex] = pCrdSysState->ulStartLineCount;        //Alan Added at 2016-8-25
            m_pIprFinishLineCount[iCrdSysIndex] = pCrdSysState->ulFinishLineCount;
            
            pCrdSysState->ulUserLineNO = 0; //Alan Added at 2016-8-23
            CSM_ClearProgExeState(iCrdSysIndex);            
            //if ()  
            { //如果解释器未使能，且程序缓冲区为动态模式，则自动清空程序缓冲区
            }
        }
    }

    //程序在运动段结束点停止
    if (pCrdSysState->ulCtrl & CRDSYS_CTRL_PROG_STOP_AT_SEG_END)
    {
        if ((IPOStateData.ulRunningStatus & CRDSYS_STATUS_PROG_STOP) && !(IPOStateData.ulCtrlStatus & CRDSYS_CTRL_PROG_STOP_AT_SEG_END))
        {
            pCrdSysState->ulCtrl &= ~CRDSYS_CTRL_PROG_STOP_AT_SEG_END;
            pCrdSysState->ulStatus &= ~CRDSYS_STATUS_PROG_MASK;
            pCrdSysState->ulStatus |= CRDSYS_STATUS_PROG_STOP;

            pCrdSysState->ulStatus |= CRDSYS_STATUS_FREE;

            //最后刷新一次当前执行行-------Alan Added at 2016-8-25
            if (CSM_GetProgData(iCrdSysIndex, IPOStateData.ulCurProgLineNO, &PastProgData)==0)
            {
                if (PastProgData.nFlag & IFLAG_BLK_BEG)
                {
                    pCrdSysState->ulStartLineCount++;
                    pCrdSysState->ulStartLineCount += PastProgData.nNullCnt;
                }

                if (PastProgData.nFlag & IFLAG_BLK_END)
                {
                    pCrdSysState->ulFinishLineCount++;
                    pCrdSysState->ulFinishLineCount += PastProgData.nNullCnt;
                }
            }

            m_pIprStartLineCount[iCrdSysIndex] = pCrdSysState->ulStartLineCount;
            m_pIprFinishLineCount[iCrdSysIndex] = pCrdSysState->ulFinishLineCount;
            //
            if (!(CSM_GetProgBufStatus(iCrdSysIndex) & DATA_BUF_STATUS_STATIC))
            { //动态模式下，清除程序缓冲区数据
                CSM_ClearProgData(iCrdSysIndex, 2);
            }
        }
    }

    //通道复位
    if (pCrdSysState->ulCtrl & CRDSYS_CTRL_RESET)
    {
        if (!(IPOStateData.ulRunningStatus & CRDSYS_STATUS_RESET) && !(IPOStateData.ulCtrlStatus & CRDSYS_CTRL_RESET))
        {
            //清除（可清除的）报警
            pCrdSysState->ulAlarm &= (CRDSYS_ALARM_CSM_INIT_ERROR | CRDSYS_ALARM_PROG_BUF_MAN_ERROR);

            //
            pCrdSysState->ulCtrl &= ~CRDSYS_CTRL_RESET;
            pCrdSysState->ulStatus &= ~CRDSYS_STATUS_RESET;

            pCrdSysState->ulStatus |= CRDSYS_STATUS_FREE;
            //
            CSM_ClearProgData(iCrdSysIndex, 2);
        }
    }

    //坐标系重定位
    if (pCrdSysState->ulCtrl & CRDSYS_CTRL_LOCATION)
    {
        if ((IPOStateData.ulRunningStatus & CRDSYS_STATUS_PROG_STOP) && !(IPOStateData.ulCtrlStatus & CRDSYS_CTRL_PROG_STOP))
        {
            pCrdSysState->ulCtrl &= ~CRDSYS_CTRL_LOCATION;
            pCrdSysState->ulStatus |= CRDSYS_STATUS_FREE;
        }
        else if (CSM_CheckGCodeExeFinish(iCrdSysIndex))
        {
            m_CSMInterface.pf_IPO_ProgStop(0x0001 << iCrdSysIndex);
        }    
    }

    //更新通道空闲状态
    if (IPOStateData.ulRunningStatus & CRDSYS_STATUS_FREE)
    { //插补器有空闲状态位
        if (!(pCrdSysState->ulStatus & CRDSYS_STATUS_FREE))
        {
            if ((pCrdSysState->ulStatus & (CRDSYS_STATUS_PROG_STOP | CRDSYS_STATUS_PROG_CANCEL)) 
                && pCrdSysState->ulCtrl == 0)
            { //
                pCrdSysState->ulStatus |= CRDSYS_STATUS_FREE;
            }
        }
    }
    else
    {
        pCrdSysState->ulStatus &= ~CRDSYS_STATUS_FREE;
    }
}

//
void CSM_UpdateMSTState(int iCrdSysIndex)
{
    MST_STATE_DATA MSTStateData;
    CRDSYS_STATE_DATA* pCrdSysState = NULL;

    //
    pCrdSysState = m_CrdSysStateData + iCrdSysIndex;

    //
    m_CSMInterface.pf_MST_GetStateData(iCrdSysIndex, &MSTStateData);

    pCrdSysState->ulAlarm &= ~CRDSYS_ALARM_MST_IN_CHARGE_MASK;
    pCrdSysState->ulAlarm |= (MSTStateData.ulAlarm & CRDSYS_ALARM_MST_IN_CHARGE_MASK);
}

//
void CSM_UpdateIPOState(int iCrdSysIndex)
{
    unsigned long i;
    IIP_BUF ProgData;
    IPO_STATE_DATA IPOStateData;
    CRDSYS_STATE_DATA* pCrdSysState = NULL;

    //
    pCrdSysState = m_CrdSysStateData + iCrdSysIndex;

    m_CSMInterface.pf_IPO_GetStateData(iCrdSysIndex, &IPOStateData);

    //////////////////////////////////////////////////////////////////////
    //laomao  2016-05-23 注释掉：通道的空闲状态在CSM_ProcessCtrlStatus函数（最后）中更新，这里的处理不严密
    /*
    //
    if (CSM_GetCtrlMode(iCrdSysIndex) != CRDSYS_STATUS_MODE_AUTO)
    {
        if (IPOStateData.ulRunningStatus & CRDSYS_STATUS_FREE)
        {
            pCrdSysState->ulStatus |= CRDSYS_STATUS_FREE;
        }
        else
        {
            pCrdSysState->ulStatus &= ~CRDSYS_STATUS_FREE;
        }
    }
    */
    //////////////////////////////////////////////////////////////////////

    //
    pCrdSysState->ulStatus &= ~CRDSYS_STATUS_IPO_IN_CHARGE_MASK;
    pCrdSysState->ulStatus |= (IPOStateData.ulRunningStatus & CRDSYS_STATUS_IPO_IN_CHARGE_MASK);

    //
    pCrdSysState->ulAlarm &= ~CRDSYS_ALARM_IPO_IN_CHARGE_MASK;
    pCrdSysState->ulAlarm |= (IPOStateData.ulAlarm & CRDSYS_ALARM_IPO_IN_CHARGE_MASK);

    //
    pCrdSysState->dRapidOverride = IPOStateData.dRapidOverride;
    pCrdSysState->dFeedOverride = IPOStateData.dFeedOverride;


    //
    if (pCrdSysState->ulStatus & (CRDSYS_STATUS_PROG_STOP | CRDSYS_STATUS_PROG_CANCEL))
    { //程序运行停止后，不用更新插补器的程序运行状态到通道
        return;
    }

    //--------------------------------------更新插补器的程序运行状态到通道----------------------------------------

    //
    if (pCrdSysState->ulCurProgLineNO < IPOStateData.ulCurProgLineNO)
    {
        //刷新当前执行用户自定义行号
        if (!CSM_GetProgData(iCrdSysIndex, IPOStateData.ulCurProgLineNO, &ProgData))
            pCrdSysState->ulUserLineNO = ProgData.ulUserLineNO;//Alan added at 2016-8-6

        //新的一行开始前先保证已完成的程序行释放掉
        if (m_CSM_AutoCtrl[iCrdSysIndex].ulReleaseLineNO < pCrdSysState->ulCurProgLineNO)
        {
            for (i = m_CSM_AutoCtrl[iCrdSysIndex].ulReleaseLineNO; i < pCrdSysState->ulCurProgLineNO; i++)
            {
                m_CSM_AutoCtrl[iCrdSysIndex].ulReleaseLineNO++;

                //
                if (CSM_GetProgData(iCrdSysIndex, m_CSM_AutoCtrl[iCrdSysIndex].ulReleaseLineNO, &ProgData))
                { //获取程序段数据失败
                    CSM_SetCrdSysAlarmBit(iCrdSysIndex, CRDSYS_ALARM_PROG_BUF_MAN_ERROR);
                    return;
                }

                if (ProgData.nFlag & IFLAG_BLK_END)
                {
                    pCrdSysState->ulFinishLineCount++;
                    pCrdSysState->ulFinishLineCount += ProgData.nNullCnt;
                }

                //释放已执行完成的程序行
                CSM_ReleaseProgData(iCrdSysIndex, m_CSM_AutoCtrl[iCrdSysIndex].ulReleaseLineNO);
            }
        }

        //将插补器中已开始的程序行更新到通道管理模块
        for (i = pCrdSysState->ulCurProgLineNO; i < IPOStateData.ulCurProgLineNO; i++)
        {
            //
            pCrdSysState->ulCurProgLineNO++;

            //
            if (CSM_GetProgData(iCrdSysIndex, pCrdSysState->ulCurProgLineNO, &ProgData))
            { //获取程序段数据失败
                CSM_SetCrdSysAlarmBit(iCrdSysIndex, CRDSYS_ALARM_PROG_BUF_MAN_ERROR);
                return;
            }

            if (ProgData.nFlag & IFLAG_BLK_BEG)
            {
                pCrdSysState->ulStartLineCount++;
                pCrdSysState->ulStartLineCount += ProgData.nNullCnt;
            }

            if (pCrdSysState->ulCurProgLineNO < IPOStateData.ulCurProgLineNO)
            {
                m_CSM_AutoCtrl[iCrdSysIndex].ulReleaseLineNO = pCrdSysState->ulCurProgLineNO;

                if (ProgData.nFlag & IFLAG_BLK_END)
                {
                    pCrdSysState->ulFinishLineCount++;
                    pCrdSysState->ulFinishLineCount += ProgData.nNullCnt;
                }

                //释放已执行完成的程序行
                CSM_ReleaseProgData(iCrdSysIndex, m_CSM_AutoCtrl[iCrdSysIndex].ulReleaseLineNO);
            }
        }

        //
        pCrdSysState->ulStatus &= ~CRDSYS_STATUS_CUR_SEGMENT_DONE;

        memcpy(pCrdSysState->dTargetCrd, IPOStateData.dTargetCrd, sizeof(IPOStateData.dTargetCrd));
    }

    //
    if (pCrdSysState->ulCurProgLineNO == IPOStateData.ulCurProgLineNO && m_CSM_AutoCtrl[iCrdSysIndex].ulCurProcessLineNO > pCrdSysState->ulCurProgLineNO)
    {
        if (!(pCrdSysState->ulStatus & CRDSYS_STATUS_CUR_SEGMENT_DONE) && (IPOStateData.ulRunningStatus & CRDSYS_STATUS_CUR_SEGMENT_DONE))
        {
            pCrdSysState->ulStatus |= CRDSYS_STATUS_CUR_SEGMENT_DONE;

            //
            if (CSM_GetProgData(iCrdSysIndex, pCrdSysState->ulCurProgLineNO, &ProgData))
            { //获取程序段数据失败
                CSM_SetCrdSysAlarmBit(iCrdSysIndex, CRDSYS_ALARM_PROG_BUF_MAN_ERROR);
                return;
            }

            if (ProgData.nFlag & IFLAG_BLK_END)
            {
                pCrdSysState->ulFinishLineCount++;
                pCrdSysState->ulFinishLineCount += ProgData.nNullCnt;
            }

            //释放已执行完成的程序行
            m_CSM_AutoCtrl[iCrdSysIndex].ulReleaseLineNO = pCrdSysState->ulCurProgLineNO;
            CSM_ReleaseProgData(iCrdSysIndex, m_CSM_AutoCtrl[iCrdSysIndex].ulReleaseLineNO);
        }
    }

    pCrdSysState->dProgFeedRate = IPOStateData.dProgFeedRate;
    pCrdSysState->dFeedRate = IPOStateData.dFeedRate;
    pCrdSysState->dAcc = IPOStateData.dAcc;
}

//获取完全停止后开始和结束行总数
Bit32 CSM_GetExeLineCount(Bit32 iCrdSysIndex, Bit32 *pStartLineCount, Bit32 *pFinishLineCount)
{
    if((m_CrdSysStateData[iCrdSysIndex].ulStatus&CRDSYS_STATUS_PROG_STOP))//程序停止
    {
        *pStartLineCount = m_pIprStartLineCount[iCrdSysIndex];
        *pFinishLineCount = m_pIprFinishLineCount[iCrdSysIndex];      
    }else
    {
        *pStartLineCount = m_CrdSysStateData[iCrdSysIndex].ulStartLineCount;
        *pFinishLineCount = m_CrdSysStateData[iCrdSysIndex].ulFinishLineCount;  
    }

    return 0;
}

//检测坐标系报警
void CSM_MonitorCrdSysAlarm(int iCrdSysIndex)
{
    if (m_CrdSysStateData[iCrdSysIndex].ulAlarm)
    {
        //停止插补器
        if (CSM_CheckCrdSysInterrelation())
        {
            CSM_ProgEStop(CSM_GetAllCrdSysMask());
        } 
        else
        {
            CSM_ProgEStop(1 << iCrdSysIndex);
        }
    }
}

//检测坐标轴报警
void CSM_MonitorAxisAlarm(int iCrdSysIndex)
{
    if (CSM_CheckAxisAlarm(iCrdSysIndex))
    {
        if (CSM_CheckCrdSysInterrelation())
        {
            CSM_ProgEStop(CSM_GetAllCrdSysMask());
        } 
        else
        {
            CSM_ProgEStop(1 << iCrdSysIndex);
        }
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
void CSM_MainCtrl()
{
    int i;
    int iCrdSysCount;

    //检测系统报警
    CSM_MonitorSysAlarm();

    //
    iCrdSysCount = CSM_GetCrdSysCount();

    for (i = 0; i < iCrdSysCount; i++)
    {
        //
        CSM_UpdateIPOState(i);

        //
        CSM_UpdateMSTState(i);

        //
        CSM_ProcessCtrlStatus(i); 

        //检测坐标系报警
        CSM_MonitorCrdSysAlarm(i);

        //检测轴报警
        CSM_MonitorAxisAlarm(i);

        //
        switch(CSM_GetCtrlMode(i))
        {
            case CRDSYS_STATUS_MODE_AUTO:
                CSM_AutoMainCtrl(i);
                break;
            case CRDSYS_STATUS_MODE_JOG:
            case CRDSYS_STATUS_MODE_STEP:
                break;
            case CRDSYS_STATUS_MODE_HANDWHEEL:
                break;
            case CRDSYS_STATUS_MODE_HOME:
                break;
            default:
                break;
        }
    }
}
