/**
  ******************************************************************************
  * @file    HC595.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2018.02.01
  * @brief   Hc595 application
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "HC595.h"
#include "LogicIC.h"
#include "DataType/DataType.h"
      #include "SysPeripheral/GPIO/GPIO_Man.h"
#include "SysPeripheral/SysTimer/SysTimer.h"

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

static uBit32 m_ulSHCPNO = 0;
static uBit32 m_ulSTCPNO = 0;
static uBit32 m_ulDATANO = 0;


static void    (HC595_SetSHCP) (void *pData, uBit8 uState)
{
    GPIO_SetOutputState(m_ulSHCPNO, uState);
    
}

static void    (HC595_SetSTCP) (void *pData, uBit8 uState)
{
    GPIO_SetOutputState(m_ulSTCPNO, uState);
    
}

static void    (HC595_SetDATA) (void *pData, uBit8 uState)
{
    GPIO_SetOutputState(m_ulDATANO, uState);
    
}

static void    (HC595_DelayUs) (uBit32 Us)
{
    SysTime_DelayUs(Us);
}


/*****************************************************************************
 * 74HC595相关控制接口
 ****************************************************************************/

/**
  * @brief  HC595初始化
  * @param  None
  * @retval None
  */
void HC595_Init(uBit32 ulSHCPNO, uBit32 ulSTCPNO, uBit32 ulDATANO, uBit8 uParBitNum)
{
    HC595_HwCtrlInterFaces(NULL, HC595_SetSHCP, HC595_SetSTCP, HC595_SetDATA, HC595_DelayUs, uParBitNum);
    
}



