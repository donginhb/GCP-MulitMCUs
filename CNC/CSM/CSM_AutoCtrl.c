/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：CSM_AutoCtrl.c

摘    要：通道自动模式运行控制

运行环境：LPC43xx 

修改描述：

当前版本：1.0

修改作者：毛军

修改日期：2015年9月27日

使用说明：

*/

#include "math.h"
#include "string.h"
#include "CSM_AutoCtrl.h"
#include "CSM_DataStructDef.h"
#include "CSM_MainCtrl.h"
#include "CSM_Interface.h"
#include "CSM_ParmMan.h"
#include "CSM_CrdSysState.h"
#include "CSM_ProgBuf.h"
#include "CSM_CircleDataProcess.h"

#include "CNC/DataStructDef/SYS_DataStructDef.h"
#include "CNC/DataStructDef/IPR_DataStructDef.h"
#include "CNC/DataStructDef/CRD_DataStructDef.h"
#include "CNC/DataStructDef/IPO_DataStructDefPub.h"

//
extern CSM_INTERFACE    m_CSMInterface;


/*
函 数 名：int CSM_CheckMSTExist(IIP_BUF* pProgData)
功    能：检测程序段是否存在MST代码
参    数：pProgData --程序段数据地址
返 回 值：存在MST代码返回1，否则返回0
调用位置：
注意事项：无
*/
/*
int CSM_CheckMSTExist(IIP_BUF* pProgData)
{
    if (pProgData->nMCode != 0 || pProgData->nSCode !=0 || pProgData->nTCode != 0)
    {
        return 1;
    }

    return 0;
}
*/

/*
函 数 名：int CSM_CheckGCodeExeFinish(int iCrdSysIndex)
功    能：检测G代码执行结束
参    数：iCrdSysIndex --坐标系索引
返 回 值：G代码执行结束返回1，否则返回0
调用位置：
注意事项：无
*/
int CSM_CheckGCodeExeFinish(int iCrdSysIndex)
{
    DATA_BUF_MAN IPOBufState;
    IPO_STATE_DATA IPOStateData;

    //
    m_CSMInterface.pf_IPO_GetBufState(iCrdSysIndex, &IPOBufState);

    if (IPOBufState.ulSegmentCount > 0)
    {
        return 0;
    }

    //
    m_CSMInterface.pf_IPO_GetStateData(iCrdSysIndex, &IPOStateData);
    
    //
    if (m_CrdSysStateData[iCrdSysIndex].ulCurProgLineNO > IPOStateData.ulCurProgLineNO)
    { //（程序开始时可能不是G代码）
        return 1;
    }

    if (!(IPOStateData.ulRunningStatus & CRDSYS_STATUS_CUR_SEGMENT_DONE))
    {
        return 0;
    }

    //(重要，防止数据不同步)
    if (m_CrdSysStateData[iCrdSysIndex].ulCurProgLineNO < IPOStateData.ulCurProgLineNO 
        || !(m_CrdSysStateData[iCrdSysIndex].ulStatus & CRDSYS_STATUS_CUR_SEGMENT_DONE) )
    {
        return 0;
    }

    return 1;
}

//
int CSM_CheckCurLineGCodeExeFinish(int iCrdSysIndex)
{
    DATA_BUF_MAN IPOBufState;
    IPO_STATE_DATA IPOStateData;

    //
    m_CSMInterface.pf_IPO_GetBufState(iCrdSysIndex, &IPOBufState);

    if (IPOBufState.ulSegmentCount > 0)
    {
        return 0;
    }

    //
    m_CSMInterface.pf_IPO_GetStateData(iCrdSysIndex, &IPOStateData);

    //
    if (m_CrdSysStateData[iCrdSysIndex].ulCurProgLineNO > IPOStateData.ulCurProgLineNO)
    { //（程序开始时可能不是G代码）
        return 1;
    }

    if (!(IPOStateData.ulRunningStatus & CRDSYS_STATUS_CUR_SEGMENT_DONE))
    {
        return 0;
    }

    //(重要，防止数据不同步)
    if (m_CrdSysStateData[iCrdSysIndex].ulCurProgLineNO < IPOStateData.ulCurProgLineNO)
    {
        return 0;
    }

    return 1;
}


