
/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：CSM_AxisStateMan.c 

摘    要：通道轴状态管理

运行环境：LPC43xx 

修改描述：

当前版本：3.0

修改作者：毛军

修改日期：2015年08月13日
*/

#include "math.h"
#include "string.h"
#include "CSM_Interface.h"
#include "CSM_AxisStateMan.h"
#include "CSM_ParmMan.h"
#include "CSM_CrdSysState.h"


#include "..\\DataStructDef\\DataType.h"
#include "..\\DataStructDef\\ERR_DataStructDef.h"
#include "..\\ErrorCodeDef\\CSM_ErrorCodeDef.h"

//
extern CSM_INTERFACE m_CSMInterface;


/*
函 数 名：void CSM_CloseAxisOutput(Bit32 iCrdSysIndex, uBit32 ulAxisMask)
功    能：设置坐标系的指定轴停止发脉冲
参    数：iCrdSysIndex --坐标系索引
          ulAxisMask --需要关闭的坐标轴掩码
返 回 值：无
调用位置：应用层调用
注意事项：
*/
void CSM_CloseAxisOutput(Bit32 iCrdSysIndex, uBit32 ulAxisMask)
{
    m_CSMInterface.pf_IPO_CloseAxisOutput(iCrdSysIndex, ulAxisMask);
}

/*
函 数 名：uBit32 CSM_GetAxisStateData(Bit32 iCrdSysIndex, Bit32 iAxisIndex, AXIS_STATE_DATA* pAxisStateData)
功    能：获取坐标轴轴状态数据
参    数：iCrdSysIndex --坐标系索引
          iAxisIndex --轴索引
          *pAxisStateData --轴状态数据（出参）
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：无
*/
uBit32 CSM_GetAxisStateData(Bit32 iCrdSysIndex, Bit32 iAxisIndex, AXIS_STATE_DATA* pAxisStateData)
{
    int iMotorNO;
    
    iMotorNO = CSM_GetAxisMapMotorNO(iCrdSysIndex, iAxisIndex, 1);

    return m_CSMInterface.pf_PAX_GetStateData(iMotorNO, pAxisStateData);
}


/*
函 数 名：double64 CSM_GetAxisCmdPos(Bit32 iCrdSysIndex, Bit32 iAxisIndex)
功    能：获取坐标轴指令位置
参    数：iCrdSysIndex --坐标系索引
          iAxisIndex --坐标轴索引
返 回 值：坐标轴指令位置
调用位置：
注意事项：
*/
double64 CSM_GetAxisCmdPos(Bit32 iCrdSysIndex, Bit32 iAxisIndex)
{
    AXIS_STATE_DATA AxisStateData;

    if(CSM_GetAxisStateData(iCrdSysIndex, iAxisIndex, &AxisStateData))
    {
        return 0.0;
    }
    else
    {
        return AxisStateData.dCmdPos;
    }
}


