
/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：CSM_CrdSyState.c 

摘    要：通道状态管理

运行环境：LPC43xx 

修改描述：

当前版本：3.0

修改作者：毛军

修改日期：2015年10月08日
*/

#include "string.h"
#include "CSM_CrdSysState.h"
#include "CSM_Interface.h"
#include "CSM_MainCtrl.h"
#include "CSM_ParmMan.h"
#include "CSM_AxisStateMan.h"
#include "CSM_ProgBuf.h"

extern CSM_INTERFACE    m_CSMInterface;

#include "..\\DataStructDef\\CRD_DataStructDef.h"
#include "..\\DataStructDef\\ERR_DataStructDef.h"
#include "..\\ErrorCodeDef\\CSM_ErrorCodeDef.h"



//
/*
函 数 名：Bit32 CSM_CheckCrdSysFree(Bit32 iCrdSysIndex)
功    能：检测通道是否空闲（运动停止且无程序或其他运动指令在执行）
参    数：iCrdSysIndex --通道索引
返 回 值：空闲返回1，否则返回0
调用位置：
注意事项：无
*/
Bit32 CSM_CheckCrdSysFree(Bit32 iCrdSysIndex)
{
    if (m_CrdSysStateData[iCrdSysIndex].ulStatus & CRDSYS_STATUS_FREE)
    {
        return 1;
    }
    
    return 0;
}


/*
函 数 名：Bit32 CSM_CheckSysFree()
功    能：检测各通道是否空闲（运动停止且无程序或其他运动指令在执行）
参    数：无
返 回 值：空闲返回1，否则返回0
调用位置：
注意事项：无
*/
Bit32 CSM_CheckSysFree()
{
    int i;

    for (i = 0; i < CRDSYS_MAX_COUNT; i++)
    {
        if (!CSM_CheckCrdSysFree(i))
        {
            return 0;
        }
    }

    return 1;
}

//检查是否存在通道报警，存在报警返回1，否则返回0
Bit32 CSM_CheckSysAlarm()
{
    int i;

    for (i = 0; i < CRDSYS_MAX_COUNT; i++)
    {
        if (m_CrdSysStateData[i].ulAlarm)
        {
            return 1;
        }
    }

    return 0;
}


/*
函 数 名：uBit32 CSM_Reset(Bit32 iCrdSysIndex)
功    能：通道复位
参    数：iCrdSysIndex --通道索引
返 回 值：成功接收复位指令返回0，否则返回错误码
调用位置：
注意事项：本函数调用成功，不表示复位完成
*/
uBit32 CSM_Reset(Bit32 iCrdSysIndex)
{
    uBit32 ulErrorCode;

    //
    if(!CSM_CheckCrdSysFree(iCrdSysIndex))
    {
        ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, iCrdSysIndex, CSM_ERR_CRDSYS_NOT_FREE, 
            CSM_CMD_RESET, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);

        return ulErrorCode;
    }

    //
    ulErrorCode = m_CSMInterface.pf_IPO_Reset(iCrdSysIndex);
    if (ulErrorCode != 0)
    {
        return ulErrorCode;
    }

    //
    ulErrorCode = m_CSMInterface.pf_MST_Reset(iCrdSysIndex);
    if (ulErrorCode != 0)
    {
        return ulErrorCode;
    }

    //
    m_CrdSysStateData[iCrdSysIndex].ulCtrl |= CRDSYS_CTRL_RESET;
    m_CrdSysStateData[iCrdSysIndex].ulStatus |= CRDSYS_STATUS_RESET;

    m_CrdSysStateData[iCrdSysIndex].ulStatus &= ~CRDSYS_STATUS_FREE;

    return 0;
}