//检测程序段是否存在G代码，存在返回1，否则返回0
int CSM_CheckGCodeExist(IIP_BUF* pProgData)
{
    if (pProgData->ulCmdAxis == 0 && pProgData->nCmd == 0)
    {
        return 0;
    }

    return 1;
}


//检测程序是否有效, 有效返回1，否则返回0
int CSM_CheckProgDataVaild(IIP_BUF* pProgData)
{
    if (pProgData->ulCmdAxis != 0 || pProgData->nCmd != 0)
    {
        return 1;
    }

    if (pProgData->nMCode != 0)
    {
        return 1;
    }

    if (pProgData->nSCode != 0)
    {
        return 1;
    }

    if (pProgData->nTCode != 0)
    {
        return 1;
    }

    return 0;
}

//


/*
函 数 名：int CSM_ProcessGCode(int iCrdSysIndex, IIP_BUF* pProgData)
功    能：处理程序段中的G代码
参    数：iCrdSysIndex --坐标系索引
          pProgData --程序段数据地址
返 回 值：成功返回0，否则返回非0
调用位置：
注意事项：无
*/
int CSM_ProcessGCode(int iCrdSysIndex, IIP_BUF* pProgData)
{
    SEGMENT_MOTION_DATA    MotionData;
    int iCrdSysAxisCount;
    int i;
    double dTemp;

    if (pProgData->ulCmdAxis == 0 && pProgData->nCmd == 0)
    {
        return 0;
    }

    //
    memset(&MotionData, 0, sizeof(MotionData));

    //
    MotionData.ulProgLineNO = m_CSM_AutoCtrl[iCrdSysIndex].ulCurProcessLineNO;
    MotionData.ulUserLineNO = pProgData->ulUserLineNO;
    MotionData.nMotionMode = MOTION_MODE_INTERPOLATION;

    memcpy(MotionData.IPO.dCrd, m_CSM_AutoCtrl[iCrdSysIndex].dCrd, sizeof(MotionData.IPO.dCrd));

    iCrdSysAxisCount = CSM_GetCrdSysAxisCount(iCrdSysIndex);
    for (i = 0; i < iCrdSysAxisCount; i++)
    {
        if (pProgData->ulCmdAxis & (0x0001 << i))
        {
            if (pProgData->nFlag & IFLAG_ABS_CRD)
            { //绝对坐标
                MotionData.IPO.dCrd[i] = pProgData->lPinc[i];
            }
            else
            { //增量坐标
                MotionData.IPO.dCrd[i] += pProgData->lPinc[i];
            }
        }
    }

    MotionData.IPO.ulOutputDisableAxisMask = ~pProgData->ulOutputEnableAxis;

    switch (pProgData->nCmd)
    {
        case ICMD_RAPID:    //快速移动
            MotionData.IPO.nCmdType = MOTION_CMD_RAPID;
            break;
        case ICMD_LINE:        //直线
            MotionData.IPO.nCmdType = MOTION_CMD_LINE;

            //
            //MotionData.IPO.dFeedRate = pProgData->Data.normal.lFeedSpeed / 60000.0;
            MotionData.IPO.dFeedRate = pProgData->Data.normal.dFeedSpeed;

            if (pProgData->nFlag & IFLAG_FINESTOP)
            {
                MotionData.IPO.dEndSpeed = 0.0;
            }
            else
            {
                MotionData.IPO.dEndSpeed = ENDSPEED_SMOTH_FLAG;
            }
            break;
        case ICMD_CW:        //顺时针圆弧
        case ICMD_CCW:        //逆时针圆弧
            if (pProgData->nFlag & IFLAG_CIRCLE_USER)
            { //上位机用户设置的圆弧数据
                if (CSM_CircleDataProcess(m_CSM_AutoCtrl[iCrdSysIndex].dCrd, pProgData, &MotionData))
                {
                    CSM_SetCrdSysAlarmBit(iCrdSysIndex, CRDSYS_ALARM_PROG_DATA_ERROR);
                    return 1;
                }
            }
            else
            { //解释器输出的圆弧数据
                if (pProgData->nCmd == ICMD_CW)
                {
                    MotionData.IPO.nCmdType = MOTION_CMD_CW;
                }
                else
                {
                    MotionData.IPO.nCmdType = MOTION_CMD_CCW;
                }

                //
                //MotionData.IPO.dFeedRate = pProgData->Data.normal.lFeedSpeed / 60000.0;
                MotionData.IPO.dFeedRate = pProgData->Data.normal.dFeedSpeed;

                if (pProgData->nFlag & IFLAG_FINESTOP)
                {
                    MotionData.IPO.dEndSpeed = 0.0;
                }
                else
                {
                    MotionData.IPO.dEndSpeed = ENDSPEED_SMOTH_FLAG;
                }

                //
                MotionData.IPO.Data.Circle.nAxisIndex[0] = pProgData->nAxis1;
                MotionData.IPO.Data.Circle.nAxisIndex[1] = pProgData->nAxis2;
                //Alan modified at 2016-8-22
                //修改原因：解释器圆弧插补在圆弧段暂停后，无法知道相对起点位置，故IPR出来的数据DI和DJ全部是绝对坐标
                //MotionData.IPO.Data.Circle.dI = m_CSM_AutoCtrl[iCrdSysIndex].dCrd[pProgData->nAxis1] + pProgData->Data.normal.i;
                //MotionData.IPO.Data.Circle.dJ = m_CSM_AutoCtrl[iCrdSysIndex].dCrd[pProgData->nAxis2] + pProgData->Data.normal.j;
                MotionData.IPO.Data.Circle.dI = pProgData->Data.normal.i;
                MotionData.IPO.Data.Circle.dJ = pProgData->Data.normal.j;

                dTemp = (double)(m_CSM_AutoCtrl[iCrdSysIndex].dCrd[pProgData->nAxis1]-pProgData->Data.normal.i);
                MotionData.IPO.Data.Circle.dR = dTemp*dTemp;
                dTemp = (double)(m_CSM_AutoCtrl[iCrdSysIndex].dCrd[pProgData->nAxis2]-pProgData->Data.normal.j);
                MotionData.IPO.Data.Circle.dR += dTemp*dTemp;
                MotionData.IPO.Data.Circle.dR = sqrt(MotionData.IPO.Data.Circle.dR);
            }

            break;
        case ICMD_DWELL:    //延时
            MotionData.IPO.nCmdType = MOTION_CMD_DWELL;
            MotionData.IPO.Data.Dwell.ulTime = pProgData->Data.Dwell.lTime;
            break;
        default:
            CSM_SetCrdSysAlarmBit(iCrdSysIndex, CRDSYS_ALARM_PROG_INVALID_CMD);
            return 1;
    }

    if (m_CSMInterface.pf_IPO_AddMotionDataToIPOBuf(iCrdSysIndex, &MotionData))
    {
        return 1;
    }

    //
    memcpy(m_CSM_AutoCtrl[iCrdSysIndex].dCrd, MotionData.IPO.dCrd, sizeof(MotionData.IPO.dCrd));

    return 0;
}


