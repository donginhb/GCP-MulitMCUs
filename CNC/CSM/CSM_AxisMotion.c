/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：CSM_AxisMotion.c

摘    要：轴运动指令接口函数

运行环境：LPC43xx 

修改描述：

当前版本：1.0

修改作者：毛军

修改日期：2015年10月13日

*/

#include "string.h"
#include "CSM_AxisMotion.h"
#include "CSM_CrdSysState.h"
#include "CSM_MainCtrl.h"
#include "CSM_Interface.h"
#include "CSM_ParmMan.h"

#include "CNC/DataStructDef/DataType.h"
#include "CNC/DataStructDef/IPO_DataStructDefPub.h"
#include "CNC/DataStructDef/ERR_DataStructDef.h"
#include "CNC/ErrorCodeDef/CSM_ErrorCodeDef.h"



extern CSM_INTERFACE    m_CSMInterface;


/*
函 数 名：uBit32 CSM_MoveAxis(Bit32 iCrdSysIndex, Bit32* pAxisPos, Bit32 iAxisCount, Bit32 iSpeed)
功    能：将坐标轴移动到指定位置（坐标系重定位）
参    数：iCrdSysIndex --坐标系索引
          pAxisPos --坐标轴目标位置数据起始地址
          iAxisCount --坐标轴数量
          iSpeed --运动速度（坐标轴的合成速度）
返 回 值：成功返回0，否则返回错误码
调用位置：应用层
注意事项：调用本函数时，坐标系必须在自动模式下，且为空闲状态；系统才用G01的方式完成移动
*/
uBit32 CSM_MoveAxis(Bit32 iCrdSysIndex, Bit32* pAxisPos, Bit32 iAxisCount, Bit32 iSpeed)
{
    SEGMENT_MOTION_DATA    MotionData;
    uBit32 ulErrorCode;
    int i;

    //
    if(!CSM_CheckCrdSysFree(iCrdSysIndex))
    {
        ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, iCrdSysIndex, CSM_ERR_CRDSYS_NOT_FREE, 
            CSM_CMD_CRDSYS_LOCATION, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);

        return ulErrorCode;
    }

    //
    if (CSM_GetCtrlMode(iCrdSysIndex) != CRDSYS_STATUS_MODE_AUTO)
    {
        ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, iCrdSysIndex, CSM_ERR_CRDSYS_MODE_ERROR, 
            CSM_CMD_CRDSYS_LOCATION, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }

    //启动插补器
    ulErrorCode = m_CSMInterface.pf_IPO_ProgRun(0x0001 << iCrdSysIndex);
    if (ulErrorCode != 0)
    {
        return ulErrorCode;
    }
    
    //发送运动段到插补器
    memset(&MotionData, 0, sizeof(MotionData));

    MotionData.ulProgLineNO = 0;
    MotionData.ulUserLineNO = 0;
    MotionData.nMotionMode = MOTION_MODE_INTERPOLATION;
    MotionData.IPO.nCmdType = MOTION_CMD_LINE;

    for (i = 0; i < iAxisCount; i++)
    {
        MotionData.IPO.dCrd[i] = pAxisPos[i];
    }

    MotionData.IPO.dFeedRate = iSpeed;
    MotionData.IPO.dEndSpeed = 0.0;

    ulErrorCode = m_CSMInterface.pf_IPO_AddMotionDataToIPOBuf(iCrdSysIndex, &MotionData);
    if (ulErrorCode != 0)
    {
        m_CrdSysStateData[iCrdSysIndex].ulCtrl |= CRDSYS_CTRL_LOCATION;
        m_CrdSysStateData[iCrdSysIndex].ulStatus &= ~CRDSYS_STATUS_FREE;
        return ulErrorCode;
    }

    m_CrdSysStateData[iCrdSysIndex].ulCtrl |= CRDSYS_CTRL_LOCATION;
    m_CrdSysStateData[iCrdSysIndex].ulStatus &= ~CRDSYS_STATUS_FREE;

    return 0;
}