/*
函 数 名：Bit32 CSM_CheckResetState(Bit32 iCrdSysIndex)
功    能：检测通道是否正在复位
参    数：iCrdSysIndex --通道索引
返 回 值：正在复位返回1，否则返回0
调用位置：
注意事项：
*/
Bit32 CSM_CheckResetState(Bit32 iCrdSysIndex)
{
    if (m_CrdSysStateData[iCrdSysIndex].ulCtrl & CRDSYS_CTRL_RESET)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


/*
函 数 名：int IPO_SetWorkMode(unsigned long ulMode)
功    能：设置插补器工作模式
参    数：iCrdSysIndex——坐标系索引，取值0-1
          ulMode——控制模式
返 回 值：成功返回0，否则返回非0值
调用位置：应用层调用
注意事项：设置模式是不在运动状态，且没有控制指令在执行
*/
uBit32 CSM_SetCtrlMode(Bit32 iCrdSysIndex, uBit32 ulMode)
{
    uBit32 ulErrorCode;
    
    //
    if (!CSM_CheckCrdSysFree(iCrdSysIndex))
    {
        ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, iCrdSysIndex, CSM_ERR_CRDSYS_NOT_FREE, 
                                                                CSM_CMD_SET_CTRL_MODE, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }

    //
    ulErrorCode = m_CSMInterface.pf_IPO_SetCtrlMode(iCrdSysIndex, ulMode);
    if (ulErrorCode != 0)
    {
        return ulErrorCode;
    }
    
    //    
    if ((m_CrdSysStateData[iCrdSysIndex].ulStatus & CRDSYS_STATUS_MODE_MASK) != ulMode)
    {
        m_CrdSysStateData[iCrdSysIndex].ulStatus &= ~CRDSYS_STATUS_MODE_MASK;
        m_CrdSysStateData[iCrdSysIndex].ulStatus |= ulMode;

        //清空坐标系当前的执行行号
        //m_CrdSysStateData[iCrdSysIndex].ulUserLineNO = 0;\\Alan del at 2016-8-6
        m_CrdSysStateData[iCrdSysIndex].ulCurProgLineNO = 0;
    }    

    return 0;
}


/*
函 数 名：unsigned long IPO_GetCtrlMode(int iCrdSysIndex)
功    能：获取插补器控制模式
参    数：iCrdSysIndex——坐标系索引，取值0-1
返 回 值：控制模式
调用位置：插补器内部调用
注意事项：
*/
uBit32 CSM_GetCtrlMode(Bit32 iCrdSysIndex)
{
    return m_CrdSysStateData[iCrdSysIndex].ulStatus & CRDSYS_STATUS_MODE_MASK;
}


/*
函 数 名：void IPO_SetCrdSysFeedOverride(int iCrdSysIndex, float32 dOverride)
功    能：设置坐标系进给修调
参    数：iCrdSysIndex——坐标系索引，取值0-1
          dOverride——进给修调，取值0.0-1.0
返 回 值：无
调用位置：应用层调用
注意事项：无
*/
uBit32 CSM_SetFeedOverride(Bit32 iCrdSysIndex, float32 dOverride)
{
    return m_CSMInterface.pf_IPO_SetCrdSysFeedOverride(iCrdSysIndex, dOverride);
}

/*
函 数 名：void IPO_SetCrdSysRapidOverride(int iCrdSysIndex, float32 dOverride)
功    能：设置坐标系快移修调
参    数：iCrdSysIndex——坐标系索引，取值0-1
          dOverride——快移修调，取值0.0-1.0
返 回 值：无
调用位置：应用层调用
注意事项：无
*/
uBit32 CSM_SetRapidOverride(Bit32 iCrdSysIndex, float32 dOverride)
{
    return m_CSMInterface.pf_IPO_SetCrdSysRapidOverride(iCrdSysIndex, dOverride);
}

//设置主轴修调
uBit32 CSM_SetSpindleOverride(Bit32 iCrdSysIndex, float32 dOverride)
{
    //待增加数据合法性检验


    m_CrdSysStateData[iCrdSysIndex].dSpindleOverride = dOverride;

    return 0;
}

/*
函 数 名：int IPO_GetCrdSysStateData(int iCrdSysIndex, IPO_STATE_DATA* pCrdSysStateData)
功    能：获取坐标系状态数据
参    数：iCrdSysIndex——坐标系索引，取值0-1
          *pCrdSysStateData——坐标系状态数据（出参）
返 回 值：成功返回0，否则返回非0值
调用位置：应用层调用
注意事项：无
*/
uBit32 CSM_GetCrdSysStateData(Bit32 iCrdSysIndex, CRDSYS_STATE_DATA* pCrdSysStateData)
{
    *pCrdSysStateData = m_CrdSysStateData[iCrdSysIndex];

    return 0;
}

/*
函 数 名：int IPO_SetSimulationEnable(int iEnable)
功    能：打开或关闭校验运行模式
参    数：iCrdSysIndex——坐标系索引，取值0-1
          iEnable——1-打开校验模式，0-关闭校验模式
返 回 值：成功返回0，否则返回非0值
调用位置：
注意事项：坐标系程序未停止或手动运行未停止时，不能设置该模式
*/
uBit32 CSM_EnableVerify(Bit32 iCrdSysIndex, Bit32 iEnable)
{
    uBit32 ulErrorCode;
    unsigned long ulCrdCtrlMode;

    //
    ulCrdCtrlMode = CSM_GetCtrlMode(iCrdSysIndex);
    if(ulCrdCtrlMode != CRDSYS_STATUS_MODE_AUTO)
    {
        ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, iCrdSysIndex, CSM_ERR_CRDSYS_MODE_ERROR, 
            CSM_CMD_SET_VERIFY, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);

        return ulErrorCode;
    }

    //
    if (!CSM_CheckCrdSysFree(iCrdSysIndex))
    {
        ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, iCrdSysIndex, CSM_ERR_CRDSYS_NOT_FREE, 
            CSM_CMD_SET_VERIFY, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }

    //
    ulErrorCode = m_CSMInterface.pf_IPO_SetVerifyEnable(iCrdSysIndex, iEnable);
    if (ulErrorCode != 0)
    {
        return ulErrorCode;
    }

    //
    if (iEnable)
    {
        m_CrdSysStateData[iCrdSysIndex].ulStatus |= CRDSYS_STATUS_VERIFY;
    }
    else
    {
        m_CrdSysStateData[iCrdSysIndex].ulStatus &= ~CRDSYS_STATUS_VERIFY;
    }

    return 0;
}

