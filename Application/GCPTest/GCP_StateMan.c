/**
  ******************************************************************************
  * @file    GCP_StateMan.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.12.15
  * @brief   VD Machine system state manage 
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "GCP_StateMan.h"
#include "DataType/DataType.h"
      

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

typedef struct
{
    uBit32      ulCtrlStatus;            //系统控制字    
    uBit32      ulRunningStatus;         //系统状态字
    uBit32      ulExtAlarm;              //系统外部报警
    
}GCP_SYS_STATE_DATA;


static GCP_SYS_STATE_DATA m_vm_SysStateData = {0};       //系统状态数据定义


/*****************************************************************************
 * 系统报警状态相关控制接口
 ****************************************************************************/

/**
  * @brief  系统报警状态位设置
  * @param  ulAlarmBit 系统报警状态位
  * @retval None
  */
void GCP_SetAlarmBit(uBit32 ulAlarmBit)
{
    m_vm_SysStateData.ulExtAlarm |= ulAlarmBit;
}


/**
  * @brief  系统报警状态位清除
  * @param  ulAlarmBit 系统报警状态位
  * @retval None
  */
void GCP_ClrAlarmBit(uBit32 ulAlarmBit)
{
    m_vm_SysStateData.ulExtAlarm &= ~ulAlarmBit;
}


/**
  * @brief  系统报警状态位获取
  * @param  ulAlarmBit 系统报警状态位
  * @retval 1-存在报警 0-没有报警
  */
Bit32 GCP_GetAlarmBit(uBit32 ulAlarmBit)
{
    
    return (m_vm_SysStateData.ulExtAlarm & ulAlarmBit) ? 1 : 0;
}


/**
  * @brief  系统报警状态位获取
  * @param  ulAlarmBit 系统报警状态位
  * @retval 1-存在报警 0-没有报警
  */
uBit32 GCP_GetAlarmStatus(void)
{
    return m_vm_SysStateData.ulExtAlarm;
}