/*
函 数 名：int CSM_SetPosCtrlMotion(int iCrdSysIndex, int iAxisIndex, POSCTRL_MOTION_DATA* pPosCtrlMotion);
功    能：设置坐标轴按位置控制方式运动
参    数：iCrdSysIndex --通道索引
          iAxisIndex --坐标轴索引
          *pPosCtrlMotion --位置控制运动参数
返 回 值：成功返回0，否则返回错误码
调用位置：应用层调用
注意事项：如果系统不在手动模式下，不能执行该指令
*/
/*
uBit32 CSM_SetAxisPosCtrlMotion(Bit32 iCrdSysIndex, Bit32 iAxisIndex, POSCTRL_MOTION_DATA* pPosCtrlMotion)
{
    uBit32 ulErrorCode;

    //
    if (CSM_CheckResetState(iCrdSysIndex))
    {
        ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, iCrdSysIndex, CSM_ERR_EXE_RESET, 
            CSM_CMD_AXIS_POS_CTRL_MOTION, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }
    
    //
    if (CSM_GetCtrlMode(iCrdSysIndex) != CRDSYS_STATUS_MODE_STEP)
    {
        ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, iCrdSysIndex, CSM_ERR_CRDSYS_MODE_ERROR, 
            CSM_CMD_AXIS_POS_CTRL_MOTION, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }

    //
    if (m_CSMInterface.pf_SYS_GetAlarmStatus())
    {
        ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, iCrdSysIndex, CSM_ERR_SYS_ALARM_EXIST, 
            CSM_CMD_AXIS_POS_CTRL_MOTION, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }

    //
    ulErrorCode = m_CSMInterface.pf_JOG_SetAxisPosCtrlMotion(iCrdSysIndex, iAxisIndex, pPosCtrlMotion);
    if (ulErrorCode != 0)
    {
        return ulErrorCode;
    }

    m_CrdSysStateData[iCrdSysIndex].ulStatus &= ~CRDSYS_STATUS_FREE;

    return 0;
}*/