uBit32 CSM_EnableSingleSegmentMode(Bit32 iCrdSysIndex, Bit32 iEnable)
{
    uBit32 ulErrorCode;
    unsigned long ulCrdCtrlMode;

    //
    ulCrdCtrlMode = CSM_GetCtrlMode(iCrdSysIndex);
    if(ulCrdCtrlMode != CRDSYS_STATUS_MODE_AUTO)
    {
        ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, iCrdSysIndex, CSM_ERR_CRDSYS_MODE_ERROR, 
            CSM_CMD_SET_SINGLE_SEG_MODE, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);

        return ulErrorCode;
    }

    //
    ulErrorCode = m_CSMInterface.pf_IPO_EnableSingleSegmentMode(iCrdSysIndex, iEnable);
    if (ulErrorCode != 0)
    {
        return ulErrorCode;
    }

    //
    if (iEnable)
    {
        m_CrdSysStateData[iCrdSysIndex].ulCtrl |= CRDSYS_CTRL_ENABLE_SGL_BLK;
        m_CrdSysStateData[iCrdSysIndex].ulCtrl &= ~CRDSYS_CTRL_DISABLE_SGL_BLK;
    }
    else
    {
        m_CrdSysStateData[iCrdSysIndex].ulCtrl |= CRDSYS_CTRL_DISABLE_SGL_BLK;
        m_CrdSysStateData[iCrdSysIndex].ulCtrl &= ~CRDSYS_CTRL_ENABLE_SGL_BLK;
    }

    return 0;
}

/*
uBit32 CSM_EnableLookahead(Bit32 iCrdSysIndex, Bit32 iEnable)
{
    return 0;
}
*/

//
Bit32 CSM_CheckAxisAlarm(Bit32 iCrdSysIndex)
{
    int i;
    int iIndex;
    AXIS_STATE_DATA AxisStateData;

    for (i = 0; i < AXIS_MAX_COUNT; i++)
    {
        iIndex = m_CSM_CrdSysMotorIndex[iCrdSysIndex][i];
        if (iIndex < 0)
        {
            break;
        }

        m_CSMInterface.pf_PAX_GetStateData(iIndex, &AxisStateData);

        if (AxisStateData.ulRunningStatus & (AXIS_STATUS_ALARM | AXIS_STATUS_FAIL))
        {
            return 1;
        }
    }

    return 0;
}

