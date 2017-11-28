/**
  ******************************************************************************
  * @file    Exti.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.11.11
  * @brief   Exti API
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "Exti.h"
#include "DataType/DataType.h"
#include "HAL/HAL_Exti.h"
#include "SysPeripheral/GPIO/GPIO_Man.h"

extern GPIO_CTRL_TABLE  g_GpioCtrlTable;

/*****************************************************************************
 * EXTI相关控制接口
 ****************************************************************************/

/**
  * @brief  EXTI 初始化
  * @param  ulIntputNO IO资源表中输入IO组中的编号
  * @param  uTrgSource 触发源
  * @retval 0-成功 非0-失败
  */
uBit32 EXTI_Init(uBit32 ulIntputNO, uBit8 uTrgSource)
{
    if (ulIntputNO >= g_GpioCtrlTable.ulInputGroupLen)
    {
        return 1;
    }
    
    return HAL_EXTI_Init(g_GpioCtrlTable.pInputGroup[ulIntputNO].nPort, g_GpioCtrlTable.pInputGroup[ulIntputNO].nPin, uTrgSource);
}