//检查电机状态是否允许手动运动，可手动运动返回0，否则返回错误码
uBit32 CSM_CheckMotorReadyForJog(Bit32 iMotorNO, uBit32 ulMotionCmdType, POSCTRL_MOTION_DATA* pPosCtrlMotion, SPEEDCTRL_MOTION_DATA* pSpeedCtrlMotion)
{
    AXIS_STATE_DATA AxisStateData;
    uBit32 ulErrorCode;
    uBit32 ulStatus;
    uBit32 ulMoveLimitMask = AXIS_STATUS_LEFT_SOFT_LIMIT | AXIS_STATUS_LEFT_LIMIT | AXIS_STATUS_RIGHT_SOFT_LIMIT | AXIS_STATUS_RIGHT_LIMIT;
    uBit32 ulMustStopAlarm = 0;

    //检查电机是否使能
    ulStatus = m_CSMInterface.pf_PAX_GetRunningStatus(iMotorNO);

    if (!(ulStatus & AXIS_STATUS_ENABLE))
    {
        ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, ERR_CRD_NO_INVALID, CSM_ERR_MOTOR_DISABLE, 
            ulMotionCmdType, iMotorNO, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }

    //检查电机是否存在报警
    if (ulStatus & AXIS_STATUS_FAIL)
    {
        ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, ERR_CRD_NO_INVALID, CSM_ERR_MOTOR_ALARM_EXIST, 
            ulMotionCmdType, iMotorNO, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }

    if (ulStatus & AXIS_STATUS_ALARM)
    {
        ulMustStopAlarm = ulStatus & AXIS_ALARM_STATUS_BIT_MASK;
        ulMustStopAlarm &= ~ulMoveLimitMask;

        if (ulMustStopAlarm > 0)
        {
            ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, ERR_CRD_NO_INVALID, CSM_ERR_MOTOR_ALARM_EXIST, 
                ulMotionCmdType, iMotorNO, ERR_LEVEL_NORMAL);
            return ulErrorCode;
        }
        else
        {
            if (ulMotionCmdType == CSM_CMD_AXIS_POS_CTRL_MOTION || ulMotionCmdType == CSM_CMD_MOTOR_POS_CTRL_MOTION)
            {
                m_CSMInterface.pf_PAX_GetStateData(iMotorNO, &AxisStateData);

                if (ulStatus & (AXIS_STATUS_LEFT_SOFT_LIMIT | AXIS_STATUS_LEFT_LIMIT))
                { //如果到达负限位
                    if (pPosCtrlMotion->dPos <= AxisStateData.dCmdPos)
                    {
                        ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, ERR_CRD_NO_INVALID, CSM_ERR_MOTOR_ALARM_EXIST, 
                            ulMotionCmdType, iMotorNO, ERR_LEVEL_NORMAL);
                        return ulErrorCode;
                    }
                }

                if (ulStatus & (AXIS_STATUS_RIGHT_SOFT_LIMIT | AXIS_STATUS_RIGHT_LIMIT))
                { //如果到达正限位
                    if (pPosCtrlMotion->dPos >= AxisStateData.dCmdPos)
                    {
                        ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, ERR_CRD_NO_INVALID, CSM_ERR_MOTOR_ALARM_EXIST, 
                            ulMotionCmdType, iMotorNO, ERR_LEVEL_NORMAL);
                        return ulErrorCode;
                    }
                }
            }
            else
            {
                if (ulStatus & (AXIS_STATUS_LEFT_SOFT_LIMIT | AXIS_STATUS_LEFT_LIMIT))
                { //如果到达负限位
                    if (pSpeedCtrlMotion->dSpeed < 0.0)
                    {
                        ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, ERR_CRD_NO_INVALID, CSM_ERR_MOTOR_ALARM_EXIST, 
                            ulMotionCmdType, iMotorNO, ERR_LEVEL_NORMAL);
                        return ulErrorCode;
                    }
                }

                if (ulStatus & (AXIS_STATUS_RIGHT_SOFT_LIMIT | AXIS_STATUS_RIGHT_LIMIT))
                { //如果到达正限位
                    if (pSpeedCtrlMotion->dSpeed > 0.0)
                    {
                        ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, ERR_CRD_NO_INVALID, CSM_ERR_MOTOR_ALARM_EXIST, 
                            ulMotionCmdType, iMotorNO, ERR_LEVEL_NORMAL);
                        return ulErrorCode;
                    }
                }

            }
        }
    }

    return 0;
}