/*
函 数 名：int IPO_ProgRun(unsigned short iCrdNOMask)
功    能：设置插补器运动程序开始执行
参    数：nCrdSysMask——开始执行程序的坐标系，bit0-bit1分别对应坐标系1和坐标系统2
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：无
*/
uBit32 CSM_ProgRun(uBit16 nCrdSysMask)
{
    int i, j;
    int iCrdSysAxisCount;
    unsigned short nBit;
    uBit32 ulStopCtrlBit;
    uBit32  ulErrorCode = 0;

    if (m_CSMInterface.pf_SYS_GetAlarmStatus())
    {
        ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, 0, CSM_ERR_SYS_ALARM_EXIST, 
            CSM_CMD_PROG_RUN, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }

    //检测能否启动程序
    for (i = 0; i < CRDSYS_MAX_COUNT; i++)
    {
        nBit =  0x0001 << i;
        if (!(nBit & nCrdSysMask))
        {
            continue;
        }

        //不在自动模式下
        if (CSM_GetCtrlMode(i) != CRDSYS_STATUS_MODE_AUTO)
        {
            ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, i, CSM_ERR_CRDSYS_MODE_ERROR, 
                CSM_CMD_PROG_RUN, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
            return ulErrorCode;
        }

        //
        if (m_CrdSysStateData[i].ulStatus & CRDSYS_STATUS_RESET)
        {
            ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, i, CSM_ERR_EXE_RESET, 
                CSM_CMD_PROG_RUN, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
            return ulErrorCode;
        }

        //
        if (m_CrdSysStateData[i].ulStatus & CRDSYS_STATUS_PROG_HOLD)
        { //程序进给保持使，允许存在CRDSYS_CTRL_PROG_STOP_AT_SEG_END的情况下再启动
            ulStopCtrlBit = CRDSYS_CTRL_PROG_STOP | CRDSYS_CTRL_PROG_ESTOP | CRDSYS_CTRL_PROG_CANCEL | CRDSYS_CTRL_PROG_HOLD;
        }
        else
        {
            ulStopCtrlBit = CRDSYS_CTRL_PROG_STOP | CRDSYS_CTRL_PROG_ESTOP | CRDSYS_CTRL_PROG_CANCEL | CRDSYS_CTRL_PROG_STOP_AT_SEG_END | CRDSYS_CTRL_PROG_HOLD;
        }
        
        if (m_CrdSysStateData[i].ulCtrl & ulStopCtrlBit)
        { //坐标系正在执行停止，不能再次启动程序
            ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, i, CSM_ERR_EXE_STOP, 
                CSM_CMD_PROG_RUN, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
            return ulErrorCode;
        }

        //
        if (m_CrdSysStateData[i].ulAlarm)
        {
            ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, i, CSM_ERR_CRDSYS_ALARM_EXIST, 
                CSM_CMD_PROG_RUN, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
            return ulErrorCode;
        }

        //
        if (CSM_CheckAxisAlarm(i))
        {
            ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, i, CSM_ERR_AXIS_ALARM_EXIST, 
                CSM_CMD_PROG_RUN, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
            return ulErrorCode;
        }

        //如果程序已启动，则无需再启动
        if (m_CrdSysStateData[i].ulStatus & CRDSYS_STATUS_PROG_RUN)
        {
            nCrdSysMask &= ~nBit;
            continue;
        }
    }

    //
    if (nCrdSysMask == 0)
    {
        return 0;
    }

    //
    ulErrorCode = m_CSMInterface.pf_IPO_ProgRun(nCrdSysMask);
    if (ulErrorCode != 0)
    {
        return ulErrorCode;
    }

    //
    for (i = 0; i < CRDSYS_MAX_COUNT; i++)
    {
        nBit =  0x0001 << i;
        if (!(nBit & nCrdSysMask))
        {
            continue;
        }

        //清上次开始行和结束行
        m_pIprStartLineCount[i]=0;//Alan Added at 2016-7-28
        m_pIprFinishLineCount[i]=0;//Alan Added at 2016-7-28

        m_CrdSysStateData[i].ulCtrl |= CRDSYS_CTRL_PROG_RUN;
        m_CrdSysStateData[i].ulStatus &= ~CRDSYS_STATUS_FREE;

        //
        if (m_CrdSysStateData[i].ulStatus & (CRDSYS_STATUS_PROG_STOP | CRDSYS_STATUS_PROG_CANCEL))
        {
            memset(m_CSM_AutoCtrl[i].dCrd, 0, sizeof(m_CSM_AutoCtrl[i].dCrd));

            iCrdSysAxisCount = CSM_GetCrdSysAxisCount(i);

            for (j = 0; j < iCrdSysAxisCount; j++)
            {
                m_CSM_AutoCtrl[i].dCrd[j] = CSM_GetAxisCmdPos(i, j);
            }
        }
    }

    return 0;
}

uBit32 CSM_ProgRun_IPR(Bit32 iCrdSysIndex)
{
    return CSM_ProgRun(0x0001 << iCrdSysIndex);
}

/*
函 数 名：int IPO_ProgStop(int nCrdSysMask);
功    能：设置插补器停止
参    数：nCrdSysMask——bit0-bit1分别对应坐标系1和坐标系统2
返 回 值：成功返回0，否则返回非0值
调用位置：
注意事项：无
*/
uBit32    CSM_ProgStop(uBit16 nCrdSysMask)
{
    int i;
    uBit32 ulErrorCode;
    unsigned short nBit;
    //
    for (i = 0; i < CRDSYS_MAX_COUNT; i++)
    {
        nBit =  0x0001 << i;
        if (!(nBit & nCrdSysMask))
        {
            continue;
        }

        //不在自动模式下
        if (CSM_GetCtrlMode(i) != CRDSYS_STATUS_MODE_AUTO)
        {
            ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, i, CSM_ERR_CRDSYS_MODE_ERROR, 
                CSM_CMD_PROG_STOP, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
            return ulErrorCode;
        }

        if ((m_CrdSysStateData[i].ulStatus & (CRDSYS_STATUS_PROG_STOP | CRDSYS_STATUS_PROG_CANCEL)) && !(m_CrdSysStateData[i].ulCtrl & CRDSYS_CTRL_PROG_MASK))
        {
            if((m_CrdSysStateData[i].ulCtrl & CRDSYS_CTRL_IPR_PAUSE)==0)
                m_CrdSysStateData[i].ulUserLineNO=0;

            nCrdSysMask &= ~nBit;
            continue;
        }

        //
        if (m_CrdSysStateData[i].ulCtrl & (CRDSYS_CTRL_PROG_STOP | CRDSYS_CTRL_PROG_ESTOP | CRDSYS_CTRL_PROG_CANCEL))
        {
            nCrdSysMask &= ~nBit;

            continue;
        }
    }

    //
    if (nCrdSysMask == 0)
    {
        return 0;
    }

    //
    ulErrorCode = m_CSMInterface.pf_IPO_ProgStop(nCrdSysMask);
    if (ulErrorCode != 0)
    {
        return ulErrorCode;
    }


    //
    for (i = 0; i < CRDSYS_MAX_COUNT; i++)
    {
        nBit =  0x0001 << i;
        if (!(nBit & nCrdSysMask))
        {
            continue;
        }
        
        m_CrdSysStateData[i].ulCtrl &= ~CRDSYS_CTRL_PROG_MASK;
        m_CrdSysStateData[i].ulCtrl |= CRDSYS_CTRL_PROG_STOP;
    }

    return 0;
}

