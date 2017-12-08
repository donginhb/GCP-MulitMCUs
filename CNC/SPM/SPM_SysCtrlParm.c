/*
Copyright (c) 2013, 东莞华科精机有限公司 All rights reserved.

文件名称：SPM_SysCtrlParm.c 

摘    要：参数管理模块

运行环境： LPC43xx 

修改描述：

当前版本：1.0

修改作者：毛军

修改日期：2015年10月14日
*/

#include <string.h>
#include "SPM_ParmCheck.h"
#include "SPM_SysCtrlParm.h"
#include "CNC/AHB/AHB_Init.h"
#include "CNC/DataStructDef/AXIS_DataStructDef.h"
#include "CNC/DataStructDef/SYS_DataStructDef.h"
#include "CNC/DataStructDef/CRD_DataStructDef.h"
#include "CNC/DataStructDef/ERR_DataStructDef.h"
#include "CNC/DataStructDef/SYS_ConfigDef.h"
#include "CNC/ErrorCodeDef/SPM_ErrorCodeDef.h"
#include "CNC/ErrorCodeMan/ECM_ErrorCodeMan.h"
#include "CNC/DVM/DEV_Interface.h"
#include "CNC/CSM/CSM_Interface.h"
#include "CNC/CNCSYS/CNCSYS_Interface.h"

#include "../IPO/IPO_Interface.h"


//常用参数再本地建立数据区
static SYS_CTRL_PARM*                m_SPM_pSysCtrlParm = NULL;
static CRDSYS_AXIS_MAP_TABLE*        m_SPM_pCrdAxisMapTable = NULL;
static CRDSYS_PARM*                    m_SPM_pCrdParm = NULL;
static AXIS_PARM*                    m_SPM_pAxisParm = NULL;
static AXIS_PITCH_CMP_PARM*            m_SPM_pAxisPitchCmpParm = NULL;

uBit32 SPM_LoadParm();



//-------------------------------------------------------------------------------------------

/*
函 数 名：const SYS_CTRL_PARM* SPM_GetSysCtrlParmAddr()
功    能：获取系统控制参数访问地址（只读）
参    数：无
返 回 值：系统控制参数访问地址
调用位置：
注意事项：无
*/
const SYS_CTRL_PARM* SPM_GetSysCtrlParmAddr()
{
    return m_SPM_pSysCtrlParm;
}

/*
函 数 名：const CRDSYS_AXIS_MAP_TABLE* SPM_GetCrdSysAxisMapTableAddr()
功    能：获取坐标系映射表访问地址（只读）
参    数：无
返 回 值：坐标系映射表访问地址
调用位置：
注意事项：无
*/
const CRDSYS_AXIS_MAP_TABLE* SPM_GetCrdSysAxisMapTableAddr()
{
    return m_SPM_pCrdAxisMapTable;
}

/*
函 数 名：const CRDSYS_PARM* SPM_GetCrdSysParmAddr()
功    能：获取坐标系参数访问地址（只读）
参    数：无
返 回 值：坐标系参数访问地址
调用位置：
注意事项：无
*/
const CRDSYS_PARM* SPM_GetCrdSysParmAddr()
{
    return m_SPM_pCrdParm;
}

/*
函 数 名：const AXIS_PARM* SPM_GetAxisParmAddr()
功    能：获取轴参数访问地址（只读）
参    数：无
返 回 值：轴参数访问地址
调用位置：
注意事项：无
*/
const AXIS_PARM* SPM_GetAxisParmAddr()
{
    return m_SPM_pAxisParm;
}

/*
函 数 名：const AXIS_PITCH_CMP_PARM* SPM_GetAxisPitchCmpParmAddr()
功    能：获取轴螺距补偿参数访问地址（只读）
参    数：无
返 回 值：轴螺距补偿参数访问地址
调用位置：
注意事项：无
*/
const AXIS_PITCH_CMP_PARM* SPM_GetAxisPitchCmpParmAddr()
{
    return m_SPM_pAxisPitchCmpParm;
}


/*
函 数 名：void SPM_InitSysCtrlData()
功    能：初始化系统控制参数
参    数：无
返 回 值：无
调用位置：系统上电时调用
注意事项：无
*/
void SPM_InitSysCtrlParm()
{
    int i, j, k;   
    
    m_SPM_pSysCtrlParm = &(g_pAHBDataStruct->SysCtrlParm);
    m_SPM_pCrdAxisMapTable = g_pAHBDataStruct->CrdAxisMapTable;
    m_SPM_pCrdParm = g_pAHBDataStruct->CrdParm;
    m_SPM_pAxisParm = g_pAHBDataStruct->AxisParm;
    
#if ENABLE_AXIS_PITCH_CMP_PARM > 0    
    m_SPM_pAxisPitchCmpParm = g_pAHBDataStruct->AxisPitchCmpParm;
#endif
    
    //坐标系映射表
    memset(m_SPM_pCrdAxisMapTable, 0, sizeof(CRDSYS_AXIS_MAP_TABLE) * CRDSYS_MAX_COUNT);
    
    for (i = 0; i < CRDSYS_MAX_COUNT; i++)
    {
        for (j = 0; j < CRDSYS_AXIS_MAX_COUNT; j++)
        {
            for (k = 0; k < AXIS_MAP_MOTOR_MAX_COUNT; k++)
            {
                m_SPM_pCrdAxisMapTable[i].CrdSysAxisMap[j].cMotorNO[k] = 0xFF;
            }
        }
    }
    
#if 0   //2017.12.05 duhanfeng 暂时屏蔽 
    //从FLASH加载系统控制参数
    if (!SPM_LoadParm())
    { 
        //加载成功
        return;
    }
    else
    { //加载失败
        CNCSYS_SetAlarmBit(SYS_ALARM_LOAD_SYS_PARM_FAIL);
    }
#endif
    
    //系统控制参数初始化
    memset(m_SPM_pSysCtrlParm, 0, sizeof(SYS_CTRL_PARM));
    m_SPM_pSysCtrlParm->nCrdSysCount = CRDSYS_MAX_COUNT;    
    m_SPM_pSysCtrlParm->nSysSet = SYS_SET_CRDSYSINTERRELATION;
    m_SPM_pSysCtrlParm->nTimeCycle = SYS_IPO_TIME_CYCLE_DEFAULT;        //插补周期1ms
    
    //轴参数初始化
    memset(m_SPM_pAxisParm, 0, sizeof(AXIS_PARM) * AXIS_MAX_COUNT);
    
    for (i = 0; i < AXIS_MAX_COUNT; i++)
    {
        m_SPM_pAxisParm[i].AxisScaleParm.nType = 1;
        m_SPM_pAxisParm[i].AxisScaleParm.lGearRatioPosScale = 1;        
        m_SPM_pAxisParm[i].AxisScaleParm.lGearRatioPlusScale = 1;    
        m_SPM_pAxisParm[i].AxisScaleParm.lInternalPulseScale = 1;         
        m_SPM_pAxisParm[i].AxisScaleParm.lQEIPulseScale = 1;            
        m_SPM_pAxisParm[i].AxisScaleParm.lCycleQeiPulseCount = 10000;            
        
#if 0
        m_SPM_pAxisParm[i].AxisMoveParm.dMaxJogSpeed = 200;    
        m_SPM_pAxisParm[i].AxisMoveParm.dMaxJogAcc = 5.0;        
        m_SPM_pAxisParm[i].AxisMoveParm.dJogJerk = 0.1;        
#else 
        m_SPM_pAxisParm[i].AxisMoveParm.dMaxJogSpeed = 200;    
        m_SPM_pAxisParm[i].AxisMoveParm.dMaxJogAcc = 0.1;        
        m_SPM_pAxisParm[i].AxisMoveParm.dJogJerk = 0.1;   
#endif
        m_SPM_pAxisParm[i].AxisMoveParm.dMaxFeedSpeed = 200;    
        m_SPM_pAxisParm[i].AxisMoveParm.dMaxFeedAcc = 5.0;    
        m_SPM_pAxisParm[i].AxisMoveParm.dFeedJerk = 0.1;        
        m_SPM_pAxisParm[i].AxisMoveParm.dRapidSpeed = 200;    
        m_SPM_pAxisParm[i].AxisMoveParm.dRapidAcc = 5.0;        
        m_SPM_pAxisParm[i].AxisMoveParm.dRapidJerk = 0.1;        
        
        m_SPM_pAxisParm[i].AxisHomeParm.dHomeRapidSpeed = 200;
        m_SPM_pAxisParm[i].AxisHomeParm.dHomeApproachSpeed = 10;
        m_SPM_pAxisParm[i].AxisHomeParm.dHomeLocatSpeed = 5;
        m_SPM_pAxisParm[i].AxisHomeParm.dHomeIndexSpeed = 1;
        //
        m_SPM_pAxisParm[i].AxisSignalConfig.SvAlarmSignal = 1;
        m_SPM_pAxisParm[i].AxisSignalConfig.SvArrivSignal = 1;
        m_SPM_pAxisParm[i].AxisSignalConfig.QeiIndexSignal = 1;
        m_SPM_pAxisParm[i].AxisSignalConfig.SvReadySignal = 1;
        m_SPM_pAxisParm[i].AxisSignalConfig.SvEnableSignal = 1;
        m_SPM_pAxisParm[i].AxisSignalConfig.HardLimNSignal = 1;
        m_SPM_pAxisParm[i].AxisSignalConfig.HardLimPSignal = 1;
        m_SPM_pAxisParm[i].AxisSignalConfig.HomeSignal = 1;
        m_SPM_pAxisParm[i].AxisSignalConfig.QeiEnable = 1;
        m_SPM_pAxisParm[i].AxisSignalConfig.SvAlarmEnable = 1;
        m_SPM_pAxisParm[i].AxisSignalConfig.SvArriveEnable = 1;
        m_SPM_pAxisParm[i].AxisSignalConfig.SvReadyEnable = 1;
    }
    
    //坐标系参数初始化
    memset(m_SPM_pCrdParm, 0, sizeof(CRDSYS_PARM) * CRDSYS_MAX_COUNT);
    
    for(i = 0; i < CRDSYS_MAX_COUNT; i++)
    {
        m_SPM_pCrdParm[i].nCrdSysSet = CRDSYS_SET_ENABLE;        
        m_SPM_pCrdParm[i].nLookaheadLen = 1000;            
        
        m_SPM_pCrdParm[i].dLineAxisCornerPreci = 20;        
        m_SPM_pCrdParm[i].dRotAxisCornerPreci = 20;        
        m_SPM_pCrdParm[i].dCircleErrorLim = 20;        
        m_SPM_pCrdParm[i].ulMinConstantVelTime = 0;    
        
        m_SPM_pCrdParm[i].dTimebaseSlewRate = 0.1;        
        m_SPM_pCrdParm[i].dFeedHoldSlewRate = 0.1;        
        m_SPM_pCrdParm[i].dStopSlewRate = 0.2;            
        
        m_SPM_pCrdParm[i].dProgMaxFeedSpeed = 200;        
        m_SPM_pCrdParm[i].dProgMaxAcc = 5.0;            
        m_SPM_pCrdParm[i].dProgMaxJerk = 0.1;            
        m_SPM_pCrdParm[i].dMaxCornerAcc = 10.0;    
        
        m_SPM_pCrdParm[i].lSpindleQeiSense = 100;    
    }  
}