/*
函 数 名：uBit32 CSM_SetAxisPosCtrlMotion(Bit32 iCrdSysIndex, uBit32 ulAxisMask, POSCTRL_MOTION_DATA* pPosCtrlMotion)
功    能：设置多个坐标轴按位置控制方式运动
参    数：iCrdSysIndex --通道索引
          ulAxisMask --坐标轴掩码
          pPosCtrlMotion --坐标轴位置控制运动参数起始地址
返 回 值：成功返回0，否则返回错误码
调用位置：应用层调用
注意事项：如果系统不在手动模式下，不能执行该指令
*/
uBit32 CSM_SetAxisPosCtrlMotion(Bit32 iCrdSysIndex, uBit32 ulAxisMask, POSCTRL_MOTION_DATA* pPosCtrlMotion)
{
    uBit32 ulErrorCode;
    int iCrdAxisCount;
    int iMotorNO;
    int iIndex;
    int i, j;

    //
    if (CSM_CheckResetState(iCrdSysIndex))
    {
        ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, iCrdSysIndex, CSM_ERR_EXE_RESET, 
            CSM_CMD_AXIS_POS_CTRL_MOTION, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }

    //
    if (CSM_GetCtrlMode(iCrdSysIndex) != CRDSYS_STATUS_MODE_STEP)
    {
        ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, iCrdSysIndex, CSM_ERR_CRDSYS_MODE_ERROR, 
            CSM_CMD_AXIS_POS_CTRL_MOTION, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }

    //
    if (m_CSMInterface.pf_SYS_GetAlarmStatus())
    {
        ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, iCrdSysIndex, CSM_ERR_SYS_ALARM_EXIST, 
            CSM_CMD_AXIS_POS_CTRL_MOTION, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }

    //检查是否存在通道报警
    for (i = 0; i < CRDSYS_MAX_COUNT; i++)
    {
        if (m_CrdSysStateData[i].ulAlarm)
        {
            ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, i, CSM_ERR_CRDSYS_ALARM_EXIST, 
                CSM_CMD_AXIS_POS_CTRL_MOTION, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
            return ulErrorCode;
        }
    }

    //检查各电机状态
    iIndex = 0;
    iCrdAxisCount = CSM_GetCrdSysAxisCount(iCrdSysIndex);

    for (i = 0; i < iCrdAxisCount; i++)
    {
        if (ulAxisMask & (0x0001 << i))
        {
            for (j = 0; j < AXIS_MAP_MOTOR_MAX_COUNT; j++)
            {
                iMotorNO = CSM_GetAxisMapMotorNO(iCrdSysIndex, i, j + 1);
                if (iMotorNO >= AXIS_MAX_COUNT)
                {
                    break;
                }

                //
                ulErrorCode = CSM_CheckMotorReadyForJog(iMotorNO, CSM_CMD_AXIS_POS_CTRL_MOTION, pPosCtrlMotion + iIndex, NULL);
                if (ulErrorCode != 0)
                {
                    return ulErrorCode;
                }

            }

            //
            iIndex++;
        }
    }

    //
    ulErrorCode = m_CSMInterface.pf_JOG_SetAxisPosCtrlMotion(iCrdSysIndex, ulAxisMask, pPosCtrlMotion);
    if (ulErrorCode != 0)
    {
        return ulErrorCode;
    }

    m_CrdSysStateData[iCrdSysIndex].ulStatus &= ~CRDSYS_STATUS_FREE;

    return 0;
}

/*
函 数 名：int JOG_SetSpeedCtrlMotion(int iCrdSysIndex, int iAxisIndex, SPEEDCTRL_MOTION_DATA* pSpeedCtrlMotion);
功    能：设置轴按速度控制方式运动
参    数：iCrdSysIndex --通道索引
          iAxisIndex --坐标轴索引
          *pSpeedCtrlMotion --速度控制运动参数
返 回 值：成功返回0，否则返回错误码
调用位置：应用层调用
注意事项：如果系统不在手动模式下，不能执行该指令
*/
/*
uBit32 CSM_SetAxisSpeedCtrlMotion(Bit32 iCrdSysIndex, Bit32 iAxisIndex, SPEEDCTRL_MOTION_DATA* pSpeedCtrlMotion)
{
    uBit32 ulErrorCode;

    //
    if (CSM_CheckResetState(iCrdSysIndex))
    {
        ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, iCrdSysIndex, CSM_ERR_EXE_RESET, 
            CSM_CMD_AXIS_SPEED_CTRL_MOTION, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }

    //
    if (CSM_GetCtrlMode(iCrdSysIndex) != CRDSYS_STATUS_MODE_JOG)
    {
        ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, iCrdSysIndex, CSM_ERR_CRDSYS_MODE_ERROR, 
            CSM_CMD_AXIS_SPEED_CTRL_MOTION, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }

    //
    if (m_CSMInterface.pf_SYS_GetAlarmStatus())
    {
        ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, iCrdSysIndex, CSM_ERR_SYS_ALARM_EXIST, 
            CSM_CMD_AXIS_SPEED_CTRL_MOTION, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }

    //
    ulErrorCode = m_CSMInterface.pf_JOG_SetAxisSpeedCtrlMotion(iCrdSysIndex, iAxisIndex, pSpeedCtrlMotion);
    if (ulErrorCode != 0)
    {
        return ulErrorCode;
    }

    m_CrdSysStateData[iCrdSysIndex].ulStatus &= ~CRDSYS_STATUS_FREE;

    return 0;

}*/