uBit32 CSM_ProgStop_IPR(Bit32 iCrdSysIndex)
{
    return CSM_ProgStop(0x0001 << iCrdSysIndex);
}


uBit32 CSM_AllChProgEStop()
{
    return CSM_ProgEStop(CSM_GetAllCrdSysMask());
}

/*
函 数 名：void IPO_ProgEStop(unsigned short nCrdSysMask)
功    能：设置插补器急停
参    数：nCrdSysMask——bit0-bit1分别对应坐标系1和坐标系统2
返 回 值：无
调用位置：
注意事项：无
*/
uBit32 CSM_ProgEStop(uBit16 nCrdSysMask)
{
    int i;
    uBit32 ulErrorCode;
    unsigned short nBit;


    //
    for (i = 0; i < CRDSYS_MAX_COUNT; i++)
    {
        nBit =  0x0001 << i;
        if (!(nBit & nCrdSysMask))
        {
            continue;
        }

        //不在自动模式下
        if (CSM_GetCtrlMode(i) != CRDSYS_STATUS_MODE_AUTO)
        {
            //ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, i, CSM_ERR_CRDSYS_MODE_ERROR, 
            //    CSM_CMD_PROG_STOP, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
            //return ulErrorCode;

            nCrdSysMask &= ~nBit;
            continue;
        }

        //
        if ((m_CrdSysStateData[i].ulStatus & (CRDSYS_STATUS_PROG_STOP | CRDSYS_STATUS_PROG_CANCEL)) && !(m_CrdSysStateData[i].ulCtrl & CRDSYS_CTRL_PROG_MASK))
        {
            nCrdSysMask &= ~nBit;

            continue;
        }

        //
        if (m_CrdSysStateData[i].ulCtrl & CRDSYS_CTRL_PROG_ESTOP)
        {
            nCrdSysMask &= ~nBit;
            continue;
        }
    }

    //
    if (nCrdSysMask == 0)
    {
        return 0;
    }

    //
    ulErrorCode = m_CSMInterface.pf_IPO_ProgEStop(nCrdSysMask);
    if (ulErrorCode != 0)
    {
        return ulErrorCode;
    }

    //
    for (i = 0; i < CRDSYS_MAX_COUNT; i++)
    {
        nBit =  0x0001 << i;
        if (!(nBit & nCrdSysMask))
        {
            continue;
        }

        m_CrdSysStateData[i].ulCtrl &= ~CRDSYS_CTRL_PROG_MASK;
        m_CrdSysStateData[i].ulCtrl |= CRDSYS_CTRL_PROG_ESTOP;
    }

    return 0;
}

