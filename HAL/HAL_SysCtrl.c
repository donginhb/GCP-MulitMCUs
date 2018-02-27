/**
  ******************************************************************************
  * @file    HAL_SysCtrl.h
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.10.19
  * @brief   MCU Ctrl Interface
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
   
   
/***********************************<INCLUDES>**********************************/
#include "HAL_SysCtrl.h"
#include "DataType/DataType.h"
#include "HwDrivers/HW_SysCtrl.h"


/*****************************************************************************
 * 系统复位相关
 ****************************************************************************/

/**
  * @brief  系统复位函数
  * @param  None
  * @retval None
  */
void HAL_SystemReset(void)
{
    HW_SystemReset();
    
}


/**
  * @brief  程序跳转
  * @param  None
  * @retval None
  * @note   此函数应用于Bootloader对APP的跳转
  */
void HAL_JumToApp(void)
{
#if (defined(LPC17XX)||defined(LPC43XX))
    HW_JumToApp();
#endif
    
}


/**
  * @brief  总中断使能
  * @param  None
  * @retval None
  */
void HAL_EnableIRQ(void)
{
    HW_EnableIRQ();
    
}


/**
  * @brief  总中断关闭
  * @param  None
  * @retval None
  */
void HAL_DisableIRQ(void)
{
    HW_DisableIRQ();
    
}

/*****************************************************************************
 * 系统JTAG引脚相关
 ****************************************************************************/

/**
  * @brief  JTAG模式设置,用于设置JTAG的模式
  * @param  mode:jtag,swd模式设置;00,全使能;01,使能SWD;10,全关闭;
  * @retval None
  */
void HAL_JTAGConfig(uBit8 uMode)
{
#if defined(STM32F10X)
    HW_SWJConfig(uMode);
#endif
    
}