/*
函 数 名：uBit32 CSM_SetAxisSpeedCtrlMotion(Bit32 iCrdSysIndex, uBit32 ulAxisMask, SPEEDCTRL_MOTION_DATA* pSpeedCtrlMotion)
功    能：设置多个轴按速度控制方式运动
参    数：iCrdSysIndex --通道索引
          ulAxisMask --坐标轴掩码
          pSpeedCtrlMotion --坐标轴速度控制运动数据起始地址
返 回 值：成功返回0，否则返回错误码
调用位置：应用层调用
注意事项：如果系统不在手动模式下，不能执行该指令
*/
uBit32 CSM_SetAxisSpeedCtrlMotion(Bit32 iCrdSysIndex, uBit32 ulAxisMask, SPEEDCTRL_MOTION_DATA* pSpeedCtrlMotion)
{
    uBit32 ulErrorCode;
    int iCrdAxisCount;
    int iMotorNO;
    int iIndex;
    int i, j;

    //
    if (CSM_CheckResetState(iCrdSysIndex))
    {
        ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, iCrdSysIndex, CSM_ERR_EXE_RESET, 
            CSM_CMD_AXIS_SPEED_CTRL_MOTION, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }

    //
    if (CSM_GetCtrlMode(iCrdSysIndex) != CRDSYS_STATUS_MODE_JOG)
    {
        ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, iCrdSysIndex, CSM_ERR_CRDSYS_MODE_ERROR, 
            CSM_CMD_AXIS_SPEED_CTRL_MOTION, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }

    //
    if (m_CSMInterface.pf_SYS_GetAlarmStatus())
    {
        ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, iCrdSysIndex, CSM_ERR_SYS_ALARM_EXIST, 
            CSM_CMD_AXIS_SPEED_CTRL_MOTION, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }

    //检查是否存在通道报警
    for (i = 0; i < CRDSYS_MAX_COUNT; i++)
    {
        if (m_CrdSysStateData[i].ulAlarm)
        {
            ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, i, CSM_ERR_CRDSYS_ALARM_EXIST, 
                CSM_CMD_AXIS_SPEED_CTRL_MOTION, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
            return ulErrorCode;
        }
    }

    //检查各电机状态
    iIndex = 0;
    iCrdAxisCount = CSM_GetCrdSysAxisCount(iCrdSysIndex);

    for (i = 0; i < iCrdAxisCount; i++)
    {
        if (ulAxisMask & (0x0001 << i))
        {
            for (j = 0; j < AXIS_MAP_MOTOR_MAX_COUNT; j++)
            {
                iMotorNO = CSM_GetAxisMapMotorNO(iCrdSysIndex, i, j + 1);
                if (iMotorNO >= AXIS_MAX_COUNT)
                {
                    break;
                }

                //
                ulErrorCode = CSM_CheckMotorReadyForJog(iMotorNO, CSM_CMD_AXIS_SPEED_CTRL_MOTION, NULL, pSpeedCtrlMotion + iIndex);
                if (ulErrorCode != 0)
                {
                    return ulErrorCode;
                }
            }

            //
            iIndex++;
        }
    }


    //
    ulErrorCode = m_CSMInterface.pf_JOG_SetAxisSpeedCtrlMotion(iCrdSysIndex, ulAxisMask, pSpeedCtrlMotion);
    if (ulErrorCode != 0)
    {
        return ulErrorCode;
    }

    m_CrdSysStateData[iCrdSysIndex].ulStatus &= ~CRDSYS_STATUS_FREE;

    return 0;

}