/*
函 数 名：int IPO_ProgSegmentEndStop(int nCrdSysMask);
功    能：设置当前段末尾结束执行停止
参    数：nCrdSysMask——bit0-bit1分别对应坐标系1和坐标系统2
返 回 值：成功返回0，否则返回非0值
调用位置：
注意事项：无
*/
uBit32    CSM_ProgSegmentEndStop(uBit16 nCrdSysMask)
{
    int i;
    uBit32 ulErrorCode;
    unsigned short nBit;

    //
    for (i = 0; i < CRDSYS_MAX_COUNT; i++)
    {
        nBit =  0x0001 << i;
        if (!(nBit & nCrdSysMask))
        {
            continue;
        }

        //不在自动模式下
        if (CSM_GetCtrlMode(i) != CRDSYS_STATUS_MODE_AUTO)
        {
            ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, i, CSM_ERR_CRDSYS_MODE_ERROR, 
                CSM_CMD_PROG_SEGMENT_END_STOP, 0, ERR_LEVEL_NORMAL);
            return ulErrorCode;
        }

        //
        if ((m_CrdSysStateData[i].ulStatus & (CRDSYS_STATUS_PROG_STOP | CRDSYS_STATUS_PROG_CANCEL)) 
            && !(m_CrdSysStateData[i].ulCtrl & CRDSYS_CTRL_PROG_MASK))
        {
            nCrdSysMask &= ~nBit;
            continue;
        }

        //
        if (m_CrdSysStateData[i].ulCtrl & (CRDSYS_CTRL_PROG_STOP | CRDSYS_CTRL_PROG_ESTOP | CRDSYS_CTRL_PROG_CANCEL | CRDSYS_CTRL_PROG_STOP_AT_SEG_END))
        {
            nCrdSysMask &= ~nBit;
            continue;
        }
    }

    //
    if (nCrdSysMask == 0)
    {
        return 0;
    }

    //
    ulErrorCode = m_CSMInterface.pf_IPO_ProgSegmentEndStop(nCrdSysMask);
    if (ulErrorCode != 0)
    {
        return ulErrorCode;
    }

    //
    for (i = 0; i < CRDSYS_MAX_COUNT; i++)
    {
        nBit =  0x0001 << i;
        if (!(nBit & nCrdSysMask))
        {
            continue;
        }

        m_CrdSysStateData[i].ulCtrl &= ~CRDSYS_CTRL_PROG_MASK;
        m_CrdSysStateData[i].ulCtrl |= CRDSYS_CTRL_PROG_STOP_AT_SEG_END;
    }

    return 0;
}


/*
函 数 名：int    IPO_ProgHold(unsigned short nCrdSysMask)
功    能：设置插补器暂停
参    数：nCrdSysMask——bit0-bit1分别对应坐标系1和坐标系统2
返 回 值：成功返回0，否则返回非0值
调用位置：
注意事项：无
*/
uBit32    CSM_ProgHold(uBit16 nCrdSysMask)
{
    int i;
    uBit32 ulErrorCode;
    unsigned short nBit;

    //
    for (i = 0; i < CRDSYS_MAX_COUNT; i++)
    {
        nBit =  0x0001 << i;
        if (!(nBit & nCrdSysMask))
        {
            continue;
        }

        //不在自动模式下
        if (CSM_GetCtrlMode(i) != CRDSYS_STATUS_MODE_AUTO)
        {
            ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, i, CSM_ERR_CRDSYS_MODE_ERROR, 
                CSM_CMD_PROG_HOLD, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
            return ulErrorCode;
        }

        if ((m_CrdSysStateData[i].ulStatus & (CRDSYS_STATUS_PROG_STOP | CRDSYS_STATUS_PROG_HOLD | CRDSYS_STATUS_PROG_CANCEL)) 
            && !(m_CrdSysStateData[i].ulCtrl & CRDSYS_CTRL_PROG_MASK))
        {
            nCrdSysMask &= ~nBit;
            continue;
        }

        //
        if (m_CrdSysStateData[i].ulCtrl & (CRDSYS_CTRL_PROG_STOP | CRDSYS_CTRL_PROG_ESTOP | CRDSYS_CTRL_PROG_CANCEL | CRDSYS_CTRL_PROG_HOLD))
        {
            continue;
        }
    }

    //
    if (nCrdSysMask == 0)
    {
        return 0;
    }

    //
    ulErrorCode = m_CSMInterface.pf_IPO_ProgHold(nCrdSysMask);
    if (ulErrorCode != 0)
    {
        return ulErrorCode;
    }

    //
    for (i = 0; i < CRDSYS_MAX_COUNT; i++)
    {
        nBit =  0x0001 << i;
        if (!(nBit & nCrdSysMask))
        {
            continue;
        }

        m_CrdSysStateData[i].ulCtrl &= ~CRDSYS_CTRL_PROG_MASK;
        m_CrdSysStateData[i].ulCtrl |= CRDSYS_CTRL_PROG_HOLD;
    }

    return 0;
}

/*
函 数 名：uBit32 CSM_ProgHold_IPR(uBit16 iCrdSysIndex)
功    能：程序暂停
参    数：iCrdSysIndex——暂停坐标系序号
返 回 值：成功返回0，否则返回非0值
调用位置：
注意事项：解释器暂停时调用，对于CSM来说实际上是执行停止动作
*/
uBit32    CSM_ProgHold_IPR(uBit16 iCrdSysIndex)
{
    uBit32 ulRet;

    ulRet = CSM_ProgStop(1<<iCrdSysIndex);

    if (ulRet)
        return ulRet;

    m_CrdSysStateData[iCrdSysIndex].ulCtrl |= CRDSYS_CTRL_IPR_PAUSE;
    return 0;
}