/*
函 数 名：uBit32 SPM_SetSysCtrlParm(SYS_CTRL_PARM* pSysCtrlParm)
功    能：设置系统控制参数
参    数：*pSysCtrlParm——系统控制参数
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：无
*/
uBit32 SPM_SetSysCtrlParm(SYS_CTRL_PARM* pSysCtrlParm)
{
    uBit16 nLastTimeCycle;
    uBit16 nLastCrdSysCount;
    uBit32 ulErrorCode;
    
    SYS_CTRL_PARM SysCtrlParmBackup;
    
    //检测通道是否空闲
    if (!CSM_CheckSysFree())
    {
        ulErrorCode = ECM_GenerateErrorCode(SYS_SOFT_MODULE_SPM, ERR_CRD_NO_INVALID, SPM_ERR_CRDSYS_NOT_FREE, 
                                            SPM_CMD_SET_SYS_CTRL_PARM, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }
    
    //检测插补器是否空闲
    if (!IPO_CheckSysFree())
    {
        ulErrorCode = ECM_GenerateErrorCode(SYS_SOFT_MODULE_SPM, ERR_CRD_NO_INVALID, SPM_ERR_IPO_NOT_FREE, 
                                            SPM_CMD_SET_SYS_CTRL_PARM, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }
    
    //系统控制参数检查
    if(SPM_CheckSysCtrlParm(pSysCtrlParm)==0)
    {
        ulErrorCode = ECM_GenerateErrorCode(SYS_SOFT_MODULE_SPM, ERR_CRD_NO_INVALID, SPM_ERR_PARM_ERROR, 
                                            SPM_CMD_SET_SYS_CTRL_PARM, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }
    
    nLastTimeCycle = m_SPM_pSysCtrlParm->nTimeCycle;
    nLastCrdSysCount = m_SPM_pSysCtrlParm->nCrdSysCount;
    
    //根据插补周期重新配置系统中断
    if(pSysCtrlParm->nTimeCycle != nLastTimeCycle || CNCSYS_GetAlarmBit(SYS_ALARM_UPDATE_SYSCTRL_PARM_FAIL))
    {
        ulErrorCode = DEV_SendSysCtrlParm(pSysCtrlParm);
        if (ulErrorCode != 0)
        {
            CNCSYS_SetAlarmBit(SYS_ALARM_UPDATE_SYSCTRL_PARM_FAIL);
            return ulErrorCode;
        }
    }
    
    //----------------------------更新参数到AHB全局数据区-------------------------------------
    //设置系统控制参数更新状态位（关插补器）
    CNCSYS_SetStatusBit(SYS_STATUS_UPDATE_SYS_CTRL_PARM);
    
    //保存系统配置参数
    SysCtrlParmBackup = *m_SPM_pSysCtrlParm;
    *m_SPM_pSysCtrlParm = *pSysCtrlParm;
    
    
    //-----------------------通知M4更新插补中断周期（重新配置TIME0）-------------------------
    if(pSysCtrlParm->nTimeCycle != nLastTimeCycle || CNCSYS_GetAlarmBit(SYS_ALARM_UPDATE_SYSCTRL_PARM_FAIL))
    {
        /////////////////////////////////////////////////////////////////
        //laomao  2016-05-13 注释掉，无论是否启动安川总线中断，都需要通知M4更新插补中断
        //if (!DEV_CheckBusInitSuccess(DEV_BUS_TYPE_MECHALINK_III))
        /////////////////////////////////////////////////////////////////
        {//未启动安川总线中断作为插补中断
            ulErrorCode = IPO_UpdateSycCtrlParm();
            
            if (ulErrorCode != 0)
            {
                CNCSYS_SetAlarmBit(SYS_ALARM_UPDATE_SYSCTRL_PARM_FAIL);
                
                *m_SPM_pSysCtrlParm = SysCtrlParmBackup;
                
                //清除系统控制参数更新状态位(开插补中断)
                CNCSYS_ClrStatusBit(SYS_STATUS_UPDATE_SYS_CTRL_PARM);
                
                return ulErrorCode;
            }
            
#if 0 //2017.11.05 Duhanfeng 暂时屏蔽
            SYS_SetSysTickTimeCylce(pSysCtrlParm->nTimeCycle);
#endif
        }
    }
    
    
    //-------------------------------------------------------------------------------------------
    
    if (nLastCrdSysCount != m_SPM_pSysCtrlParm->nCrdSysCount)
    {
        //重新初始化插补缓冲区
        IPO_InitIPOBuf();
        
        //重新初始化程序缓冲区
        CSM_InitProgBuf();
        
        //重新初始化用户自定义数据缓冲区
        CSM_InitUserBuf();
    }
    
    //清除系统控制参数更新状态位(开插补中断)
    CNCSYS_ClrStatusBit(SYS_STATUS_UPDATE_SYS_CTRL_PARM);
    
    //清除加载参数失败报警
    CNCSYS_ClrAlarmBit(SYS_ALARM_LOAD_SYS_PARM_FAIL);
    
    /////////////////////////////////
    //laomao  2016-05-13 添加
    CNCSYS_ClrAlarmBit(SYS_ALARM_UPDATE_SYSCTRL_PARM_FAIL);
    /////////////////////////////////
    
    return 0;
    
}

/*
函 数 名：uBit32 SPM_SetAxisParm(Bit32 iAxisNO, AXIS_PARM* pAxisParm)
功    能：设置电机轴控制参数
参    数：iAxisNO --电机编号
          *pAxisParm --电机轴控制参数数据
返 回 值：成功返回0，否则返回错误码
注意事项：
*/
uBit32 SPM_SetAxisParm(Bit32 iAxisNO, AXIS_PARM* pAxisParm)
{
    uBit32 ulErrorCode;
    uBit32 ulErrorCode1;
    int iCreateRapidTable = 0;
    int iCreateJogPosCtrlTable = 0;


    //检测通道是否空闲
    if (!CSM_CheckSysFree())
    {
        ulErrorCode = ECM_GenerateErrorCode(SYS_SOFT_MODULE_SPM, ERR_CRD_NO_INVALID, SPM_ERR_CRDSYS_NOT_FREE, 
            SPM_CMD_SET_AXIS_PARM, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }

    //检测插补器是否空闲
    if (!IPO_CheckSysFree())
    {
        ulErrorCode = ECM_GenerateErrorCode(SYS_SOFT_MODULE_SPM, ERR_CRD_NO_INVALID, SPM_ERR_IPO_NOT_FREE, 
            SPM_CMD_SET_AXIS_PARM, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }


    if (!memcmp(pAxisParm, m_SPM_pAxisParm + iAxisNO, sizeof(AXIS_PARM)))
    {
        return 0;
    }

    //参数合法性检查(在通信层实现)
    if (SPM_CheckAxisParm(pAxisParm)==0)
    {
        ulErrorCode = ECM_GenerateErrorCode(SYS_SOFT_MODULE_SPM, ERR_CRD_NO_INVALID, SPM_ERR_PARM_ERROR, 
            SPM_CMD_SET_AXIS_PARM, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        
        return ulErrorCode;
    }

    //发送轴参数到设备端
    ulErrorCode = PAX_SendAxisParm(iAxisNO, pAxisParm);

    if (ulErrorCode != 0)
    {
        return ulErrorCode;
    }
    
    ///////////////////////////////////////////
    //laomao  2016-05-09 添加   
    //
    if (m_SPM_pAxisParm[iAxisNO].AxisMoveParm.dRapidJerk != pAxisParm->AxisMoveParm.dRapidJerk
        || m_SPM_pAxisParm[iAxisNO].AxisMoveParm.dRapidAcc != pAxisParm->AxisMoveParm.dRapidAcc
        || m_SPM_pAxisParm[iAxisNO].AxisMoveParm.dRapidSpeed != pAxisParm->AxisMoveParm.dRapidSpeed)
    {
        iCreateRapidTable = 1;
    }
    
    if (m_SPM_pAxisParm[iAxisNO].AxisMoveParm.dJogJerk != pAxisParm->AxisMoveParm.dJogJerk
        || m_SPM_pAxisParm[iAxisNO].AxisMoveParm.dMaxJogAcc != pAxisParm->AxisMoveParm.dMaxJogAcc
        || m_SPM_pAxisParm[iAxisNO].AxisMoveParm.dMaxJogSpeed != pAxisParm->AxisMoveParm.dMaxJogSpeed)
    {
        iCreateJogPosCtrlTable = 1;
    }

    ///////////////////////////////////////////

    //存储轴参数
    m_SPM_pAxisParm[iAxisNO] = *pAxisParm;

    
    //重新创建轴快移速度规划表
    if (iCreateRapidTable)
    {
        ulErrorCode = IPO_CreateAxisRapidSpeedCtrlTable(iAxisNO);
    }
    
    ///////////////////////////////////////////
    //laomao  2016-05-09 添加
    if (iCreateJogPosCtrlTable)
    {
        ulErrorCode1 = IPO_CreateMotorJogPosCtrlTable(iAxisNO);
        
        if (ulErrorCode == 0 && ulErrorCode1 != 0)
        {
            ulErrorCode = ulErrorCode1;
        }
    }
    ///////////////////////////////////////////


    if (ulErrorCode != 0)
    { //
        return ulErrorCode;
    }    

    return 0;
   
}


/*
函 数 名：uBit32 SPM_SetAxisPitchCmpParm(int iAxisNO, AXIS_PITCH_CMP_PARM* pPitchParm)
功    能：设置电机轴螺距补偿参数
参    数：iAxisNO --电机编号
          *pPitchParm --电机轴螺距补偿参数
返 回 值：成功返回0，否则返回错误码
注意事项：
*/
uBit32 SPM_SetAxisPitchCmpParm(Bit32 iAxisNO, AXIS_PITCH_CMP_PARM* pPitchParm)
{
 #if ENABLE_AXIS_PITCH_CMP_PARM 
    uBit32 ulErrorCode;

    //检测通道是否空闲
    if (!CSM_CheckSysFree())
    {
        ulErrorCode = ECM_GenerateErrorCode(SYS_SOFT_MODULE_SPM, ERR_CRD_NO_INVALID, SPM_ERR_CRDSYS_NOT_FREE, 
            SPM_CMD_SET_AXIS_PTICH_CMP_PARM, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }

    //检测插补器是否空闲
    if (!IPO_CheckSysFree())
    {
        ulErrorCode = ECM_GenerateErrorCode(SYS_SOFT_MODULE_SPM, ERR_CRD_NO_INVALID, SPM_ERR_IPO_NOT_FREE, 
            SPM_CMD_SET_AXIS_PTICH_CMP_PARM, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }

    //存储轴参数
    m_SPM_pAxisPitchCmpParm[iAxisNO] = *pPitchParm;

    return 0;
#else
    return 1;
#endif    
}


/*
函 数 名：uBit32 SPM_SetCrdSysAxisMapTable(CRDSYS_AXIS_MAP_TABLE* pCrdAxisMapTable, Bit32 iCrdSysCount)
功    能：设置坐标系坐标轴映射表
参    数：pCrdAxisMapTable --坐标系映射表数据起始地址
          iCrdSysCount --坐标系数量
返 回 值：成功返回0, 否则返回错误码
注意事项：系统所有坐标系的坐标系映射表必须一次性设置，以便于数据合法性检查
*/
uBit32 SPM_SetCrdSysAxisMapTable(CRDSYS_AXIS_MAP_TABLE* pCrdAxisMapTable, Bit32 iCrdSysCount)
{
    uBit32 ulErrorCode;
    int i, j;


    //检测通道是否空闲
    if (!CSM_CheckSysFree())
    {
        ulErrorCode = ECM_GenerateErrorCode(SYS_SOFT_MODULE_SPM, ERR_CRD_NO_INVALID, SPM_ERR_CRDSYS_NOT_FREE, 
            SPM_CMD_SET_CRDSYS_AXIS_MAP_TABLE, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }

    //检测插补器是否空闲
    if (!IPO_CheckSysFree())
    {
        ulErrorCode = ECM_GenerateErrorCode(SYS_SOFT_MODULE_SPM, ERR_CRD_NO_INVALID, SPM_ERR_IPO_NOT_FREE, 
            SPM_CMD_SET_CRDSYS_AXIS_MAP_TABLE, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }

    //参数合法性检验
    ulErrorCode = ECM_GenerateErrorCode(SYS_SOFT_MODULE_SPM, ERR_CRD_NO_INVALID, SPM_ERR_PARM_ERROR, 
        SPM_CMD_SET_CRDSYS_AXIS_MAP_TABLE, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);

    if (iCrdSysCount != m_SPM_pSysCtrlParm->nCrdSysCount)
    {
        return ulErrorCode;
    }

    if (SPM_CheckCrdAxisMapTable(pCrdAxisMapTable, iCrdSysCount)==0)
    {
        return ulErrorCode;
    }

    //设置更新系统参数状态位（关插补中断）
    CNCSYS_SetStatusBit(SYS_STATUS_UPDATE_SYS_CTRL_PARM);

    //
    memcpy(m_SPM_pCrdAxisMapTable, pCrdAxisMapTable, sizeof(CRDSYS_AXIS_MAP_TABLE) * iCrdSysCount);

    if (iCrdSysCount < CRDSYS_MAX_COUNT)
    {
        memset(m_SPM_pCrdAxisMapTable + iCrdSysCount, 0, sizeof(CRDSYS_AXIS_MAP_TABLE) * (CRDSYS_MAX_COUNT - iCrdSysCount));
        
        for (i=iCrdSysCount;  i<CRDSYS_MAX_COUNT; i++)
        {
            for (j=0; j<CRDSYS_AXIS_MAX_COUNT; j++)
            {
                m_SPM_pCrdAxisMapTable[i].CrdSysAxisMap[j].cMotorNO[0] = DEV_INVALID_NO;
                m_SPM_pCrdAxisMapTable[i].CrdSysAxisMap[j].cMotorNO[1] = DEV_INVALID_NO;
            }
        }
    }

    //通知插补器更新坐标系映射表
    ulErrorCode = IPO_UpdateCrdSysAxisMapTable();
    if (ulErrorCode != 0)
    {
        CNCSYS_ClrStatusBit(SYS_STATUS_UPDATE_SYS_CTRL_PARM);
        return ulErrorCode;
    }

    //更新通道管理模块坐标系映射表
    ulErrorCode = CSM_UpdateCrdSysAxisMapTable();
    if (ulErrorCode != 0)
    {
        CNCSYS_ClrStatusBit(SYS_STATUS_UPDATE_SYS_CTRL_PARM);
        return ulErrorCode;
    }

    //
    CNCSYS_ClrStatusBit(SYS_STATUS_UPDATE_SYS_CTRL_PARM);

    //
    CNCSYS_ClrAlarmBit(SYS_ALARM_LOAD_SYS_CONFIG_FAIL);


    return 0;   
}

/*
函 数 名：uBit32 SPM_SetCrdParm(Bit32 iCrdSysIndex, CRDSYS_PARM* pCrdParm)
功    能：设置坐标系参数
参    数：iCrdSysIndex --坐标系索引
          *pCrdParm --坐标系参数
返 回 值：成功返回0, 否则返回错误码
注意事项：
*/
uBit32 SPM_SetCrdParm(Bit32 iCrdSysIndex,  CRDSYS_PARM* pCrdParm)
{
    uBit32 ulErrorCode;

    //检测通道是否空闲
    if (!CSM_CheckSysFree())
    {
        ulErrorCode = ECM_GenerateErrorCode(SYS_SOFT_MODULE_SPM, ERR_CRD_NO_INVALID, SPM_ERR_CRDSYS_NOT_FREE, 
            SPM_CMD_SET_SYS_CTRL_PARM, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }

    //检测插补器是否空闲
    if (!IPO_CheckSysFree())
    {
        ulErrorCode = ECM_GenerateErrorCode(SYS_SOFT_MODULE_SPM, ERR_CRD_NO_INVALID, SPM_ERR_IPO_NOT_FREE, 
            SPM_CMD_SET_SYS_CTRL_PARM, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }

    //参数合法性检验
    if (SPM_CheckCrdSysParm(pCrdParm)==0)
    {
        ulErrorCode = ECM_GenerateErrorCode(SYS_SOFT_MODULE_SPM, ERR_CRD_NO_INVALID, SPM_ERR_PARM_ERROR, 
            SPM_CMD_SET_SYS_CTRL_PARM, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);

        return ulErrorCode;
    }

    m_SPM_pCrdParm[iCrdSysIndex] = *pCrdParm;

    return 0; 
}


//----------------------------------------------------------------------------------------------------

/*
函 数 名：int SPM_GetCrdSysAxisMapTable(int iCrdSysIndex, CRDSYS_AXIS_MAP_TABLE* pCrdAxisMapTable)
功    能：获取坐标系坐标轴映射表
参    数：iCrdSysIndex——坐标系索引
          *pCrdAxisMapTable——坐标系坐标轴映射表（出参）
返 回 值：无
注意事项：坐标系有运动指令时，不能修改坐标系映射表
*/
void SPM_GetCrdSysAxisMapTable(int iCrdSysIndex, CRDSYS_AXIS_MAP_TABLE* pCrdAxisMapTable)
{
    *pCrdAxisMapTable = m_SPM_pCrdAxisMapTable[iCrdSysIndex];
}


/*
函 数 名：int SPM_GetCrdSysCtrlData(int iCrdSysIndex, CRDSYS_CTRL_DATA* pCrdSysCtrlData)
功    能：设置坐标系控制参数
参    数：iCrdSysIndex——坐标系索引
          *pCrdSysCtrlData——坐标系控制参数(出参)
返 回 值：无
注意事项：无
*/
void SPM_GetCrdParm(int iCrdSysIndex, CRDSYS_PARM* pCrdParm)
{
    *pCrdParm = m_SPM_pCrdParm[iCrdSysIndex];
}


/*
函 数 名：unsigned short SPM_GetCrdAxisMapMotorNO(int iCrdSysIndex, int iAxisIndex)
功    能：获取去指定坐标系中指定坐标轴对应的电机编号
参    数：iCrdSysIndex——坐标系索引
          iAxisIndex——坐标轴在坐标轴映射表中的索引
返 回 值：指定坐标轴对应的电机编号
调用位置：插补器调用
注意事项：无
*/
unsigned short SPM_GetCrdAxisMapMotorNO(int iCrdSysIndex, int iAxisIndex)
{
    unsigned short* pMotorNO = (unsigned short*)(m_SPM_pCrdAxisMapTable[iCrdSysIndex].CrdSysAxisMap[iAxisIndex].cMotorNO);

    return *pMotorNO;
}


/*
函 数 名：unsigned short SPM_GetMotorInMapCrdNo(nt iAxisIndex)
功    能：获取电机编号对应的坐标系编号
参    数：iAxisIndex——坐标轴在坐标轴映射表中的索引
返 回 值：指定电机编号的坐标系号
调用位置：插补器调用
注意事项：无
*/
int SPM_GetMotorInMapCrdNo(int iAxisIndex, int *iCrdSysIndex)
{
    int i,j,k;
    int nMotorNum;
    int nMotorNo;
    CRDSYS_AXIS_MAP_TABLE CrdAxisMapTable;

    for(i=0;i<CRDSYS_MAX_COUNT;i++)
    {
        SPM_GetCrdSysAxisMapTable(i, &CrdAxisMapTable);
        nMotorNum = CrdAxisMapTable.nCrdAxisCount;
        for(j=0;j<nMotorNum;j++)
        {
           for(k=0;k<AXIE_MAP_MOTOR_MAX_COUNT;k++)
           {
               nMotorNo = CrdAxisMapTable.CrdSysAxisMap[j].cMotorNO[k];
               if(nMotorNo == iAxisIndex)
               {
                 *iCrdSysIndex = i;
                 return 1;
               }
           }
        }
    }
    return 0;
}

/*
函 数 名：int SPM_GetCrdSysAxisMoveParm(int iCrdSysIndex, int iAxisIndex, AXIS_MOVE_PARM* pAxisMoveParm)
功    能：获取坐标系中指定轴的运动控制数据
参    数：iCrdSysIndex——坐标系索引
          iAxisIndex——坐标轴在坐标轴映射表中的索引
          *pAxisCtrlData——轴控制数据（出参）
返 回 值：成功返回0，否则返回非0
调用位置：插补器使用
注意事项：
*/
int SPM_GetCrdSysAxisMoveParm(int iCrdSysIndex, int iAxisIndex, AXIS_MOVE_PARM* pAxisMoveParm)
{
    unsigned short nMotorNO;

    nMotorNO = m_SPM_pCrdAxisMapTable[iCrdSysIndex].CrdSysAxisMap[iAxisIndex].cMotorNO[0];

    *pAxisMoveParm = m_SPM_pAxisParm[nMotorNO].AxisMoveParm;
 
    return 0;
}

//
void SPM_GetAxisParm(int iAxisIndex, AXIS_PARM* pAxisParm)
{
    *pAxisParm = m_SPM_pAxisParm[iAxisIndex];
}

//
uBit32 SPM_SetLookaheadEnable(Bit32 iCrdSysIndex, Bit32 iEnable)
{
    if (iEnable)
    {
        m_SPM_pCrdParm[iCrdSysIndex].nCrdSysSet |= CRDSYS_SET_LOOKAHEAD_ENABLE;
    }
    else
    {
        m_SPM_pCrdParm[iCrdSysIndex].nCrdSysSet &= ~CRDSYS_SET_LOOKAHEAD_ENABLE;
    }
    
    return 0;
}


/*
函 数 名：int SPM_CheckLookaheadEnable(int iCrdSysNO)
功    能：检测前瞻是否使能
参    数：iCrdSysIndex——坐标系索引
返 回 值：使能返回1，否则返回0
调用位置：前瞻模块
注意事项：无
*/
int SPM_CheckLookaheadEnable(int iCrdSysIndex)
{
    if (m_SPM_pCrdParm[iCrdSysIndex].nCrdSysSet & CRDSYS_SET_LOOKAHEAD_ENABLE)
    {
        return 1;
    }
    else
    {
        return 0;
    }
    
}


/*
函 数 名：int SPM_GetLookaheadLen(nt iCrdSysNO)
功    能：获取前瞻长度
参    数：iCrdSysIndex——坐标系索引
返 回 值：前瞻长度
调用位置：前瞻模块
注意事项：无
*/
int SPM_GetLookaheadLen(int iCrdSysIndex)
{
    return m_SPM_pCrdParm[iCrdSysIndex].nLookaheadLen;
}



/*
函 数 名：void SPM_GetSysCtrlParm(SYS_CTRL_PARM* pSysCtrlParm)
功    能：获取系统控制参数
参    数：*pSysCtrlParm——系统控制参数
返 回 值：无
注意事项：
*/
void SPM_GetSysCtrlParm(SYS_CTRL_PARM* pSysCtrlParm)
{
    *pSysCtrlParm = *m_SPM_pSysCtrlParm;
}

/*
函 数 名：int SPM_GetCrdSysCount()
功    能：获取系统坐标系数量
参    数：无
返 回 值：系统坐标系数量
调用位置：
注意事项：
*/
int SPM_GetCrdSysCount()
{
    return m_SPM_pSysCtrlParm->nCrdSysCount;
}

/*
函 数 名：float32 SPM_GetSysTimeCycle()
功    能：获取系统插补周期
参    数：无
返 回 值：系统插补周期
调用位置：
注意事项：
*/
float32 SPM_GetSysTimeCycle()
{
    return m_SPM_pSysCtrlParm->nTimeCycle / 1000.0;
}

/*
函 数 名：float32 SPM_GetSysTimeCycleUs()
功    能：获取系统插补周期
参    数：无
返 回 值：系统插补周期
调用位置：
注意事项：
*/
uBit16 SPM_GetSysTimeCycleUs()
{
    return m_SPM_pSysCtrlParm->nTimeCycle;
}

/*
函 数 名：int SPM_CheckCrdSysInterrelation()
功    能：检测坐标系是否关联
参    数：无
返 回 值：坐标系关联返回1，否则返回0
调用位置：
注意事项：
*/
int SPM_CheckCrdSysInterrelation()
{
    if (m_SPM_pSysCtrlParm->nSysSet & SYS_SET_CRDSYSINTERRELATION)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


//------------------------------------------------------------------------------------------------
/*
函 数 名：uBit32 SYS_SaveSetting(void)
功    能：保存控制参数到FLASH
参    数：无
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：

uBit32 SPM_SaveParm(void)
{
#ifdef SPM_M0
    unsigned long ulErrorCode;
    int iParmItemCount;
    char* pDataAddr[4];
    uBit32 DataLen[4];
    uBit32 ulTotalLen;
    unsigned char ucCheckSum;
    char* pDataBuf;
    char cFlashRWBuf[512];
    int iFlashRWBufLen;
    int iFlashRWBufWritePos;
    int iFlashRWBufFreeLen;
    int iParmReadPos;
    int i, j;

    uBit32 ulFlashRWAddr;
    uBit32 ulMismatchOffset;


    //检测通道是否空闲
    if (!CSM_CheckSysFree())
    {
        ulErrorCode = ECM_GenerateErrorCode(SYS_SOFT_MODULE_SPM, ERR_CRD_NO_INVALID, SPM_ERR_CRDSYS_NOT_FREE, 
            SPM_CMD_SAVE_SYS_PARM, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }

    //检测插补器是否空闲
    if (!IPO_CheckSysFree())
    {
        ulErrorCode = ECM_GenerateErrorCode(SYS_SOFT_MODULE_SPM, ERR_CRD_NO_INVALID, SPM_ERR_IPO_NOT_FREE, 
            SPM_CMD_SAVE_SYS_PARM, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }

    //
    iParmItemCount = 4;

    pDataAddr[0] = (char*)m_SPM_pSysCtrlParm;
    DataLen[0] = sizeof(SYS_CTRL_PARM);

    pDataAddr[1] = (char*)m_SPM_pCrdParm;
    DataLen[1] = sizeof(CRDSYS_PARM) * CRDSYS_MAX_COUNT;

    pDataAddr[2] = (char*)m_SPM_pAxisParm;
    DataLen[2] = sizeof(AXIS_PARM) * AXIS_MAX_COUNT;

    pDataAddr[3] = (char*)m_SPM_pAxisPitchCmpParm;
    DataLen[3] = sizeof(AXIS_PITCH_CMP_PARM) * AXIS_MAX_COUNT;

    //检查存储区大小是否满足存储需要
    ulTotalLen = 0;
    for (i = 0; i < iParmItemCount; i++)
    {
        ulTotalLen += DataLen[i];
    }

    if (ulTotalLen + 5 > FLASH_M0_SYS_PARM_END_ADDR - FLASH_M0_SYS_PARM_START_ADDR + 1)   //数据存储区开始存放4字节标示字符和1字节校验和
    {
        ulErrorCode = ECM_GenerateErrorCode(SYS_SOFT_MODULE_SPM, ERR_CRD_NO_INVALID, SPM_ERR_LESS_FLASH_FOR_SAVE, 
            SPM_CMD_SAVE_SYS_PARM, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }

    //计算数据校验和
    ucCheckSum = 0;

    for (i = 0; i < iParmItemCount; i++)
    {
        pDataBuf = pDataAddr[i];

        for (j = 0; j < DataLen[i]; j++)
        {
            ucCheckSum += pDataBuf[j];
        }
    }

    //-----------------------------------------写数据到FLASH-----------------------------------------
    __disable_irq();

    ulErrorCode = ECM_GenerateErrorCode(SYS_SOFT_MODULE_SPM, ERR_CRD_NO_INVALID, SPM_ERR_FLASH_WRITE_FAIL, 
        SPM_CMD_SAVE_SYS_PARM, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);

    //
    FlashInit(FLASH_M0_BASE_ADDR, M0_FLASH_CLK_FREQ); 

    //Alan???擦除页比较费时，超过4s，是否考虑擦除整个扇区
    if (FlashErase_Pages(FLASH_M0_SYS_PARM_START_ADDR, ulTotalLen + 5))
    { //擦除失败
        __enable_irq();
        return ulErrorCode;
    }

    //
    ulFlashRWAddr = FLASH_M0_SYS_PARM_START_ADDR;

    //
    memset(cFlashRWBuf, 0, sizeof(cFlashRWBuf));
    iFlashRWBufLen = 512;

    //
    memcpy(cFlashRWBuf, "PARM", 4);

    //
    cFlashRWBuf[4] = ucCheckSum;

    iFlashRWBufWritePos = 5;
    iFlashRWBufFreeLen = iFlashRWBufLen - 5;

    //
    for (i = 0; i < iParmItemCount; i++)
    {
        pDataBuf = pDataAddr[i];
        iParmReadPos = 0;

        while(iParmReadPos < DataLen[i])
        {
            if (iFlashRWBufFreeLen < DataLen[i] - iParmReadPos)
            {
                memcpy(cFlashRWBuf + iFlashRWBufWritePos, pDataBuf + iParmReadPos, iFlashRWBufFreeLen);
                iParmReadPos += iFlashRWBufFreeLen;

                //
                if (FlashWrite(ulFlashRWAddr, cFlashRWBuf, iFlashRWBufLen))
                {
                    __enable_irq();
                    return ulErrorCode;
                }

                //
                if (FlashRAMCompare(ulFlashRWAddr, (uBit32)cFlashRWBuf, iFlashRWBufLen, &ulMismatchOffset))
                { //数据对比不成功，说明未成功写入FLASH
                    __enable_irq();
                    return ulErrorCode;
                }

                //
                ulFlashRWAddr += iFlashRWBufLen;

                iFlashRWBufWritePos = 0;
                iFlashRWBufFreeLen = iFlashRWBufLen;
                memset(cFlashRWBuf, 0, sizeof(cFlashRWBuf));
            }
            else
            {
                memcpy(cFlashRWBuf + iFlashRWBufWritePos, pDataBuf + iParmReadPos, DataLen[i] - iParmReadPos);
                
                iFlashRWBufWritePos += DataLen[i] - iParmReadPos;
                iFlashRWBufFreeLen -= DataLen[i] - iParmReadPos;

                if (iFlashRWBufFreeLen == 0 || i == iParmItemCount - 1)
                {
                    //
                    if (FlashWrite(ulFlashRWAddr, cFlashRWBuf, iFlashRWBufLen))
                    {
                        __enable_irq();
                        return ulErrorCode;
                    }

                    //
                    if (FlashRAMCompare(ulFlashRWAddr, (uBit32)cFlashRWBuf, iFlashRWBufLen, &ulMismatchOffset))
                    { //数据对比不成功，说明未成功写入FLASH
                        __enable_irq();
                        return ulErrorCode;
                    }

                    //
                    ulFlashRWAddr += iFlashRWBufLen;

                    iFlashRWBufWritePos = 0;
                    iFlashRWBufFreeLen = iFlashRWBufLen;
                    memset(cFlashRWBuf, 0, sizeof(cFlashRWBuf));
                }

                break; //参数项处理完成，进行下一数据项处理
            }
        }
    }

    __enable_irq();

#endif

    return 0;
}
*/
/*
函 数 名：uBit32 SPM_WriteConfToFlash(void)
功    能：写控制参数到FLASH
参    数：无
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 SPM_WriteParmToFlash(void)
{
#if 0
    
    unsigned long ulErrorCode;
    int iParmItemCount;
    char* pDataAddr[4];
    uBit32 DataLen[4];
    uBit32 ulTotalLen;
    unsigned char ucCheckSum;
    char* pDataBuf;
    char cFlashRWBuf[512];
    int iFlashRWBufLen;
    int iFlashRWBufWritePos;
    int iFlashRWBufFreeLen;
    int iParmReadPos;
    int i, j;

    uBit32 ulFlashRWAddr;
    uBit32 ulMismatchOffset;

    
    //iParmItemCount = 4;
    iParmItemCount = 3;//Alan modified at 2016-8-26

    pDataAddr[0] = (char*)m_SPM_pSysCtrlParm;
    DataLen[0] = sizeof(SYS_CTRL_PARM);

    pDataAddr[1] = (char*)m_SPM_pCrdParm;
    DataLen[1] = sizeof(CRDSYS_PARM) * CRDSYS_MAX_COUNT;

    pDataAddr[2] = (char*)m_SPM_pAxisParm;
    DataLen[2] = sizeof(AXIS_PARM) * AXIS_MAX_COUNT;

    //Alan Deleted at 2016-8-26
    //pDataAddr[3] = (char*)m_SPM_pAxisPitchCmpParm;
    //DataLen[3] = sizeof(AXIS_PITCH_CMP_PARM) * AXIS_MAX_COUNT;

    //检查存储区大小是否满足存储需要
    ulTotalLen = 0;
    for (i = 0; i < iParmItemCount; i++)
    {
        ulTotalLen += DataLen[i];
    }

    if (ulTotalLen + 5 > FLASH_M0_SYS_PARM_END_ADDR - FLASH_M0_SYS_PARM_START_ADDR + 1)   //数据存储区开始存放4字节标示字符和1字节校验和
    {
        ulErrorCode = ECM_GenerateErrorCode(SYS_SOFT_MODULE_SPM, ERR_CRD_NO_INVALID, SPM_ERR_LESS_FLASH_FOR_SAVE, 
            SPM_CMD_SAVE_TO_CONTROLLER, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }

    //计算数据校验和
    ucCheckSum = 0;

    for (i = 0; i < iParmItemCount; i++)
    {
        pDataBuf = pDataAddr[i];

        for (j = 0; j < DataLen[i]; j++)
        {
            ucCheckSum += pDataBuf[j];
        }
    }

    //-----------------------------------------写数据到FLASH-----------------------------------------
    ulErrorCode = ECM_GenerateErrorCode(SYS_SOFT_MODULE_SPM, ERR_CRD_NO_INVALID, SPM_ERR_FLASH_WRITE_FAIL, 
        SPM_CMD_SAVE_TO_CONTROLLER, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);

    //
    ulFlashRWAddr = FLASH_M0_SYS_PARM_START_ADDR;

    //
    memset(cFlashRWBuf, 0, sizeof(cFlashRWBuf));
    iFlashRWBufLen = 512;

    //
    memcpy(cFlashRWBuf, "PARM", 4);

    //
    cFlashRWBuf[4] = ucCheckSum;

    iFlashRWBufWritePos = 5;
    iFlashRWBufFreeLen = iFlashRWBufLen - 5;

    //
    for (i = 0; i < iParmItemCount; i++)
    {
        pDataBuf = pDataAddr[i];
        iParmReadPos = 0;

        while(iParmReadPos < DataLen[i])
        {
            if (iFlashRWBufFreeLen < DataLen[i] - iParmReadPos)
            {
                memcpy(cFlashRWBuf + iFlashRWBufWritePos, pDataBuf + iParmReadPos, iFlashRWBufFreeLen);
                iParmReadPos += iFlashRWBufFreeLen;

                //
                if (FlashWrite(ulFlashRWAddr, cFlashRWBuf, iFlashRWBufLen))
                {
                    return ulErrorCode;
                }

                //
                if (FlashRAMCompare(ulFlashRWAddr, (uBit32)cFlashRWBuf, iFlashRWBufLen, &ulMismatchOffset))
                { //数据对比不成功，说明未成功写入FLASH
                    return ulErrorCode;
                }

                //
                ulFlashRWAddr += iFlashRWBufLen;

                iFlashRWBufWritePos = 0;
                iFlashRWBufFreeLen = iFlashRWBufLen;
                memset(cFlashRWBuf, 0, sizeof(cFlashRWBuf));
            }
            else
            {
                memcpy(cFlashRWBuf + iFlashRWBufWritePos, pDataBuf + iParmReadPos, DataLen[i] - iParmReadPos);

                iFlashRWBufWritePos += DataLen[i] - iParmReadPos;
                iFlashRWBufFreeLen -= DataLen[i] - iParmReadPos;

                if (iFlashRWBufFreeLen == 0 || i == iParmItemCount - 1)
                {
                    //
                    if (FlashWrite(ulFlashRWAddr, cFlashRWBuf, iFlashRWBufLen))
                    {
                        return ulErrorCode;
                    }

                    //
                    if (FlashRAMCompare(ulFlashRWAddr, (uBit32)cFlashRWBuf, iFlashRWBufLen, &ulMismatchOffset))
                    { //数据对比不成功，说明未成功写入FLASH
                        return ulErrorCode;
                    }

                    //
                    ulFlashRWAddr += iFlashRWBufLen;

                    iFlashRWBufWritePos = 0;
                    iFlashRWBufFreeLen = iFlashRWBufLen;
                    memset(cFlashRWBuf, 0, sizeof(cFlashRWBuf));
                }

                break; //参数项处理完成，进行下一数据项处理
            }
        }
    }

#endif
    
    return 0;
}

//从FLASH读取系统参数，成功返回0，否则返回非0值
uBit32 SPM_ReadParmFromFlash()
{
#if 0
    int iParmItemCount;
    char* pDataAddr[4];
    uBit32 DataLen[4];
    uBit32 ulTotalLen;
    unsigned char ucCheckSum;
    unsigned char ucSaveCheckSum;
    char* pDataBuf;
    char cFlashRWBuf[512];
    int iFlashRWBufLen;
    int iFlashRWBufReadPos;
    int iParmWritePos;
    uBit32 ulCurReadCount; 
    int i, j;

    uBit32 ulFlashRWAddr;

    //iParmItemCount = 4;
    iParmItemCount = 3;//Alan Modified at 2016-8-26

    pDataAddr[0] = (char*)m_SPM_pSysCtrlParm;
    DataLen[0] = sizeof(SYS_CTRL_PARM);

    pDataAddr[1] = (char*)m_SPM_pCrdParm;
    DataLen[1] = sizeof(CRDSYS_PARM) * CRDSYS_MAX_COUNT;

    pDataAddr[2] = (char*)m_SPM_pAxisParm;
    DataLen[2] = sizeof(AXIS_PARM) * AXIS_MAX_COUNT;

    //Alan Deleted at 2016-8-26
    //pDataAddr[3] = (char*)m_SPM_pAxisPitchCmpParm;
    //DataLen[3] = sizeof(AXIS_PITCH_CMP_PARM) * AXIS_MAX_COUNT;

    //
    ulTotalLen = 0;
    for (i = 0; i < iParmItemCount; i++)
    {
        ulTotalLen += DataLen[i];
    }

    if (ulTotalLen + 5 > FLASH_M0_SYS_PARM_END_ADDR - FLASH_M0_SYS_PARM_START_ADDR + 1)   //数据存储区开始存放4字节标示字符和1字节校验和
    {
        return 1;
    }

    //-----------------------------------------------从FLASH读取数据-------------------------------------------
    //__disable_irq();

    //
    memset(cFlashRWBuf, 0, sizeof(cFlashRWBuf));
    iFlashRWBufLen = 512;

    ulFlashRWAddr = FLASH_M0_SYS_PARM_START_ADDR;

    ulCurReadCount =  FlashRead(ulFlashRWAddr, cFlashRWBuf, iFlashRWBufLen);
    if (ulCurReadCount != iFlashRWBufLen)
    {
        //__enable_irq();
        return 1;
    }
    ulFlashRWAddr += iFlashRWBufLen;

    if (cFlashRWBuf[0] != 'P' || cFlashRWBuf[1] != 'A' || cFlashRWBuf[2] != 'R' ||cFlashRWBuf[3] != 'M')
    {
        //__enable_irq();
        return 1;
    }

    ucSaveCheckSum = cFlashRWBuf[4];

    iFlashRWBufReadPos = 5;

    //
    for (i = 0; i < iParmItemCount; i++)
    {
        pDataBuf = pDataAddr[i];
        iParmWritePos = 0;

        while (iParmWritePos < DataLen[i])
        {
            if (iFlashRWBufLen - iFlashRWBufReadPos >  DataLen[i] - iParmWritePos)
            {
                memcpy(pDataBuf + iParmWritePos, cFlashRWBuf + iFlashRWBufReadPos, DataLen[i] - iParmWritePos);

                iFlashRWBufReadPos += DataLen[i] - iParmWritePos;

                break;  //进入下一参数项处理
            }
            else
            {
                memcpy(pDataBuf + iParmWritePos, cFlashRWBuf + iFlashRWBufReadPos, iFlashRWBufLen - iFlashRWBufReadPos);

                iParmWritePos += iFlashRWBufLen - iFlashRWBufReadPos;

                //cFlashRWBuf中的数据已全部存放到参数存储区，继续从FLASH读取
                ulCurReadCount =  FlashRead(ulFlashRWAddr, cFlashRWBuf, iFlashRWBufLen);
                if (ulCurReadCount != iFlashRWBufLen)
                {
                    //__enable_irq();
                    return 1;
                }
                ulFlashRWAddr += iFlashRWBufLen;

                iFlashRWBufReadPos = 0;
            }
        }
    }

    //计算校验和
    ucCheckSum = 0;

    for (i = 0; i < iParmItemCount; i++)
    {
        pDataBuf = pDataAddr[i];

        for (j = 0; j < DataLen[i]; j++)
        {
            ucCheckSum += pDataBuf[j];
        }
    }

    if (ucCheckSum != ucSaveCheckSum)
    {
        //__enable_irq();
        return 1;
    }

    //__enable_irq();

#endif

    return 0;
}


/*
函 数 名：uBit32 SPM_SaveConf(void)
功    能：保存系统配置信息到FLASH
参    数：无
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：

uBit32 SPM_SaveConfig(void)        
{
#ifdef SPM_M0
    unsigned long ulErrorCode;
    int iParmItemCount;
    char* pDataAddr[2];
    uBit32 DataLen[2];
    uBit32 ulTotalLen;
    unsigned char ucCheckSum;
    char* pDataBuf;
    char cFlashRWBuf[512];
    int iFlashRWBufLen;
    int iFlashRWBufWritePos;
    int iFlashRWBufFreeLen;
    int iParmReadPos;
    int i, j;

    uBit32 ulFlashRWAddr;
    uBit32 ulMismatchOffset;

    const DEVICE_INFO* pDeviceInfo = DEV_GetDevInfoReadAddr();


    //检测通道是否空闲
    if (!CSM_CheckSysFree())
    {
        ulErrorCode = ECM_GenerateErrorCode(SYS_SOFT_MODULE_SPM, ERR_CRD_NO_INVALID, SPM_ERR_CRDSYS_NOT_FREE, 
            SPM_CMD_SAVE_SYS_PARM, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }

    //检测插补器是否空闲
    if (!IPO_CheckSysFree())
    {
        ulErrorCode = ECM_GenerateErrorCode(SYS_SOFT_MODULE_SPM, ERR_CRD_NO_INVALID, SPM_ERR_IPO_NOT_FREE, 
            SPM_CMD_SAVE_SYS_PARM, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }

    //
    iParmItemCount = 2;

    pDataAddr[0] = (char*)pDeviceInfo;
    DataLen[0] = sizeof(DEVICE_INFO) * DEV_MAX_COUNT;

    pDataAddr[1] = (char*)m_SPM_pCrdAxisMapTable;
    DataLen[1] = sizeof(CRDSYS_AXIS_MAP_TABLE) * CRDSYS_MAX_COUNT;


    //检查存储区大小是否满足存储需要
    ulTotalLen = 0;
    for (i = 0; i < iParmItemCount; i++)
    {
        ulTotalLen += DataLen[i];
    }

    if (ulTotalLen > FLASH_M0_SYS_CONFIG_END_ADDR - FLASH_M0_SYS_CONFIG_START_ADDR + 1 - 5)   //数据存储区开始存放4字节标示字符和1字节校验和
    {
        ulErrorCode = ECM_GenerateErrorCode(SYS_SOFT_MODULE_SPM, ERR_CRD_NO_INVALID, SPM_ERR_LESS_FLASH_FOR_SAVE, 
            SPM_CMD_SAVE_SYS_CONFIG, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }

    //计算数据校验和
    ucCheckSum = 0;

    for (i = 0; i < iParmItemCount; i++)
    {
        pDataBuf = pDataAddr[i];

        for (j = 0; j < DataLen[i]; j++)
        {
            ucCheckSum += pDataBuf[j];
        }
    }

    //-----------------------------------------写数据到FLASH-----------------------------------------
    __disable_irq();

    ulErrorCode = ECM_GenerateErrorCode(SYS_SOFT_MODULE_SPM, ERR_CRD_NO_INVALID, SPM_ERR_FLASH_WRITE_FAIL, 
        SPM_CMD_SAVE_SYS_CONFIG, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);

    //
    //FlashInit(FLASH_M0_BASE_ADDR, CGU_GetPCLKFrequency(CGU_PERIPHERAL_M4CORE)/1000); 
    FlashInit(FLASH_M0_BASE_ADDR, M0_FLASH_CLK_FREQ); 

    //
    if (FlashErase_Pages(FLASH_M0_SYS_CONFIG_START_ADDR, ulTotalLen + 5))
    { //擦除失败
        __enable_irq();
        return ulErrorCode;
    }

    //
    ulFlashRWAddr = FLASH_M0_SYS_CONFIG_START_ADDR;

    //
    memset(cFlashRWBuf, 0, sizeof(cFlashRWBuf));
    iFlashRWBufLen = 512;

    //
    memcpy(cFlashRWBuf, "CONF", 4);

    //
    cFlashRWBuf[4] = ucCheckSum;

    iFlashRWBufWritePos = 5;
    iFlashRWBufFreeLen = iFlashRWBufLen - 5;

    //
    for (i = 0; i < iParmItemCount; i++)
    {
        pDataBuf = pDataAddr[i];
        iParmReadPos = 0;

        while(iParmReadPos < DataLen[i])
        {
            if (iFlashRWBufFreeLen < DataLen[i] - iParmReadPos)
            {
                memcpy(cFlashRWBuf + iFlashRWBufWritePos, pDataBuf + iParmReadPos, iFlashRWBufFreeLen);
                iParmReadPos += iFlashRWBufFreeLen;

                //
                if (FlashWrite(ulFlashRWAddr, cFlashRWBuf, iFlashRWBufLen))
                {
                    __enable_irq();
                    return ulErrorCode;
                }

                //
                if (FlashRAMCompare(ulFlashRWAddr, (uBit32)cFlashRWBuf, iFlashRWBufLen, &ulMismatchOffset))
                { //数据对比不成功，说明未成功写入FLASH
                    __enable_irq();
                    return ulErrorCode;
                }

                //
                ulFlashRWAddr += iFlashRWBufLen;

                iFlashRWBufWritePos = 0;
                iFlashRWBufFreeLen = iFlashRWBufLen;
                memset(cFlashRWBuf, 0, sizeof(cFlashRWBuf));
            }
            else
            {
                memcpy(cFlashRWBuf + iFlashRWBufWritePos, pDataBuf + iParmReadPos, DataLen[i] - iParmReadPos);

                iFlashRWBufWritePos += DataLen[i] - iParmReadPos;
                iFlashRWBufFreeLen -= DataLen[i] - iParmReadPos;

                if (iFlashRWBufFreeLen == 0 || i == iParmItemCount - 1)
                {
                    //
                    if (FlashWrite(ulFlashRWAddr, cFlashRWBuf, iFlashRWBufLen))
                    {
                        __enable_irq();
                        return ulErrorCode;
                    }

                    //
                    if (FlashRAMCompare(ulFlashRWAddr, (uBit32)cFlashRWBuf, iFlashRWBufLen, &ulMismatchOffset))
                    { //数据对比不成功，说明未成功写入FLASH
                        __enable_irq();
                        return ulErrorCode;
                    }

                    //
                    ulFlashRWAddr += iFlashRWBufLen;

                    iFlashRWBufWritePos = 0;
                    iFlashRWBufFreeLen = iFlashRWBufLen;
                    memset(cFlashRWBuf, 0, sizeof(cFlashRWBuf));
                }

                break; //参数项处理完成，进行下一数据项处理
            }
        }
    }

    __enable_irq();
#endif

    return 0;
}
*/

/*
函 数 名：uBit32 SPM_WriteConfToFlash(void)
功    能：保存配置信息和控制参数到FLASH
参    数：无
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 SPM_WriteConfToFlash(void)
{
/*  本部分代码需重写  
    unsigned long ulErrorCode;
    int iParmItemCount;
    char* pDataAddr[2];
    uBit32 DataLen[2];
    uBit32 ulTotalLen;
    unsigned char ucCheckSum;
    char* pDataBuf;
    char cFlashRWBuf[512];
    int iFlashRWBufLen;
    int iFlashRWBufWritePos;
    int iFlashRWBufFreeLen;
    int iParmReadPos;
    int i, j;

    uBit32 ulFlashRWAddr;
    uBit32 ulMismatchOffset;

    const DEVICE_INFO* pDeviceInfo = DEV_GetDevInfoReadAddr();

    iParmItemCount = 2;
    pDataAddr[0] = (char*)pDeviceInfo;
    DataLen[0] = sizeof(DEVICE_INFO) * DEV_MAX_COUNT;

    pDataAddr[1] = (char*)m_SPM_pCrdAxisMapTable;
    DataLen[1] = sizeof(CRDSYS_AXIS_MAP_TABLE) * CRDSYS_MAX_COUNT;


    //检查存储区大小是否满足存储需要
    ulTotalLen = 0;
    for (i = 0; i < iParmItemCount; i++)
    {
        ulTotalLen += DataLen[i];
    }

    if (ulTotalLen > FLASH_M0_SYS_CONFIG_END_ADDR - FLASH_M0_SYS_CONFIG_START_ADDR + 1 - 5)   //数据存储区开始存放4字节标示字符和1字节校验和
    {
        ulErrorCode = ECM_GenerateErrorCode(SYS_SOFT_MODULE_SPM, ERR_CRD_NO_INVALID, SPM_ERR_LESS_FLASH_FOR_SAVE, 
            SPM_CMD_SAVE_TO_CONTROLLER, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }

    //计算数据校验和
    ucCheckSum = 0;

    for (i = 0; i < iParmItemCount; i++)
    {
        pDataBuf = pDataAddr[i];

        for (j = 0; j < DataLen[i]; j++)
        {
            ucCheckSum += pDataBuf[j];
        }
    }

    //-----------------------------------------写数据到FLASH-----------------------------------------
    ulErrorCode = ECM_GenerateErrorCode(SYS_SOFT_MODULE_SPM, ERR_CRD_NO_INVALID, SPM_ERR_FLASH_WRITE_FAIL, 
        SPM_CMD_SAVE_TO_CONTROLLER, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);

    //
    ulFlashRWAddr = FLASH_M0_SYS_CONFIG_START_ADDR;

    //
    memset(cFlashRWBuf, 0, sizeof(cFlashRWBuf));
    iFlashRWBufLen = 512;

    //
    memcpy(cFlashRWBuf, "CONF", 4);

    //
    cFlashRWBuf[4] = ucCheckSum;

    iFlashRWBufWritePos = 5;
    iFlashRWBufFreeLen = iFlashRWBufLen - 5;

    //
    for (i = 0; i < iParmItemCount; i++)
    {
        pDataBuf = pDataAddr[i];
        iParmReadPos = 0;

        while(iParmReadPos < DataLen[i])
        {
            if (iFlashRWBufFreeLen < DataLen[i] - iParmReadPos)
            {
                memcpy(cFlashRWBuf + iFlashRWBufWritePos, pDataBuf + iParmReadPos, iFlashRWBufFreeLen);
                iParmReadPos += iFlashRWBufFreeLen;

                //
                if (FlashWrite(ulFlashRWAddr, cFlashRWBuf, iFlashRWBufLen))
                {
                    return ulErrorCode;
                }

                //
                if (FlashRAMCompare(ulFlashRWAddr, (uBit32)cFlashRWBuf, iFlashRWBufLen, &ulMismatchOffset))
                { //数据对比不成功，说明未成功写入FLASH
                    return ulErrorCode;
                }

                //
                ulFlashRWAddr += iFlashRWBufLen;

                iFlashRWBufWritePos = 0;
                iFlashRWBufFreeLen = iFlashRWBufLen;
                memset(cFlashRWBuf, 0, sizeof(cFlashRWBuf));
            }
            else
            {
                memcpy(cFlashRWBuf + iFlashRWBufWritePos, pDataBuf + iParmReadPos, DataLen[i] - iParmReadPos);

                iFlashRWBufWritePos += DataLen[i] - iParmReadPos;
                iFlashRWBufFreeLen -= DataLen[i] - iParmReadPos;

                if (iFlashRWBufFreeLen == 0 || i == iParmItemCount - 1)
                {
                    //
                    if (FlashWrite(ulFlashRWAddr, cFlashRWBuf, iFlashRWBufLen))
                    {
                        return ulErrorCode;
                    }

                    //
                    if (FlashRAMCompare(ulFlashRWAddr, (uBit32)cFlashRWBuf, iFlashRWBufLen, &ulMismatchOffset))
                    { //数据对比不成功，说明未成功写入FLASH
                        return ulErrorCode;
                    }

                    //
                    ulFlashRWAddr += iFlashRWBufLen;

                    iFlashRWBufWritePos = 0;
                    iFlashRWBufFreeLen = iFlashRWBufLen;
                    memset(cFlashRWBuf, 0, sizeof(cFlashRWBuf));
                }

                break; //参数项处理完成，进行下一数据项处理
            }
        }
    }
*/
    return 0;
}

//从FLASH读取系统配置信息，成功返回0，否则返回非0值
uBit32 SPM_ReadConfigFromFlash(char* pDeviceInfoBuf, uBit32 ulDeviceInfoLen)
{
 /* 
    int iParmItemCount;
    char* pDataAddr[2];
    uBit32 DataLen[2];
    uBit32 ulTotalLen;
    unsigned char ucCheckSum;
    unsigned char ucSaveCheckSum;
    char* pDataBuf;
    char cFlashRWBuf[512];
    int iFlashRWBufLen;
    int iFlashRWBufReadPos;
    int iParmWritePos;
    uBit32 ulCurReadCount; 
    int i, j;

    uBit32 ulFlashRWAddr;

    //
    iParmItemCount = 2;
    
    pDataAddr[0] = pDeviceInfoBuf;
    DataLen[0] = ulDeviceInfoLen;

    pDataAddr[1] = (char*)m_SPM_pCrdAxisMapTable;
    DataLen[1] = sizeof(CRDSYS_AXIS_MAP_TABLE) * CRDSYS_MAX_COUNT;


    //
    ulTotalLen = 0;
    for (i = 0; i < iParmItemCount; i++)
    {
        ulTotalLen += DataLen[i];
    }

    if (ulTotalLen + 5 > FLASH_M0_SYS_CONFIG_END_ADDR - FLASH_M0_SYS_CONFIG_START_ADDR + 1)   //数据存储区开始存放4字节标示字符和1字节校验和
    {
        return 1;
    }

    //-----------------------------------------------从FLASH读取数据-------------------------------------------
    //__disable_irq();   //读FLASH不用关中断

    //
    memset(cFlashRWBuf, 0, sizeof(cFlashRWBuf));
    iFlashRWBufLen = 512;

    ulFlashRWAddr = FLASH_M0_SYS_CONFIG_START_ADDR;

    ulCurReadCount =  FlashRead(ulFlashRWAddr, cFlashRWBuf, iFlashRWBufLen);
    if (ulCurReadCount != iFlashRWBufLen)
    {
        //__enable_irq();
        return 1;
    }
    ulFlashRWAddr += iFlashRWBufLen;

    if (cFlashRWBuf[0] != 'C' || cFlashRWBuf[1] != 'O' || cFlashRWBuf[2] != 'N' ||cFlashRWBuf[3] != 'F')
    {
        //__enable_irq();
        return 1;
    }

    ucSaveCheckSum = cFlashRWBuf[4];

    iFlashRWBufReadPos = 5;

    //
    for (i = 0; i < iParmItemCount; i++)
    {
        pDataBuf = pDataAddr[i];
        iParmWritePos = 0;

        while (iParmWritePos < DataLen[i])
        {
            if (iFlashRWBufLen - iFlashRWBufReadPos >  DataLen[i] - iParmWritePos)
            {
                memcpy(pDataBuf + iParmWritePos, cFlashRWBuf + iFlashRWBufReadPos, DataLen[i] - iParmWritePos);

                iFlashRWBufReadPos += DataLen[i] - iParmWritePos;

                break;  //进入下一参数项处理
            }
            else
            {
                memcpy(pDataBuf + iParmWritePos, cFlashRWBuf + iFlashRWBufReadPos, iFlashRWBufLen - iFlashRWBufReadPos);

                iParmWritePos += iFlashRWBufLen - iFlashRWBufReadPos;

                //cFlashRWBuf中的数据已全部存放到参数存储区，继续从FLASH读取
                ulCurReadCount =  FlashRead(ulFlashRWAddr, cFlashRWBuf, iFlashRWBufLen);
                if (ulCurReadCount != iFlashRWBufLen)
                {
                    //__enable_irq();
                    return 1;
                }
                ulFlashRWAddr += iFlashRWBufLen;

                iFlashRWBufReadPos = 0;
            }
        }
    }

    //计算校验和
    ucCheckSum = 0;

    for (i = 0; i < iParmItemCount; i++)
    {
        pDataBuf = pDataAddr[i];

        for (j = 0; j < DataLen[i]; j++)
        {
            ucCheckSum += pDataBuf[j];
        }
    }

    if (ucCheckSum != ucSaveCheckSum)
    {
        //__enable_irq();
        return 1;
    }

    //__enable_irq();
*/
    return 0;

}

/*
函 数 名：uBit32 SPM_SaveConfAndParm(void)
功    能：保存配置信息和控制参数到FLASH
参    数：无
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 SPM_SaveConfAndParm(void)
{
 /* 
    unsigned long ulErrorCode;

    //检测通道是否空闲
    if (!CSM_CheckSysFree())
    {
        ulErrorCode = ECM_GenerateErrorCode(SYS_SOFT_MODULE_SPM, ERR_CRD_NO_INVALID, SPM_ERR_CRDSYS_NOT_FREE, 
            SPM_CMD_SAVE_TO_CONTROLLER, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }

    //检测插补器是否空闲
    if (!IPO_CheckSysFree())
    {
        ulErrorCode = ECM_GenerateErrorCode(SYS_SOFT_MODULE_SPM, ERR_CRD_NO_INVALID, SPM_ERR_IPO_NOT_FREE, 
            SPM_CMD_SAVE_TO_CONTROLLER, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);
        return ulErrorCode;
    }

    //-----------------------------------------写数据到FLASH-----------------------------------------
    __disable_irq();

    ulErrorCode = ECM_GenerateErrorCode(SYS_SOFT_MODULE_SPM, ERR_CRD_NO_INVALID, SPM_ERR_FLASH_WRITE_FAIL, 
        SPM_CMD_SAVE_TO_CONTROLLER, ERR_DEVICE_NO_INVALID, ERR_LEVEL_NORMAL);

    //擦除FLASH扇区
    FlashInit(FLASH_M0_BASE_ADDR, M0_FLASH_CLK_FREQ); 
    //
    if (FlashErase_Sectors(FLASH_M0_SYS_CONFPARM_START_ADDR,FLASH_M0_SYS_CONFPARM_END_ADDR-FLASH_M0_SYS_CONFPARM_START_ADDR+1))
    {
        __enable_irq();
        return ulErrorCode;
    }


    //写系统配置数据到Flash
    ulErrorCode = SPM_WriteConfToFlash();
    if (ulErrorCode)
    {
        __enable_irq();
        return ulErrorCode;
    }

    //写系统控制参数Flash
    ulErrorCode = SPM_WriteParmToFlash();
    if (ulErrorCode)
    {
        __enable_irq();
        return ulErrorCode;
    }

    __enable_irq();
*/
    return 0;
}

//从FLASH加载系统参数，成功返回0，否则返回非0值
uBit32 SPM_LoadParm()
{    
    int i;

    //
    if (SPM_ReadParmFromFlash())
    { //读取失败
        return 1;
    }

    //
    if (!SPM_CheckSysCtrlParm(m_SPM_pSysCtrlParm))
    {
        return 1;
    }

    //
    for (i = 0; i < CRDSYS_MAX_COUNT; i++)
    {
        if (!SPM_CheckCrdSysParm(m_SPM_pCrdParm + i))
        {
            return 1;
        }
    }

    //
    for (i = 0; i < AXIS_MAX_COUNT; i++)
    {
        if (!SPM_CheckAxisParm(m_SPM_pAxisParm + i))
        {
            return 1;
        }
    }

    //
    for (i = 0; i < AXIS_MAX_COUNT; i++)
    {
        if (!SPM_CheckAxisPitchCmpParm(m_SPM_pAxisPitchCmpParm + i))
        {
            return 1;
        }
    }

    return 0;
}


//检查两个设备信息是否对应同一个设备，是同一设备返回1，否则返回0
int SPM_ChckDevMatch(DEVICE_INFO* pCurDevInfo, DEVICE_INFO* pLastDevInfo)
{
    DEVICE_INFO TempDeviceInfo;

    if (pCurDevInfo->nBusType == DEV_BUS_TYPE_MECHALINK_III && pLastDevInfo->nBusType == DEV_BUS_TYPE_MECHALINK_III)
    {
        if (pCurDevInfo->nDevieID == pLastDevInfo->nDevieID)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        //
        /*  考虑以后更换设备方便，暂不做该判定
        if (pCurDevInfo->nDeviceNOLast != pLastDevInfo->nDeviceNO)
        {
            return 0;
        }
        */

        //
        TempDeviceInfo = *pCurDevInfo;
        TempDeviceInfo.nDeviceNO = pLastDevInfo->nDeviceNO;
        TempDeviceInfo.nDeviceNOLast = pLastDevInfo->nDeviceNOLast;

        if (memcmp(&TempDeviceInfo, pLastDevInfo, sizeof(DEVICE_INFO)))
        {
            return 0;
        }
        else
        {
            return 1;
        }
    }
}


//从FLASH加载系统配置信息，成功返回0，否则返回非0值
//调用本函数前，应已完成设备扫描
uBit32 SPM_LoadConfig()
{
    int i,j,k;
    DEVICE_INFO SavDeviceInfo[DEV_MAX_COUNT];
    const DEVICE_INFO *pDeviceInfo;
    uBit32 ulCurDevCount=0;
    //uBit32 ulSavCodedDevCount=0;
    DEVICE_CONFIG DeviceConfig[DEV_MAX_COUNT];
    uBit32 ulRegDveCount = 0;
    uBit32 ulDevConfigIndex = 0;

    //
    memset(SavDeviceInfo, 0, sizeof(SavDeviceInfo));
    memset(DeviceConfig, 0, sizeof(DeviceConfig));

    //读取配置信息
    if (SPM_ReadConfigFromFlash((char*)SavDeviceInfo, sizeof(SavDeviceInfo))==0)
    { 
        //-------------------------检查系统当前扫描到的硬件信息和FLASH中存储的硬件信息是否匹配-----------------
        ulCurDevCount = DEV_GetCount();
        pDeviceInfo = DEV_GetDevInfoReadAddr();

        //检查所有保存的已编号设备是否依然存在
        for (i=0; i<DEV_MAX_COUNT; i++)
        {
            if (SavDeviceInfo[i].nDeviceNO != DEV_INVALID_NO)//保存的已编号设备
            {
                ulRegDveCount++;
                for (j=0; j<ulCurDevCount; j++)
                {
                    if (SPM_ChckDevMatch((DEVICE_INFO *)&pDeviceInfo[j], SavDeviceInfo+i))
                    {
                        DeviceConfig[ulDevConfigIndex].nDeviceNO = SavDeviceInfo[i].nDeviceNO;
                        DeviceConfig[ulDevConfigIndex].nDevieID = SavDeviceInfo[i].nDevieID;
                        ulDevConfigIndex++;
                        break;
                    }
                }
            }
        }

        //有设备更改或增减
        if (ulRegDveCount==ulDevConfigIndex && ulCurDevCount==ulRegDveCount)
        {
            //下发配置信息
            if (DEV_SetConfig(DeviceConfig, ulRegDveCount)==0)
            {
                return 0;
            }
        }
    }

    CNCSYS_SetAlarmBit(SYS_ALARM_LOAD_SYS_CONFIG_FAIL);
    //坐标系映射表
    memset(m_SPM_pCrdAxisMapTable, 0, sizeof(CRDSYS_AXIS_MAP_TABLE) * CRDSYS_MAX_COUNT);

    for (i = 0; i < CRDSYS_MAX_COUNT; i++)
    {
        for (j = 0; j < CRDSYS_AXIS_MAX_COUNT; j++)
        {
            for (k = 0; k < AXIS_MAP_MOTOR_MAX_COUNT; k++)
            {
                m_SPM_pCrdAxisMapTable[i].CrdSysAxisMap[j].cMotorNO[k] = 0xFF;
            }
        }
    }

    return 1;
}

//设置硬件配置信息(设置硬件配置信息后，坐标系映射表需重新建立)
uBit32 SPM_SetConfig(DEVICE_CONFIG *pDevConf, uBit32 ulDevCount)
{
    int i, j;
    uBit32 ulErrorCode;
    uBit32 ulMotorCount;                //当前扫描到的电机设备总数
    uBit32 ulCodedMotorCount;        //上次编号设备总数
    CRDSYS_AXIS_MAP_TABLE pCrdAxisMap[CRDSYS_MAX_COUNT];

    //检查电机数量数量是否更改，如果电机数量更改则清除坐标系轴映射表
    ulMotorCount = DEV_GetMotorCount();
    ulCodedMotorCount = PAX_GetRegCount();

    //设置硬件配置信息
    ulErrorCode = DEV_SetConfig(pDevConf, ulDevCount);
    if (ulErrorCode)
        return ulErrorCode;

    if (ulMotorCount!=ulCodedMotorCount)
    {
        //先保存
        memcpy(pCrdAxisMap, m_SPM_pCrdAxisMapTable, sizeof(CRDSYS_AXIS_MAP_TABLE)*CRDSYS_MAX_COUNT);

        //清除原有映射表信息
        for (i=0; i<CRDSYS_MAX_COUNT; i++)
        {
            for (j=0; j<CRDSYS_AXIS_MAX_COUNT; j++)
            {
                m_SPM_pCrdAxisMapTable[i].CrdSysAxisMap[j].cMotorNO[0] = DEV_INVALID_NO;
                m_SPM_pCrdAxisMapTable[i].CrdSysAxisMap[j].cMotorNO[1] = DEV_INVALID_NO;
            }

            m_SPM_pCrdAxisMapTable[i].nCrdAxisCount=0;
        }

        //更新轴映射表

        //设置更新系统参数状态位（关插补中断）
        CNCSYS_SetStatusBit(SYS_STATUS_UPDATE_SYS_CTRL_PARM);

        //通知插补器更新坐标系映射表
        ulErrorCode = IPO_UpdateCrdSysAxisMapTable();
        if (ulErrorCode != 0)
        {
            CNCSYS_ClrStatusBit(SYS_STATUS_UPDATE_SYS_CTRL_PARM);
            return ulErrorCode;
        }

        //更新通道管理模块坐标系映射表
        ulErrorCode = CSM_UpdateCrdSysAxisMapTable();
        if (ulErrorCode != 0)
        {
            CNCSYS_ClrStatusBit(SYS_STATUS_UPDATE_SYS_CTRL_PARM);
            return ulErrorCode;
        }

        CNCSYS_ClrStatusBit(SYS_STATUS_UPDATE_SYS_CTRL_PARM);
        CNCSYS_SetAlarmBit(SYS_ALARM_LOAD_SYS_CONFIG_FAIL);
    }

    return 0;
}


/*
函 数 名：Bit32 SPM_GetAxisMapTable(Bit32 nCh, Bit32 iAxisNo)
功    能：获取通道逻辑轴号的物理轴号
参    数：    nCh        --通道序号，从0开始
            iAxisNo    --轴序号，从0开始
返 回 值：物理轴号，-1：错误
调用位置：
注意事项：
*/
Bit32 SPM_GetAxisMapTable(Bit32 nCh, Bit32 iAxisNo)
{
    if (nCh<0 || nCh>=CRDSYS_MAX_COUNT || iAxisNo<0||iAxisNo>CRDSYS_AXIS_MAX_COUNT)
        return -1;

    if(m_SPM_pCrdAxisMapTable[nCh].nCrdAxisCount<iAxisNo)
        return -1;

    return m_SPM_pCrdAxisMapTable[nCh].CrdSysAxisMap[iAxisNo].cMotorNO[0];
}


/*
函 数 名：Bit32 SPM_GetAxisType(Bit32 nCh, Bit32 iAxisNo)
功    能：获取轴的类型
参    数：    nCh        --通道序号，从0开始
            iAxisNo    --轴序号，从0开始
返 回 值：轴类型
            #define AXIS_TYPE_NULL              (0)            //未安装
            #define AXIS_TYPE_LINE              (1)            //直线轴
            #define AXIS_TYPE_ROTATE            (2)            //旋转轴(角度值任意)
            #define AXIS_TYPE_ROTATE_360        (3)            //旋转轴(角度值为0-360)
调用位置：
注意事项：
*/
Bit32 SPM_GetAxisType(Bit32 nCh, Bit32 iAxisNo)
{
    int iMotorNo;

    iMotorNo = SPM_GetAxisMapTable(nCh, iAxisNo);

    if (iMotorNo==-1)
        return AXIS_TYPE_NULL;

    return m_SPM_pAxisParm[iMotorNo].AxisScaleParm.nType;
}