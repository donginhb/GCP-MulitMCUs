/**
  ******************************************************************************
  * @file    TempSensor.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.11.25
  * @brief   TempSensor application
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "TempSensor.h"
#include "Ds18b20.h"
#include "DataType/DataType.h"
#include "SysPeripheral/GPIO/GPIO_Man.h"

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

static uBit32 m_ulBusPinNo = 0;    //数据引脚编号

/**
  * @brief  总线状态设置
  * @param  bStates true-高电平 false-低电平
  * @retval None
  */
static void TEMP_SetBusStates(bool bState)
{
    GPIO_SetOutputState(m_ulBusPinNo, bState);
    
}


/**
  * @brief  总线状态获取
  * @param  None
  * @retval true-高电平 false-低电平
  */
static bool TEMP_GetBusStates(void)
{
    
    return GPIO_GetOutputState(m_ulBusPinNo);
}


/*****************************************************************************
 * 温度传感器相关控制接口
 ****************************************************************************/

/**
  * @brief  温度模块接口初始化
  * @param  ulBusPinNO 总线IO在资源表中的编号
  * @param  pf_DelayUs 微秒级延时函数
  * @retval 0-成功 非0-失败
  */
uBit32 TEMP_InitInterface(uBit32 ulBusPinNO, void (*pf_DelayUs)(uBit32 ulUs))
{
    m_ulBusPinNo = ulBusPinNO;
    return DS18B20_InitInterfaces(TEMP_SetBusStates, TEMP_GetBusStates, pf_DelayUs);
}


/**
  * @brief  温度转化
  * @param  None
  * @retval None
  */
void TEMP_StartConvert(void)
{
    DS18B20_StartConvertTemp();
    
}


/**
  * @brief  获取温度值
  * @param  None
  * @retval 实际上的温度值
  */
float TEMP_GetValue(void)
{
    
    return DS18B20_GetTempVal();
}