/*
函 数 名：void CSM_MainCtrl()
功    能：通道管理主控函数
参    数：无
返 回 值：无
调用位置：在大循环中调用
注意事项：
*/
void CSM_AutoMainCtrl(int iCrdSysIndex)
{
    CRDSYS_STATE_DATA* pCrdSysStateData = NULL;
    CSM_AUTO_CTRL_DATA* pCMSAutoCtrl = NULL;
    IIP_BUF* pProgData = NULL;
    IIP_BUF PastProgData;
    uBit32 i;

    //
    pCrdSysStateData = m_CrdSysStateData + iCrdSysIndex;
    pCMSAutoCtrl = m_CSM_AutoCtrl + iCrdSysIndex;
    pProgData = m_CSM_CurProgData + iCrdSysIndex;

    //检测停止指令
    if (pCrdSysStateData->ulStatus & (CRDSYS_STATUS_PROG_STOP | CRDSYS_STATUS_PROG_CANCEL))
    {
        return;
    }

    //
    if (pCrdSysStateData->ulCtrl & CRDSYS_CTRL_PROG_ESTOP)
    {
        return;
    }

    //
    switch (pCMSAutoCtrl->nExeStep)
    {
        case CSM_AUTO_EXE_GET_PROG_DATA:                        //获取程序段数据
            if (pCrdSysStateData->ulCtrl & CRDSYS_CTRL_PROG_STOP_AT_SEG_END)
            {
                pCMSAutoCtrl->nExeStep = CSM_AUTO_EXE_END;
                break;
            }

            //
            if (CSM_GetProgData(iCrdSysIndex, pCMSAutoCtrl->ulCurProcessLineNO, pProgData))
            { //获取程序数据失败
                break;
            }

            if (CSM_CheckProgDataVaild(pProgData))
            {
                if (m_CSMInterface.pf_MST_CheckBeforeActionMSTExist(pProgData))
                {
                    pCMSAutoCtrl->nExeStep = CSM_AUTO_EXE_WAIT_G_CODE_FINISH;
                }
                else
                {
                    pCMSAutoCtrl->nExeStep = CSM_AUTO_EXE_PROCESS_G_CODE;
                }
            }
            else
            {
                pCMSAutoCtrl->ulCurProcessLineNO++;
                break;
            }

        case CSM_AUTO_EXE_WAIT_G_CODE_FINISH:                    //等待G代码执行完成
            if (pCMSAutoCtrl->nExeStep == CSM_AUTO_EXE_WAIT_G_CODE_FINISH)
            {
                if (CSM_CheckGCodeExeFinish(iCrdSysIndex))
                {
                    pCMSAutoCtrl->nExeStep = CSM_AUTO_EXE_PROCESS_BEFORE_ACTION_MST;

                    //(处理前面的空行)
                    for (i = pCrdSysStateData->ulCurProgLineNO; i < pCMSAutoCtrl->ulCurProcessLineNO - 1; i++)
                    {
                        pCrdSysStateData->ulCurProgLineNO++;

                        //
                        if (CSM_GetProgData(iCrdSysIndex, pCrdSysStateData->ulCurProgLineNO, &PastProgData))
                        { //获取程序段数据失败
                            CSM_SetCrdSysAlarmBit(iCrdSysIndex, CRDSYS_ALARM_PROG_BUF_MAN_ERROR);
                            return;
                        }

                        if (PastProgData.nFlag & IFLAG_BLK_BEG)
                        {
                            pCrdSysStateData->ulStartLineCount++;
                            pCrdSysStateData->ulStartLineCount += PastProgData.nNullCnt;
                        }

                        if (PastProgData.nFlag & IFLAG_BLK_END)
                        {
                            pCrdSysStateData->ulFinishLineCount++;
                            pCrdSysStateData->ulFinishLineCount += PastProgData.nNullCnt;
                        }

                        //释放已执行完成的程序行
                        //m_pIprStartLineCount[iCrdSysIndex] = pCrdSysStateData->ulStartLineCount;       
                        //m_pIprFinishLineCount[iCrdSysIndex] = pCrdSysStateData->ulFinishLineCount;
                        pCMSAutoCtrl->ulReleaseLineNO = pCrdSysStateData->ulCurProgLineNO;
                        CSM_ReleaseProgData(iCrdSysIndex, pCMSAutoCtrl->ulReleaseLineNO);
                    }

                    //
                    pCrdSysStateData->ulCurProgLineNO = pCMSAutoCtrl->ulCurProcessLineNO;
                    pCrdSysStateData->ulStatus &= ~CRDSYS_STATUS_CUR_SEGMENT_DONE;

                    //
                    if (pProgData->nFlag & IFLAG_BLK_BEG)
                    {
                        pCrdSysStateData->ulStartLineCount++;
                        pCrdSysStateData->ulStartLineCount += pProgData->nNullCnt;
                    }
                }
                else
                {
                    break;
                }
            }
        case CSM_AUTO_EXE_PROCESS_BEFORE_ACTION_MST:            //处理前作用MST
            if (pCMSAutoCtrl->nExeStep == CSM_AUTO_EXE_PROCESS_BEFORE_ACTION_MST)
            {
                if (m_CSMInterface.pf_MST_ProcessBeforeActionMST(iCrdSysIndex, pProgData))
                {
                    break;
                }
                else
                {
                    pCMSAutoCtrl->nExeStep = CSM_AUTO_EXE_PROCESS_G_CODE;
                }
            }
        case CSM_AUTO_EXE_PROCESS_G_CODE:                        //处理G代码
            if (CSM_CheckGCodeExist(pProgData))
            { //有G代码
                if (CSM_ProcessGCode(iCrdSysIndex, pProgData))
                {
                    break;
                }

                //
                if (m_CSMInterface.pf_MST_CheckAfterActionMSTExist(pProgData))
                {
                    pCMSAutoCtrl->nExeStep = CSM_AUTO_EXE_WAIT_CUR_LINE_G_CODE_FINISH;
                }
                else
                {
                    pCMSAutoCtrl->ulCurProcessLineNO++;
                    pCMSAutoCtrl->nExeStep = CSM_AUTO_EXE_GET_PROG_DATA;
                    break;
                }
            }
            else
            { //没有G代码
                if (m_CSMInterface.pf_MST_CheckAfterActionMSTExist(pProgData))
                {
                    pCMSAutoCtrl->nExeStep = CSM_AUTO_EXE_WAIT_CUR_LINE_G_CODE_FINISH;
                }
                else
                {
                    //(当前程序行只存在前作用MST)
                    pCrdSysStateData->ulStatus |= CRDSYS_STATUS_CUR_SEGMENT_DONE;

                    if (pProgData->nFlag & IFLAG_BLK_END)
                    {
                        pCrdSysStateData->ulFinishLineCount++;
                        pCrdSysStateData->ulFinishLineCount += pProgData->nNullCnt;
                    }

                    //m_pIprStartLineCount[iCrdSysIndex] = pCrdSysStateData->ulStartLineCount;       
                    //m_pIprFinishLineCount[iCrdSysIndex] = pCrdSysStateData->ulFinishLineCount;
                    //释放已执行完成的程序行
                    pCMSAutoCtrl->ulReleaseLineNO = pCrdSysStateData->ulCurProgLineNO;
                    CSM_ReleaseProgData(iCrdSysIndex, pCMSAutoCtrl->ulReleaseLineNO);

                    //
                    pCMSAutoCtrl->ulCurProcessLineNO++;
                    pCMSAutoCtrl->nExeStep = CSM_AUTO_EXE_GET_PROG_DATA;
                    break;
                }
            }
        case CSM_AUTO_EXE_WAIT_CUR_LINE_G_CODE_FINISH:            //等待当前行G代码执行完成
            if (CSM_CheckCurLineGCodeExeFinish(iCrdSysIndex))
            {
                pCMSAutoCtrl->nExeStep = CSM_AUTO_EXE_PROCESS_AFTER_ACTION_MST;

                //
                if (!m_CSMInterface.pf_MST_CheckBeforeActionMSTExist(pProgData) && !CSM_CheckGCodeExist(pProgData))
                { //没有前置MST，也没有G代码
                    //(处理前面的空行)
                    for (i = pCrdSysStateData->ulCurProgLineNO; i < pCMSAutoCtrl->ulCurProcessLineNO - 1; i++)
                    {
                        pCrdSysStateData->ulCurProgLineNO++;

                        //
                        if (CSM_GetProgData(iCrdSysIndex, pCrdSysStateData->ulCurProgLineNO, &PastProgData))
                        { //获取程序段数据失败
                            CSM_SetCrdSysAlarmBit(iCrdSysIndex, CRDSYS_ALARM_PROG_BUF_MAN_ERROR);
                            return;
                        }

                        if (PastProgData.nFlag & IFLAG_BLK_BEG)
                        {
                            pCrdSysStateData->ulStartLineCount++;
                            pCrdSysStateData->ulStartLineCount += PastProgData.nNullCnt;
                        }

                        if (PastProgData.nFlag & IFLAG_BLK_END)
                        {
                            pCrdSysStateData->ulFinishLineCount++;
                            pCrdSysStateData->ulFinishLineCount += PastProgData.nNullCnt;
                        }

                        //m_pIprStartLineCount[iCrdSysIndex] = pCrdSysStateData->ulStartLineCount;       
                        //m_pIprFinishLineCount[iCrdSysIndex] = pCrdSysStateData->ulFinishLineCount;
                        //释放已执行完成的程序行
                        pCMSAutoCtrl->ulReleaseLineNO = pCrdSysStateData->ulCurProgLineNO;
                        CSM_ReleaseProgData(iCrdSysIndex, pCMSAutoCtrl->ulReleaseLineNO);

                    }

                    //新的一行开始
                    pCrdSysStateData->ulCurProgLineNO = pCMSAutoCtrl->ulCurProcessLineNO;
                    pCrdSysStateData->ulStatus &= ~CRDSYS_STATUS_CUR_SEGMENT_DONE;

                    //
                    if (pProgData->nFlag & IFLAG_BLK_BEG)
                    {
                        pCrdSysStateData->ulStartLineCount++;
                        pCrdSysStateData->ulStartLineCount += pProgData->nNullCnt;
                    }
                }

            }
            else
            {
                break;
            }
        case CSM_AUTO_EXE_PROCESS_AFTER_ACTION_MST:                //处理后作用MST
            if (m_CSMInterface.pf_MST_ProcessAfterActionMST(iCrdSysIndex, pProgData))
            {
                break;
            }
            else
            {
                //
                pCrdSysStateData->ulStatus |= CRDSYS_STATUS_CUR_SEGMENT_DONE;

                if (pProgData->nFlag & IFLAG_BLK_END)
                {
                    pCrdSysStateData->ulFinishLineCount++;
                    pCrdSysStateData->ulFinishLineCount += pProgData->nNullCnt;
                }

                //m_pIprStartLineCount[iCrdSysIndex] = pCrdSysStateData->ulStartLineCount;       
                //m_pIprFinishLineCount[iCrdSysIndex] = pCrdSysStateData->ulFinishLineCount;
                //释放已执行完成的程序行
                pCMSAutoCtrl->ulReleaseLineNO = pCrdSysStateData->ulCurProgLineNO;
                CSM_ReleaseProgData(iCrdSysIndex, pCMSAutoCtrl->ulReleaseLineNO);

                //
                pCMSAutoCtrl->ulCurProcessLineNO++;
                pCMSAutoCtrl->nExeStep = CSM_AUTO_EXE_GET_PROG_DATA;
            }
            break;
        case CSM_AUTO_EXE_END:                                    //程序运行结束
            break;
        default:
            break;
    }


}