//清除程序执行状态
void CSM_ClearProgExeState(Bit32 iCrdSysIndex)
{
    m_CSM_AutoCtrl[iCrdSysIndex].ulCurProcessLineNO = 1;
    m_CSM_AutoCtrl[iCrdSysIndex].ulReleaseLineNO = 0;

    m_CrdSysStateData[iCrdSysIndex].ulCurProgLineNO = 0;
    //m_CrdSysStateData[iCrdSysIndex].ulUserLineNO = 0;\\Alan del at 2016-8-6
    m_CrdSysStateData[iCrdSysIndex].ulStartLineCount = 0;
    m_CrdSysStateData[iCrdSysIndex].ulFinishLineCount = 0;

    if (!(CSM_GetProgBufStatus(iCrdSysIndex) & DATA_BUF_STATUS_STATIC))
    { //动态模式下，清除程序缓冲区数据
        CSM_ClearProgData(iCrdSysIndex, 2);
    }
}

//程序取消
uBit32 CSM_ProgCancel(uBit16 nCrdSysMask)
{
    int i;
    uBit32 ulErrorCode;
    unsigned short nBit;

    //
    for (i = 0; i < CRDSYS_MAX_COUNT; i++)
    {
        nBit =  0x0001 << i;
        if (!(nBit & nCrdSysMask))
        {
            continue;
        }

        //不在自动模式下
        if (CSM_GetCtrlMode(i) != CRDSYS_STATUS_MODE_AUTO)
        {
            ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, i, CSM_ERR_CRDSYS_MODE_ERROR, 
                CSM_CMD_PROG_CANCEL, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
            return ulErrorCode;
        }

        //
        if ((m_CrdSysStateData[i].ulStatus & (CRDSYS_STATUS_PROG_STOP | CRDSYS_STATUS_PROG_CANCEL)) 
            && !(m_CrdSysStateData[i].ulCtrl & CRDSYS_CTRL_PROG_MASK))
        {
            nCrdSysMask &= ~nBit;
            CSM_ClearProgExeState(i);
            continue;
        }

        /*
        //
        if (m_CrdSysStateData[i].ulCtrl & (CRDSYS_CTRL_PROG_STOP | CRDSYS_CTRL_PROG_ESTOP | CRDSYS_CTRL_PROG_CANCEL))
        {
            nCrdSysMask &= ~nBit;

            continue;
        }
        */
        //////////////////////////////////////
        //
        if (m_CrdSysStateData[i].ulCtrl & CRDSYS_CTRL_PROG_CANCEL)
        {
            nCrdSysMask &= ~nBit;

            continue;
        }

        //
        if (m_CrdSysStateData[i].ulCtrl & CRDSYS_CTRL_PROG_ESTOP)
        {
            ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, i, CSM_ERR_EXE_ESTOP, 
                CSM_CMD_PROG_CANCEL, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
            return ulErrorCode;
        }

        //////////////////////////////////////
    }

    //
    if (nCrdSysMask == 0)
    {
        return 0;
    }

    //
    ulErrorCode = m_CSMInterface.pf_IPO_ProgStop(nCrdSysMask);
    if (ulErrorCode != 0)
    {
        return ulErrorCode;
    }


    //
    for (i = 0; i < CRDSYS_MAX_COUNT; i++)
    {
        nBit =  0x0001 << i;
        if (!(nBit & nCrdSysMask))
        {
            continue;
        }

        m_CrdSysStateData[i].ulCtrl &= ~CRDSYS_CTRL_PROG_MASK;
        m_CrdSysStateData[i].ulCtrl |= CRDSYS_CTRL_PROG_CANCEL;
    }

    return 0;
}


/*
函 数 名：void IPO_SetCrdSysAlarm(int iCrdSysIndex, unsigned long ulAlarmBit)
功    能：设置坐标系报警状态位
参    数：iCrdSysIndex——坐标系索引，取值0-1
          ulAlarmBit——要设置的报警状态位
返 回 值：无
调用位置：
注意事项：无
*/
void CSM_SetCrdSysAlarmBit(int iCrdSysIndex, unsigned long ulAlarmBit)
{
    m_CrdSysStateData[iCrdSysIndex].ulAlarm |= ulAlarmBit;
}

uBit32 CSM_GetCrdSysStatus(Bit32 iCrdSysIndex)
{
    return m_CrdSysStateData[iCrdSysIndex].ulStatus;
}

//获取当前运动段的目标位置
void CSM_GetCurLineTargetPos(Bit32 iCrdSysIndex, double64* pdPos, Bit32 iAxisCount)
{
    int i;

    if (iAxisCount>CRDSYS_AXIS_MAX_COUNT)
        iAxisCount = CRDSYS_AXIS_MAX_COUNT;

    for (i = 0; i < iAxisCount; i++)
    {
        pdPos[i] = m_CrdSysStateData[iCrdSysIndex].dTargetCrd[i];
    }
}

