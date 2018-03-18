/**
  ******************************************************************************
  * @file    BS04_LedCtrl.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2018.01.25
  * @brief   
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "BS04_LedCtrl.h"
#include "BS04_HardwareDef.h"
#include "DataType/DataType.h"

#include "SysPeripheral/SysTimer/SysTimer.h"
#include "SysPeripheral/GPIO/GPIO_Man.h"

#include <string.h>


/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

#define BS04_LED_CTRL_MODE_AUTO          (0)     //自动模式
#define BS04_LED_CTRL_MODE_FORCE_OPEN    (1)     //常开
#define BS04_LED_CTRL_MODE_FORCE_CLOSE   (2)     //常闭

uBit8 m_irb_uLedCtrlMode = BS04_LED_CTRL_MODE_AUTO;  //LED控制模式


/*****************************************************************************
 * LED相关控制接口
 ****************************************************************************/

/**
  * @brief  LED控制模式设置
  * @param  uCtrlMode 控制模式
  * @retval None
  */
void BS04_SetLedCtrlMode(uBit8 uMode)
{
    m_irb_uLedCtrlMode = uMode;
    
}


/**
  * @brief  LED任务管理
  * @param  None
  * @retval None
  */
void BS04_LEDHandler(void)
{
    
    
    
}
