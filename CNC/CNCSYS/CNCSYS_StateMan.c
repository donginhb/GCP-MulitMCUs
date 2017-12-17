/**
  ******************************************************************************
  * @file    CNCSYS_StateMan.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.12.05
  * @brief   CNC system state mamage
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include <string.h>
#include "CNCSYS_Interface.h"
#include "CNC/DataStructDef/SYS_DataStructDef.h"
#include "CNC/DataStructDef/ERR_DataStructDef.h"
#include "CNC/ErrorCodeDef/SYS_ErrorCodeDef.h"
#include "CNC/ErrorCodeMan/ECM_ErrorCodeMan.h"
#include "CNC/CSM/CSM_Interface.h"
#include "CNC/DVM/DEV_Interface.h"
#include "CNC/SPM/SPM_SysCtrlParm.h"
#include "CNC/IPO/IPO_Interface.h"



/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

static CNSYS_STATE_DATA m_CncSysStateData;    //CNC 系统状态控制数据


/*****************************************************************************
 * CNC系统状态相关控制接口
 ****************************************************************************/

/**
  * @brief  系统状态数据初始化
  * @param  None
  * @retval None
  */
void CNCSYS_InitStateData(void)
{
    memset(&m_CncSysStateData, 0, sizeof(m_CncSysStateData));
    
}


/**
  * @brief  系统状态数据读取地址获取
  * @param  None
  * @retval 系统状态数据读取地址(指针)
  */
const CNSYS_STATE_DATA* CNCSYS_GetStateReadAddr(void)
{
    return &m_CncSysStateData;
}


/**
  * @brief  系统报警状态位设置
  * @param  ulAlarmBit 系统报警状态位
  * @retval None
  */
void CNCSYS_SetAlarmBit(uBit32 ulAlarmBit)
{
    m_CncSysStateData.ulSysExtAlarm |= ulAlarmBit;
}


/**
  * @brief  系统报警状态位清除
  * @param  ulAlarmBit 系统报警状态位
  * @retval None
  */
void CNCSYS_ClrAlarmBit(uBit32 ulAlarmBit)
{
    m_CncSysStateData.ulSysExtAlarm &= ~ulAlarmBit;
}


/**
  * @brief  系统报警状态位获取
  * @param  ulAlarmBit 系统报警状态位
  * @retval 1-存在报警 0-没有报警
  */
Bit32 CNCSYS_GetAlarmBit(uBit32 ulAlarmBit)
{
    if (m_CncSysStateData.ulSysExtAlarm & ulAlarmBit)
    {
        return 1;
    }
    
    return 0;
}


/**
  * @brief  系统报警状态位获取
  * @param  ulAlarmBit 系统报警状态位
  * @retval 1-存在报警 0-没有报警
  */
uBit32 CNCSYS_GetAlarmStatus(void)
{
    return m_CncSysStateData.ulSysExtAlarm;
}


/**
  * @brief  系统状态位获取
  * @param  Node
  * @retval 系统运行状态位
  */
uBit32 CNCSYS_GetSysStatus(void)
{
    return m_CncSysStateData.ulSysRunningStatus;
}


/**
  * @brief  系统状态位设置
  * @param  ulStatusBit 系统运行状态位
  * @retval Node
  */
void CNCSYS_SetStatusBit(uBit32 ulStatusBit)
{
    m_CncSysStateData.ulSysRunningStatus |= ulStatusBit;
}


/**
  * @brief  系统状态位清除
  * @param  ulStatusBit 系统运行状态位
  * @retval Node
  */
void CNCSYS_ClrStatusBit(uBit32 ulStatusBit)
{
    m_CncSysStateData.ulSysRunningStatus &= ~ulStatusBit;
}