//获取当前指令位置
void CSM_GetCmdPos(Bit32 iCrdSysIndex, double64* pdPos, Bit32 iAxisCount)
{
    int i;

    if (iAxisCount>CRDSYS_AXIS_MAX_COUNT)
        iAxisCount = CRDSYS_AXIS_MAX_COUNT;

    for (i = 0; i < iAxisCount; i++)
    {
        LAX_GetCmdPos(iCrdSysIndex, i, pdPos + i, 1);
    }
}


//检测通道是否急停（存在不可恢复的错误），系统急停返回1，否则返回0
Bit32 CSM_CheckEStop(Bit32 iCrdSysIndex)
{
    int i;
    int iIndex;
    uBit32 ulStatus = 0;

    //
    if (m_CrdSysStateData[iCrdSysIndex].ulAlarm)
    {
        return 1;
    }

    //
    for (i = 0; i < AXIS_MAX_COUNT; i++)
    {
        iIndex = m_CSM_CrdSysMotorIndex[iCrdSysIndex][i];
        if (iIndex < 0)
        {
            break;
        }

        ulStatus = m_CSMInterface.pf_PAX_GetRunningStatus(iIndex);
        if (ulStatus & (AXIS_STATUS_ALARM | AXIS_STATUS_FAIL))
        {
            return 1;
        }
    }

    return 0;
}

//检测通道中指定的轴是否在运动，返回值中对应位为0表示没有轴在移动，对应位非零表示还在运动
uBit32 CSM_CheckAxisMoveState(Bit32 iCrdSysIndex, uBit32 ulAxisMask)
{
    int i;
    int iAxisCount;
    uBit32 ulAxisMoveState = 0;

    iAxisCount = CSM_GetCrdSysAxisCount(iCrdSysIndex);
    for (i = 0; i < iAxisCount; i++)
    {
        if (!LAX_CheckAxisStop(iCrdSysIndex, i))
        {
            ulAxisMoveState |= (0x0001 << i);
        }    
    }

    return ulAxisMoveState;
}

//检测程序是否取消
Bit32 CSM_CheckProgCancel(Bit32 iCrdSysIndex)
{
    if (m_CrdSysStateData[iCrdSysIndex].ulStatus & CRDSYS_STATUS_PROG_CANCEL)
    {
        return 1;
    }

    return 0;
}

//检测循环启动
Bit32 CSM_CheckProgRun(Bit32 iCrdSysIndex)
{
    if (m_CrdSysStateData[iCrdSysIndex].ulStatus & CRDSYS_STATUS_PROG_RUN)
    {
        return 1;
    }

    return 0;
}


//获取当前跳段设置（返回值表示跳段屏蔽字）
Bit32 CSM_GetSkipSetting(Bit32 iCrdSysIndex)
{
    return 0;        //跳段屏蔽字右外部输入（后续完善）
}

//获取开始的行数量
Bit32 CSM_GetStartLineCount(Bit32 iCrdSysIndex)
{
    return m_CrdSysStateData[iCrdSysIndex].ulStartLineCount;
}

//获取完成的行数量
Bit32 CSM_GetFinishLineCount(Bit32 iCrdSysIndex)
{
    return m_CrdSysStateData[iCrdSysIndex].ulFinishLineCount;
}



const CRDSYS_STATE_DATA* CSM_GetCrdSysStateReadAddr(Bit32 iCrdSysIndex)
{
    return m_CrdSysStateData + iCrdSysIndex;
}


uBit32 CSM_SpindleRun(Bit32 iCrdSysIndex, Bit32 iDir, uBit32 ulSpeed)
{
    return 0;
}

uBit32 CSM_SpindleStop(Bit32 iCrdSysIndex)
{
    return 0;
}


//获取通道的用户自定义32位输入缓冲区地址
uBit32* CSM_GetUserinAddr(Bit32 iCrdSysIndex)
{
    return &(m_CrdSysStateData[iCrdSysIndex].ulUserIn);
}

//获取通道的用户自定义32位输出缓冲区地址
uBit32* CSM_GetUserOutAddr(Bit32 iCrdSysIndex)
{
    return &(m_CrdSysStateData[iCrdSysIndex].ulUserOut);
}

//Alan Deleted at 2016-8-23
//由模拟器实现
/*
//保存插补器缓存数据（供解释器使用）
Bit32 CSM_SaveProgData(Bit32 iCrdSysIndex ,Bit8 *buf)
{
    return 0;
}

//装载插补器缓存数据（供解释器使用）
Bit32 CSM_LoadProgData(Bit32 iCrdSysIndex ,Bit8 *buf)
{
    return 0;
}*/