/*
函 数 名：uBit32 LAX_Enable(Bit32 iCrdSysIndex, Bit32 iAxisIndex, Bit32 iEnable, Bit32 iWaitExeResult)
功    能：设置轴使能
参    数：iCrdSysIndex --坐标系索引
          iAxisIndex --坐标轴索引
          iEnable --使能标记，0-断使能，1-加使能
          iWaitExeResult --标记是否等待执行结果，0-不等待，1-等待
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 LAX_Enable(Bit32 iCrdSysIndex, Bit32 iAxisIndex, Bit32 iEnable, Bit32 iWaitExeResult)
{
    int i;
    int iMotorNO;
    uBit32 ulErrorCode;

    //
    if(!CSM_CheckCrdSysFree(iCrdSysIndex))
    {
        ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, iCrdSysIndex, CSM_ERR_CRDSYS_NOT_FREE, 
            CSM_CMD_LAX_ENABLE, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);

        return ulErrorCode;
    }

    //
    for (i = 1; i <= AXIE_MAP_MOTOR_MAX_COUNT; i++)
    {
        iMotorNO = CSM_GetAxisMapMotorNO(iCrdSysIndex, iAxisIndex, i);
        if (iMotorNO >= 0 && iMotorNO < AXIS_MAX_COUNT)
        {
            ulErrorCode = m_CSMInterface.pf_PAX_Enable(iMotorNO, iEnable, iWaitExeResult);
            if (ulErrorCode != 0)
            {
                return ulErrorCode;
            }
        }
        else
        {
            break;
        }
    }

    return 0;
}



/*
函 数 名：uBit32 LAX_Home(Bit32 iCrdSysIndex, Bit32 iAxisIndex, Bit32 iHomeFlag, Bit32 iWaitExeResult)
功    能：设置轴回零
参    数：iCrdSysIndex --坐标系索引
          iAxisIndex --坐标轴索引
          iHomeFlag --回零标记，0-回零停止，1-回零开始
          iWaitExeResult --标记是否等待执行结果，0-不等待，1-等待
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：本函数回零停止执行成功表示回零停止指令成功发送，不表示回零动作已经停止，从回零停止指令成功发送到回零动作完全停止有一个过程；
*/
uBit32 LAX_Home(Bit32 iCrdSysIndex, Bit32 iAxisIndex, Bit32 iHomeFlag, Bit32 iWaitExeResult)
{
    int i;
    int iMotorNO;
    uBit32 ulErrorCode;

    //
    if (CSM_GetCtrlMode(iCrdSysIndex) != CRDSYS_STATUS_MODE_HOME)
    {
        ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, iCrdSysIndex, CSM_ERR_CRDSYS_MODE_ERROR, 
            CSM_CMD_LAX_HOME, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);

        return ulErrorCode;
    }

    //
    for (i = 1; i <= AXIE_MAP_MOTOR_MAX_COUNT; i++)
    {
        iMotorNO = CSM_GetAxisMapMotorNO(iCrdSysIndex, iAxisIndex, i);
        if (iMotorNO >= 0 && iMotorNO < AXIS_MAX_COUNT)
        {
            ulErrorCode = m_CSMInterface.pf_PAX_Home(iMotorNO, iHomeFlag, iWaitExeResult);
            if (ulErrorCode != 0)
            {
                return ulErrorCode;
            }
        }
        else
        {
            break;
        }
    }

    return 0;        //1个轴映射2个电机时，需要考虑回零运动的同步性，需要修改过通信协议，使多个电机可同时回零
}