/*
函 数 名：uBit32 CSM_SetAxisJogStop(Bit32 iCrdSysIndex, uBit32 ulAxisMask)
功    能：设置坐标轴手动运动停止
参    数：iCrdSysIndex --通道索引
          ulAxisMask --执行手动运动停止的坐标轴掩码
返 回 值：成功返回0，否则返回错误码
调用位置：应用层调用
注意事项：无
*/
uBit32 CSM_SetAxisJogStop(Bit32 iCrdSysIndex, uBit32 ulAxisMask)
{
    uBit32 ulErrorCode;
    uBit32 ulCtrlMode;
    uBit32 ulMotorMask;
    int iAxisCount;
    int i;

    //
    if (CSM_CheckResetState(iCrdSysIndex))
    {
        ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, iCrdSysIndex, CSM_ERR_EXE_RESET, 
            CSM_CMD_SET_AXIS_JOG_STOP, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }

    //
    ulCtrlMode = CSM_GetCtrlMode(iCrdSysIndex);
    if (ulCtrlMode != CRDSYS_STATUS_MODE_JOG && ulCtrlMode != CRDSYS_STATUS_MODE_STEP)
    {
        ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, iCrdSysIndex, CSM_ERR_CRDSYS_MODE_ERROR, 
            CSM_CMD_SET_AXIS_JOG_STOP, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }

    //
    iAxisCount = CSM_GetCrdSysAxisCount(iCrdSysIndex);
    ulMotorMask = 0;

    for (i = 0; i < iAxisCount; i++)
    {
        if (ulAxisMask & (0x0001 << i))
        {
            ulMotorMask |= CSM_GetAxisMapMotorMask(iCrdSysIndex, i);
        }
    }

    return m_CSMInterface.pf_JOG_SetMotionStop(ulMotorMask);

}

/*
函 数 名：uBit32 CSM_AxisJogEStop(Bit32 iCrdSysIndex, uBit32 ulAxisMask)
功    能：坐标轴手动运动急停
参    数：iCrdSysIndex --通道索引
          ulAxisMask --执行手动急停的坐标轴掩码
返 回 值：成功返回0，否则返回错误码
调用位置： 
注意事项：无
*/
uBit32 CSM_AxisJogEStop(Bit32 iCrdSysIndex, uBit32 ulAxisMask)
{
    uBit32 ulMotorMask;
    int iAxisCount;
    int i;

    //
    iAxisCount = CSM_GetCrdSysAxisCount(iCrdSysIndex);
    ulMotorMask = 0;

    for (i = 0; i < iAxisCount; i++)
    {
        if (ulAxisMask & (0x0001 << i))
        {
            ulMotorMask |= CSM_GetAxisMapMotorMask(iCrdSysIndex, i);
        }
    }

    m_CSMInterface.pf_JOG_MotorEStop(ulMotorMask);

    return 0;
}

/*
函 数 名：void CSM_CrdSysJogEStop(Bit32 iCrdSysIndex)
功    能：通道手动运动急停
参    数：无
返 回 值：无
调用位置： 
注意事项：无
*/
void CSM_CrdSysJogEStop(Bit32 iCrdSysIndex)
{
    uBit32 ulMotorMask;

    ulMotorMask = CSM_GetCrdSysMapMotorMask(iCrdSysIndex);

    m_CSMInterface.pf_JOG_MotorEStop(ulMotorMask);
}