/*
函 数 名：uBit32 LAX_SetCmdPos(Bit32 iCrdSysIndex, Bit32 iAxisIndex, Bit32 lCmdPos, Bit32 iWaitExeResult)
功    能：设置轴指令位置
参    数：iCrdSysIndex --坐标系索引
          iAxisIndex --坐标轴索引
          lCmdPos --指令位置，单位：um（旋转轴为0.001度）
          iWaitExeResult --标记是否等待执行结果，0-不等待，1-等待
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 LAX_SetCmdPos(Bit32 iCrdSysIndex, Bit32 iAxisIndex, Bit32 lCmdPos, Bit32 iWaitExeResult)
{
    int i;
    int iMotorNO;
    uBit32 ulErrorCode;

    //
    if(!CSM_CheckCrdSysFree(iCrdSysIndex))
    {
        ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, iCrdSysIndex, CSM_ERR_CRDSYS_NOT_FREE, 
            CSM_CMD_LAX_SET_CMD_POS, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);

        return ulErrorCode;
    }

    //
    for (i = 1; i <= AXIE_MAP_MOTOR_MAX_COUNT; i++)
    {
        iMotorNO = CSM_GetAxisMapMotorNO(iCrdSysIndex, iAxisIndex, i);
        if (iMotorNO >= 0 && iMotorNO < AXIS_MAX_COUNT)
        {
            ulErrorCode = m_CSMInterface.pf_PAX_SetCmdPos(iMotorNO, lCmdPos, iWaitExeResult);
            if (ulErrorCode != 0)
            {
                return ulErrorCode;
            }
        }
        else
        {
            break;
        }
    }

    return 0;
}

//added by alan at 2016-7-14
uBit32 CSM_SetCmdPos(Bit32 iCrdSysIndex,Bit32 *lCmdPos, Bit32 iAxisCount)
{
    uBit32 ulErr;
    
    if (iAxisCount>CRDSYS_AXIS_MAX_COUNT)
        iAxisCount = CRDSYS_AXIS_MAX_COUNT;

    for (int i=0; i<iAxisCount; i++)
    {
        ulErr = LAX_SetCmdPos(iCrdSysIndex,i,lCmdPos[i],1);

        if (ulErr)
            return ulErr;
    }

    return 0;
}

//07获取机床参考点位置参数的缓存地址，G28用 (YHZ 疑问回零时这个参数起何作用?)
Bit32* CSM_GetAxisRfPosAddr(Bit32 nCh,Bit32 iAxisNo)
{
    return NULL;
}

/*
函 数 名：uBit32 LAX_SetQeiPos(Bit32 iCrdSysIndex, Bit32 iAxisIndex, Bit32 lQeiPos, Bit32 iWaitExeResult)
功    能：设置轴编码器位置
参    数：iCrdSysIndex --坐标系索引
          iAxisIndex --坐标轴索引
          lQeiPos --编码器位置
          iWaitExeResult --标记是否等待执行结果，0-不等待，1-等待
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 LAX_SetQeiPos(Bit32 iCrdSysIndex, Bit32 iAxisIndex, Bit32 lQeiPos, Bit32 iWaitExeResult)
{
    int i;
    int iMotorNO;
    uBit32 ulErrorCode;

    //
    if(!CSM_CheckCrdSysFree(iCrdSysIndex))
    {
        ulErrorCode = m_CSMInterface.pf_ECM_GenerateErrorCode(SYS_SOFT_MODULE_CSM, iCrdSysIndex, CSM_ERR_CRDSYS_NOT_FREE, 
            CSM_CMD_LAX_SET_QEI_POS, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);

        return ulErrorCode;
    }

    //
    for (i = 1; i <= AXIE_MAP_MOTOR_MAX_COUNT; i++)
    {
        iMotorNO = CSM_GetAxisMapMotorNO(iCrdSysIndex, iAxisIndex, i);
        if (iMotorNO >= 0 && iMotorNO < AXIS_MAX_COUNT)
        {
            ulErrorCode = m_CSMInterface.pf_PAX_SetQeiPos(iMotorNO, lQeiPos, iWaitExeResult);
            if (ulErrorCode != 0)
            {
                return ulErrorCode;
            }
        }
        else
        {
            break;
        }
    }

    return 0;
}


/*
函 数 名：uBit32 LAX_CheckLastCmdExeState(Bit32 iCrdSysIndex, Bit32 iAxisIndex, uBit32* pCmdType)
功    能：检测坐标轴上一次的指令执行状态
参    数：iCrdSysIndex  --通道索引
          iAxisIndex --坐标轴索引
          *pCmdType --指令类型（出参）
返 回 值：执行成功返回0，否则返回错误码（执行失败、正在执行）
调用位置：
注意事项：
*/
/*
uBit32 LAX_CheckLastCmdExeState(Bit32 iCrdSysIndex, Bit32 iAxisIndex, uBit32* pCmdType)
{
    int i;
    int iMotorNO;
    uBit32 ulCmdType = 0;
    uBit32 ulErrorCode = 0;

    for (i = 1; i <= AXIE_MAP_MOTOR_MAX_COUNT; i++)
    {
        iMotorNO = CSM_GetAxisMapMotorNO(iCrdSysIndex, iAxisIndex, i);
        if (iMotorNO >= 0 && iMotorNO < AXIS_MAX_COUNT)
        {
            ulErrorCode = m_CSMInterface.pf_DEV_CheckLastCmdExeState(iMotorNO, &ulCmdType);
            if (ulErrorCode != 0)
            {
                *pCmdType = ulCmdType;
                return ulErrorCode;
            }
        }
        else
        {
            break;
        }
    }

    *pCmdType = ulCmdType;
    return ulErrorCode;
}
*/

/*
函 数 名：uBit32 LAX_CheckLastCmdExeState(Bit32 iCrdSysIndex, Bit32 iAxisIndex, uBit32* pCmdType)
功    能：检测坐标轴上一次的指令执行状态
参    数：iCrdSysIndex  --通道索引
          iAxisIndex --坐标轴索引
          *pCmdType --指令类型（出参）
          *pExeRes --指令执行结果（出参）
返 回 值：执行完成返回0，正在执行返回1
调用位置：
注意事项：
*/
uBit32 LAX_CheckLastCmdExeState(Bit32 iCrdSysIndex, Bit32 iAxisIndex, uBit32* pCmdType, uBit32 *pExeRes)
{
    int i;
    int iMotorNO;
    uBit32 ulExeProc=0;//0:执行完成 非0正在执行
    uBit32 ulCmdType=-1;

    *pExeRes = 0;

    for (i = 1; i <= AXIE_MAP_MOTOR_MAX_COUNT; i++)
    {
        iMotorNO = CSM_GetAxisMapMotorNO(iCrdSysIndex, iAxisIndex, i);

        if (iMotorNO >= 0 && iMotorNO < AXIS_MAX_COUNT)
        {
            ulExeProc += m_CSMInterface.pf_DEV_CheckLastCmdExeState(iMotorNO, pCmdType, pExeRes);

            if (ulExeProc)//
                return 1;

            if (*pExeRes)    //结果出错
                break;

            if (ulCmdType!=*pCmdType && (i>1))//不同指令
                return 1;

            ulCmdType = *pCmdType;
        }
        else
        {
            break;
        }
    }

    return 0;
}



/*
函 数 名：int CSM_GetAxisStateData(int iCrdSysIndex, int iAxisIndex, AXIS_STATE_DATA* pAxisStateData);
功    能：获取电机轴状态数据
参    数：iCrdSysIndex --通道索引
          iAxisIndex --坐标轴索引
          pAxisStateData --坐标轴状态数据缓冲区起始地址
          iStateDataLen --坐标轴状态数据缓冲区长度（表示可以存放多少个轴状态）
返 回 值：成功返回0，否则返回非0值
调用位置：应用层调用
注意事项：无
*/
uBit32 LAX_GetAxisStateData(Bit32 iCrdSysIndex, Bit32 iAxisIndex, AXIS_STATE_DATA* pAxisStateData, Bit32 iStateDataLen)
{
    int i;
    int iMotorNO;
    uBit32 ulErrorCode;

    for (i = 0; i < AXIE_MAP_MOTOR_MAX_COUNT; i++)
    {
        if (i >= iStateDataLen)
        {
            break;
        }

        //
        iMotorNO = CSM_GetAxisMapMotorNO(iCrdSysIndex, iAxisIndex, i+1);
        if (iMotorNO >= 0 && iMotorNO < AXIS_MAX_COUNT)
        {
            ulErrorCode = m_CSMInterface.pf_PAX_GetStateData(iMotorNO, pAxisStateData + i);
            if (ulErrorCode != 0)
            {
                return ulErrorCode;
            }
        }
        else
        {
            break;
        }
    }

    return 0;
}