/*
函 数 名：uBit32 CSM_SetMotorPosCtrlMotion(Bit32 iMotorNO, POSCTRL_MOTION_DATA* pPosCtrlMotion)
功    能：设置电机按位置控制方式运动
参    数：iMotorNO --电机编号，取值从0开始
          pPosCtrlMotion --电机运动参数
返 回 值：成功返回0，否则返回错误码
调用位置： 
注意事项：无
*/
uBit32 CSM_SetMotorPosCtrlMotion(Bit32 iMotorNO, POSCTRL_MOTION_DATA* pPosCtrlMotion)
{
    uBit32 ulErrorCode;
    int i;
    int iCrdSysIndex = -1;

    //检查是否存在系统报警
    if (m_CSMInterface.pf_SYS_GetAlarmStatus())
    {
        ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, ERR_CRD_NO_INVALID, CSM_ERR_SYS_ALARM_EXIST, 
            CSM_CMD_MOTOR_POS_CTRL_MOTION, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }

    //检查是否存在通道报警
    for (i = 0; i < CRDSYS_MAX_COUNT; i++)
    {
        if (m_CrdSysStateData[i].ulAlarm)
        {
            ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, i, CSM_ERR_CRDSYS_ALARM_EXIST, 
                CSM_CMD_MOTOR_POS_CTRL_MOTION, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
            return ulErrorCode;
        }
    }

    //
    ulErrorCode = CSM_CheckMotorReadyForJog(iMotorNO, CSM_CMD_MOTOR_POS_CTRL_MOTION, pPosCtrlMotion, NULL);
    if (ulErrorCode != 0)
    {
        return ulErrorCode;
    }

    //
    iCrdSysIndex = CSM_GetMotorMapCrdSysIndex(iMotorNO);

    if (iCrdSysIndex >= 0)
    { //电机已分配到坐标系
        //
        if (CSM_CheckResetState(iCrdSysIndex))
        {
            ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, iCrdSysIndex, CSM_ERR_EXE_RESET, 
                CSM_CMD_MOTOR_POS_CTRL_MOTION, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
            return ulErrorCode;
        }

        //
        if (CSM_GetCtrlMode(iCrdSysIndex) != CRDSYS_STATUS_MODE_STEP)
        {
            ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, iCrdSysIndex, CSM_ERR_CRDSYS_MODE_ERROR, 
                CSM_CMD_MOTOR_POS_CTRL_MOTION, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
            return ulErrorCode;
        }

        //
        ulErrorCode = m_CSMInterface.pf_JOG_SetMotorPosCtrlMotion(iMotorNO, pPosCtrlMotion);
        if (ulErrorCode != 0)
        {
            return ulErrorCode;
        }

        m_CrdSysStateData[iCrdSysIndex].ulStatus &= ~CRDSYS_STATUS_FREE;

        return 0;
    }
    else
    { //电机未分配到坐标系
        //检查电机是否正在回零
        if (m_CSMInterface.pf_PAX_GetRunningStatus(iMotorNO) & AXIS_STATUS_HOME_RUNNING)
        {
            ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, ERR_CRD_NO_INVALID, CSM_ERR_MOTOR_EXE_HOME, 
                CSM_CMD_MOTOR_POS_CTRL_MOTION, iMotorNO, ERR_LEVEL_NORMAL);
            return ulErrorCode;
        }

        //
        return m_CSMInterface.pf_JOG_SetMotorPosCtrlMotion(iMotorNO, pPosCtrlMotion);
    }
}

/*
函 数 名：uBit32 CSM_SetMotorSpeedCtrlMotion(Bit32 iMotorNO, SPEEDCTRL_MOTION_DATA* pSpeedCtrlMotion)
功    能：设置电机按速度控制方式运动
参    数：iMotorNO --电机编号，取值从0开始
          pSpeedCtrlMotion --电机运动参数
返 回 值：成功返回0，否则返回错误码
调用位置： 
注意事项：无
*/
uBit32 CSM_SetMotorSpeedCtrlMotion(Bit32 iMotorNO, SPEEDCTRL_MOTION_DATA* pSpeedCtrlMotion)
{
    uBit32 ulErrorCode;
    int i;
    int iCrdSysIndex = -1;

    //检查是否存在系统报警
    if (m_CSMInterface.pf_SYS_GetAlarmStatus())
    {
        ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, ERR_CRD_NO_INVALID, CSM_ERR_SYS_ALARM_EXIST, 
            CSM_CMD_MOTOR_SPEED_CTRL_MOTION, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }

    //检查是否存在通道报警
    for (i = 0; i < CRDSYS_MAX_COUNT; i++)
    {
        if (m_CrdSysStateData[i].ulAlarm)
        {
            ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, i, CSM_ERR_CRDSYS_ALARM_EXIST, 
                CSM_CMD_MOTOR_SPEED_CTRL_MOTION, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
            return ulErrorCode;
        }
    }

    //
    ulErrorCode = CSM_CheckMotorReadyForJog(iMotorNO, CSM_CMD_MOTOR_SPEED_CTRL_MOTION, NULL, pSpeedCtrlMotion);
    if (ulErrorCode != 0)
    {
        return ulErrorCode;
    }

    //
    iCrdSysIndex = CSM_GetMotorMapCrdSysIndex(iMotorNO);

    if (iCrdSysIndex >= 0)
    {
        //
        if (CSM_CheckResetState(iCrdSysIndex))
        {
            ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, iCrdSysIndex, CSM_ERR_EXE_RESET, 
                CSM_CMD_MOTOR_SPEED_CTRL_MOTION, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
            return ulErrorCode;
        }

        //
        if (CSM_GetCtrlMode(iCrdSysIndex) != CRDSYS_STATUS_MODE_JOG)
        {
            ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, iCrdSysIndex, CSM_ERR_CRDSYS_MODE_ERROR, 
                CSM_CMD_MOTOR_SPEED_CTRL_MOTION, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
            return ulErrorCode;
        }

        //
        ulErrorCode = m_CSMInterface.pf_JOG_SetMotorSpeedCtrlMotion(iMotorNO, pSpeedCtrlMotion);
        if (ulErrorCode != 0)
        {
            return ulErrorCode;
        }

        return 0;
    }
    else
    {
        //检查电机是否正在回零
        if (m_CSMInterface.pf_PAX_GetRunningStatus(iMotorNO) & AXIS_STATUS_HOME_RUNNING)
        {
            ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, ERR_CRD_NO_INVALID, CSM_ERR_MOTOR_EXE_HOME, 
                CSM_CMD_MOTOR_SPEED_CTRL_MOTION, iMotorNO, ERR_LEVEL_NORMAL);
            return ulErrorCode;
        }

        //
        return m_CSMInterface.pf_JOG_SetMotorSpeedCtrlMotion(iMotorNO, pSpeedCtrlMotion);
    }
}