/*
函 数 名：uBit32 LAX_GetCtrlStatus(Bit32 iCrdSysIndex, Bit32 iAxisIndex, uBit32* pulCtrlStatus, Bit32 iDataLen)
功    能：获取轴控制状态字
参    数：iCrdSysIndex --坐标系索引
          iAxisIndex --轴索引
          pulCtrlStatus --轴状态存储缓冲区地址
          iDataLen --轴状态存储缓冲区长度（表示可以存放多少轴的控制状态）
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 LAX_GetCtrlStatus(Bit32 iCrdSysIndex, Bit32 iAxisIndex, uBit32* pulCtrlStatus, Bit32 iDataLen)
{
    int i;
    int iMotorNO;
    uBit32 ulErrorCode;
    AXIS_STATE_DATA AxisStateData;

    for (i = 0; i < AXIE_MAP_MOTOR_MAX_COUNT; i++)
    {
        if (i >= iDataLen)
        {
            break;
        }

        //
        iMotorNO = CSM_GetAxisMapMotorNO(iCrdSysIndex, iAxisIndex, i+1);
        if (iMotorNO >= 0 && iMotorNO < AXIS_MAX_COUNT)
        {
            ulErrorCode = m_CSMInterface.pf_PAX_GetStateData(iMotorNO, &AxisStateData);
            if (ulErrorCode != 0)
            {
                return ulErrorCode;
            }
            else
            {
                pulCtrlStatus[i] = AxisStateData.ulCtrlStatus;
            }
        }
        else
        {
            break;
        }
    }

    return 0;
}

/*
函 数 名：uBit32 LAX_GetRunningStatus(Bit32 iCrdSysIndex, Bit32 iAxisIndex, uBit32* pulRunningStatus, Bit32 iDataLen)
功    能：获取轴运行状态字
参    数：iCrdSysIndex --坐标系索引
          iAxisIndex --轴索引
          pulRunningStatus --轴状态存储缓冲区地址
          iDataLen --轴状态存储缓冲区长度（表示可以存放多少轴的运行状态）
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 LAX_GetRunningStatus(Bit32 iCrdSysIndex, Bit32 iAxisIndex, uBit32* pulRunningStatus, Bit32 iDataLen)
{
    int i;
    int iMotorNO;
    uBit32 ulErrorCode;
    AXIS_STATE_DATA AxisStateData;

    for (i = 0; i < AXIE_MAP_MOTOR_MAX_COUNT; i++)
    {
        if (i >= iDataLen)
        {
            break;
        }

        //
        iMotorNO = CSM_GetAxisMapMotorNO(iCrdSysIndex, iAxisIndex, i+1);
        if (iMotorNO >= 0 && iMotorNO < AXIS_MAX_COUNT)
        {
            ulErrorCode = m_CSMInterface.pf_PAX_GetStateData(iMotorNO, &AxisStateData);
            if (ulErrorCode != 0)
            {
                return ulErrorCode;
            }
            else
            {
                pulRunningStatus[i] = AxisStateData.ulRunningStatus;
            }
        }
        else
        {
            break;
        }
    }

    return 0;
}

/*
函 数 名：uBit32 LAX_GetCmdPos(Bit32 iCrdSysIndex, Bit32 iAxisIndex, double64* pdCmdPos, Bit32 iDataLen)
功    能：获取轴指令位置
参    数：iCrdSysIndex --坐标系索引
          iAxisIndex --轴索引
          pdCmdPos --轴指令位置存储缓冲区地址
          iDataLen --轴指令位置存储缓冲区长度（表示可以存放多少轴的指令位置）
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 LAX_GetCmdPos(Bit32 iCrdSysIndex, Bit32 iAxisIndex, double64* pdCmdPos, Bit32 iDataLen)
{
    int i;
    int iMotorNO;
    uBit32 ulErrorCode;
    AXIS_STATE_DATA AxisStateData;

    for (i = 0; i < AXIE_MAP_MOTOR_MAX_COUNT; i++)
    {
        if (i >= iDataLen)
        {
            break;
        }

        //
        iMotorNO = CSM_GetAxisMapMotorNO(iCrdSysIndex, iAxisIndex, i+1);
        if (iMotorNO >= 0 && iMotorNO < AXIS_MAX_COUNT)
        {
            ulErrorCode = m_CSMInterface.pf_PAX_GetStateData(iMotorNO, &AxisStateData);
            if (ulErrorCode != 0)
            {
                return ulErrorCode;
            }
            else
            {
                pdCmdPos[i] = AxisStateData.dCmdPos;
            }
        }
        else
        {
            break;
        }
    }

    return 0;
}

/*
函 数 名：uBit32 LAX_GetPulsePos(Bit32 iCrdSysIndex, Bit32 iAxisIndex, double64* pdPulsePos, Bit32 iDataLen)
功    能：获取轴脉冲位置
参    数：iCrdSysIndex --坐标系索引
          iAxisIndex --轴索引
          pdPulsePos --轴脉冲位置存储缓冲区地址
          iDataLen --轴脉冲位置存储缓冲区长度（表示可以存放多少轴的脉冲位置）
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 LAX_GetPulsePos(Bit32 iCrdSysIndex, Bit32 iAxisIndex, double64* pdPulsePos, Bit32 iDataLen)
{
    int i;
    int iMotorNO;
    uBit32 ulErrorCode;
    AXIS_STATE_DATA AxisStateData;

    for (i = 0; i < AXIE_MAP_MOTOR_MAX_COUNT; i++)
    {
        if (i >= iDataLen)
        {
            break;
        }

        //
        iMotorNO = CSM_GetAxisMapMotorNO(iCrdSysIndex, iAxisIndex, i+1);
        if (iMotorNO >= 0 && iMotorNO < AXIS_MAX_COUNT)
        {
            ulErrorCode = m_CSMInterface.pf_PAX_GetStateData(iMotorNO, &AxisStateData);
            if (ulErrorCode != 0)
            {
                return ulErrorCode;
            }
            else
            {
                pdPulsePos[i] = AxisStateData.dPulsePos;
            }
        }
        else
        {
            break;
        }
    }

    return 0;
}

/*
函 数 名：uBit32 LAX_GetQeiPos(Bit32 iCrdSysIndex, Bit32 iAxisIndex, Bit32* plQeiPos, Bit32 iDataLen)
功    能：获取轴编码器位置
参    数：iCrdSysIndex --坐标系索引
          iAxisIndex --轴索引
          plQeiPos --轴编码器位置存储缓冲区地址
          iDataLen --轴编码器位置存储缓冲区长度（表示可以存放多少轴的编码器位置）
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 LAX_GetQeiPos(Bit32 iCrdSysIndex, Bit32 iAxisIndex, Bit32* plQeiPos, Bit32 iDataLen)
{
    int i;
    int iMotorNO;
     uBit32 ulErrorCode;
    AXIS_STATE_DATA AxisStateData;

    for (i = 0; i < AXIE_MAP_MOTOR_MAX_COUNT; i++)
    {
        if (i >= iDataLen)
        {
            break;
        }

        //
        iMotorNO = CSM_GetAxisMapMotorNO(iCrdSysIndex, iAxisIndex, i+1);
        if (iMotorNO >= 0 && iMotorNO < AXIS_MAX_COUNT)
        {
            ulErrorCode = m_CSMInterface.pf_PAX_GetStateData(iMotorNO, &AxisStateData);
            if (ulErrorCode != 0)
            {
                return ulErrorCode;
            }
            else
            {
                plQeiPos[i] = AxisStateData.lQeiPos;
            }
        }
        else
        {
            break;
        }
    }

    return 0;
}

/*
函 数 名：uBit32 LAX_GetCmdSpeed(Bit32 iCrdSysIndex, Bit32 iAxisIndex, float32* pdCmdSpeed, Bit32 iDataLen)
功    能：获取轴指令速度
参    数：iCrdSysIndex --坐标系索引
          iAxisIndex --轴索引
          pdCmdSpeed --轴指令速度存储缓冲区地址
          iDataLen --轴指令速度存储缓冲区长度（表示可以存放多少轴的指令速度）
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 LAX_GetCmdSpeed(Bit32 iCrdSysIndex, Bit32 iAxisIndex, float32* pdCmdSpeed, Bit32 iDataLen)
{
    int i;
    int iMotorNO;
    uBit32 ulErrorCode;
    AXIS_STATE_DATA AxisStateData;

    for (i = 0; i < AXIE_MAP_MOTOR_MAX_COUNT; i++)
    {
        if (i >= iDataLen)
        {
            break;
        }

        //
        iMotorNO = CSM_GetAxisMapMotorNO(iCrdSysIndex, iAxisIndex, i+1);
        if (iMotorNO >= 0 && iMotorNO < AXIS_MAX_COUNT)
        {
            ulErrorCode = m_CSMInterface.pf_PAX_GetStateData(iMotorNO, &AxisStateData);
            if (ulErrorCode != 0)
            {
                return ulErrorCode;
            }
            else
            {
                pdCmdSpeed[i] = AxisStateData.dCmdSpeed;
            }
        }
        else
        {
            break;
        }
    }

    return 0;
}

/*
函 数 名：uBit32 LAX_GetQeiSpeed(Bit32 iCrdSysIndex, Bit32 iAxisIndex, float32* pdQeiSpeed, Bit32 iDataLen)
功    能：获取轴编码器速度
参    数：iCrdSysIndex --坐标系索引
          iAxisIndex --轴索引
          pdQeiSpeed --轴编码器速度存储缓冲区地址
          iDataLen --轴编码器速度存储缓冲区长度（表示可以存放多少轴的编码器速度）
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 LAX_GetQeiSpeed(Bit32 iCrdSysIndex, Bit32 iAxisIndex, float32* pdQeiSpeed, Bit32 iDataLen)
{
    int i;
    int iMotorNO;
    uBit32 ulErrorCode;
    AXIS_STATE_DATA AxisStateData;

    for (i = 0; i < AXIE_MAP_MOTOR_MAX_COUNT; i++)
    {
        if (i >= iDataLen)
        {
            break;
        }

        //
        iMotorNO = CSM_GetAxisMapMotorNO(iCrdSysIndex, iAxisIndex, i+1);
        if (iMotorNO >= 0 && iMotorNO < AXIS_MAX_COUNT)
        {
            ulErrorCode = m_CSMInterface.pf_PAX_GetStateData(iMotorNO, &AxisStateData);
            if (ulErrorCode != 0)
            {
                return ulErrorCode;
            }
            else
            {
                pdQeiSpeed[i] = AxisStateData.dQeiSpeed;
            }
        }
        else
        {
            break;
        }
    }

    return 0;
}

/*
函 数 名：uBit32 LAX_GetAccSpeed(Bit32 iCrdSysIndex, Bit32 iAxisIndex, float32* pdAccSpeed, Bit32 iDataLen)
功    能：获取轴加速度
参    数：iCrdSysIndex --坐标系索引
          iAxisIndex --轴索引
          pdAccSpeed --轴加速度存储缓冲区地址
          iDataLen --轴加速度存储缓冲区长度（表示可以存放多少轴的加速度）
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 LAX_GetAccSpeed(Bit32 iCrdSysIndex, Bit32 iAxisIndex, float32* pdAccSpeed, Bit32 iDataLen)
{
    int i;
    int iMotorNO;
    uBit32 ulErrorCode;
    AXIS_STATE_DATA AxisStateData;

    for (i = 0; i < AXIE_MAP_MOTOR_MAX_COUNT; i++)
    {
        if (i >= iDataLen)
        {
            break;
        }

        //
        iMotorNO = CSM_GetAxisMapMotorNO(iCrdSysIndex, iAxisIndex, i+1);
        if (iMotorNO >= 0 && iMotorNO < AXIS_MAX_COUNT)
        {
            ulErrorCode = m_CSMInterface.pf_PAX_GetStateData(iMotorNO, &AxisStateData);
            if (ulErrorCode != 0)
            {
                return ulErrorCode;
            }
            else
            {
                pdAccSpeed[i] = AxisStateData.dAcc;
            }
        }
        else
        {
            break;
        }
    }

    return 0;
}

/*
函 数 名：uBit32 LAX_GetSvStatus(Bit32 iCrdSysIndex, Bit32 iAxisIndex, uBit32* pulSvStatus, Bit32 iDataLen)
功    能：获取伺服系统状态
参    数：iCrdSysIndex --坐标系索引
          iAxisIndex --轴索引
          pulSvStatus --伺服状态存储缓冲区地址
          iDataLen --伺服状态存储缓冲区长度（表示可以存放多少轴的伺服状态）
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 LAX_GetSvStatus(Bit32 iCrdSysIndex, Bit32 iAxisIndex, uBit32* pulSvStatus, Bit32 iDataLen)
{
    int i;
    int iMotorNO;
    uBit32 ulErrorCode;
    AXIS_STATE_DATA AxisStateData;

    for (i = 0; i < AXIE_MAP_MOTOR_MAX_COUNT; i++)
    {
        if (i >= iDataLen)
        {
            break;
        }

        //
        iMotorNO = CSM_GetAxisMapMotorNO(iCrdSysIndex, iAxisIndex, i+1);
        if (iMotorNO >= 0 && iMotorNO < AXIS_MAX_COUNT)
        {
            ulErrorCode = m_CSMInterface.pf_PAX_GetStateData(iMotorNO, &AxisStateData);
            if (ulErrorCode != 0)
            {
                return ulErrorCode;
            }
            else
            {
                pulSvStatus[i] = AxisStateData.ulSvStatus;
            }
        }
        else
        {
            break;
        }
    }

    return 0;
}

/*
函 数 名：AXIS_GetSvAlarm
功    能：获取伺服报警状态
参    数：iCrdSysIndex --坐标系索引
          iAxisIndex --轴索引
          pulRunningStatus --伺服报警存储缓冲区地址
          iDataLen --伺服报警存储缓冲区长度（表示可以存放多少轴的伺服报警）
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 LAX_GetSvAlarm(Bit32 iCrdSysIndex, Bit32 iAxisIndex, uBit32* pulSvAlarm, Bit32 iDataLen)
{
    int i;
    int iMotorNO;
    uBit32 ulErrorCode;
    AXIS_STATE_DATA AxisStateData;

    for (i = 0; i < AXIE_MAP_MOTOR_MAX_COUNT; i++)
    {
        if (i >= iDataLen)
        {
            break;
        }

        //
        iMotorNO = CSM_GetAxisMapMotorNO(iCrdSysIndex, iAxisIndex, i+1);
        if (iMotorNO >= 0 && iMotorNO < AXIS_MAX_COUNT)
        {
            ulErrorCode = m_CSMInterface.pf_PAX_GetStateData(iMotorNO, &AxisStateData);
            if (ulErrorCode != 0)
            {
                return ulErrorCode;
            }
            else
            {
                pulSvAlarm[i] = AxisStateData.ulSvAlarm;
            }
        }
        else
        {
            break;
        }
    }

    return 0;
}

/*
函 数 名：Bit32 CSM_CheckMotorStop(Bit32 iMotorNO)
功    能：检测电机是否运动停止
参    数：iMotorNO --坐标轴索引，取值从0开始
返 回 值：运动停止返回1，否则返回0
调用位置：
注意事项：无
*/
Bit32 CSM_CheckMotorStop(Bit32 iMotorNO)
{
    AXIS_STATE_DATA AxisStateData;

    m_CSMInterface.pf_PAX_GetStateData(iMotorNO, &AxisStateData);

    if (!equal(AxisStateData.dCmdSpeed, 0.0) 
        //|| !equal(AxisStateData.dQeiSpeed, 0.0)
        || !(AxisStateData.ulRunningStatus & AXIS_STATUS_ZERO_SPEED))
    {
        return 0;
    }
    return 1; 
}

/*
函 数 名：int CSM_CheckAxisStop(int iCrdSysIndex, int iAxisIndex);
功    能：检测轴是否运动停止
参    数：iCrdSysIndex --通道索引
          iAxisIndex --坐标轴轴索引
返 回 值：运动停止返回1，否则返回0
调用位置：
注意事项：无
*/
Bit32 LAX_CheckAxisStop(Bit32 iCrdSysIndex, Bit32 iAxisIndex)
{
    int i;
    unsigned short nMotorNO;

    //
    for (i = 0; i < AXIS_MAP_MOTOR_MAX_COUNT; i++)
    {
        nMotorNO = CSM_GetAxisMapMotorNO(iCrdSysIndex, iAxisIndex, i+1);
        if ( nMotorNO < AXIS_MAX_COUNT)
        {
            if(!CSM_CheckMotorStop(nMotorNO))
            {
                return 0;
            }          
        }
    }

    return 1;
}