/*
函 数 名：uBit32 CSM_SetMotorJogStop(Bit32 iMotorNO)
功    能：设置电机手动运动停止
参    数：iMotorNO --电机编号，取值从0开始
返 回 值：成功返回0，否则返回错误码
调用位置： 
注意事项：无
*/
uBit32 CSM_SetMotorJogStop(Bit32 iMotorNO)
{
    uBit32 ulErrorCode;
    uBit32 ulCtrlMode;
    int iCrdSysIndex = -1;

    iCrdSysIndex = CSM_GetMotorMapCrdSysIndex(iMotorNO);

    if (iCrdSysIndex >= 0)
    {
        //
        if (CSM_CheckResetState(iCrdSysIndex))
        {
            ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, iCrdSysIndex, CSM_ERR_EXE_RESET, 
                CSM_CMD_SET_MOTOR_JOG_STOP, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
            return ulErrorCode;
        }

        //
        ulCtrlMode = CSM_GetCtrlMode(iCrdSysIndex);
        if (ulCtrlMode != CRDSYS_STATUS_MODE_JOG && ulCtrlMode != CRDSYS_STATUS_MODE_STEP)
        {
            ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, iCrdSysIndex, CSM_ERR_CRDSYS_MODE_ERROR, 
                CSM_CMD_SET_MOTOR_JOG_STOP, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
            return ulErrorCode;
        }

        //
        ulErrorCode = m_CSMInterface.pf_JOG_SetMotionStop(0x0001 << iMotorNO);
        if (ulErrorCode != 0)
        {
            return ulErrorCode;
        }

        return 0;
    }
    else
    {
        //检查电机是否正在回零
        if (m_CSMInterface.pf_PAX_GetRunningStatus(iMotorNO) & AXIS_STATUS_HOME_RUNNING)
        {
            ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, ERR_CRD_NO_INVALID, CSM_ERR_MOTOR_EXE_HOME, 
                CSM_CMD_SET_MOTOR_JOG_STOP, iMotorNO, ERR_LEVEL_NORMAL);
            return ulErrorCode;
        }

        //
        return m_CSMInterface.pf_JOG_SetMotionStop(0x0001 << iMotorNO);
    }
}

/*
函 数 名：void CSM_MotorJogEStop(Bit32 iMotorNO)
功    能：设置电机手动运动急停
参    数：iMotorNO --电机编号，取值从0开始
返 回 值：成功返回0，否则返回错误码
调用位置： 
注意事项：无
*/
uBit32 CSM_MotorJogEStop(Bit32 iMotorNO)
{
    m_CSMInterface.pf_JOG_MotorEStop(0x0001 << iMotorNO);

    return 0;
}



