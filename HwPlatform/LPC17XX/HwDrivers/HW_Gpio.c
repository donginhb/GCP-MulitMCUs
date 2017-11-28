/**
  ******************************************************************************
  * @file    HW_Gpio.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.08.22
  * @brief   Flash drivers
  ******************************************************************************
  * @attention
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "HW_Gpio.h"
#include "chip.h"


/**
  * @brief  开漏IO配置
  * @param  nPort IO端口号
  * @param  nPin  IO引脚号
  * @retval None
  */
void HW_GPIO_ConfigOD(uint16_t nPort, uint16_t nPin)
{
    Chip_GPIO_SetPinDIROutput(LPC_GPIO, nPort, nPin);
    Chip_IOCON_EnableOD(LPC_IOCON, nPort, nPin);
    
}


/**
  * @brief  输入IO配置
  * @param  nPort IO端口号
  * @param  nPin  IO引脚号
  * @retval None
  */
void HW_GPIO_ConfigInput(uint16_t nPort, uint16_t nPin)
{
    Chip_GPIO_SetPinDIRInput(LPC_GPIO, nPort, nPin);
    Chip_IOCON_DisableOD(LPC_IOCON, nPort, nPin);
    
}


/**
  * @brief  输出IO配置
  * @param  nPort IO端口号
  * @param  nPin  IO引脚号
  * @retval None
  */
void HW_GPIO_ConfigOutput(uint16_t nPort, uint16_t nPin)
{
    Chip_GPIO_SetPinDIROutput(LPC_GPIO, nPort, nPin);
    Chip_IOCON_DisableOD(LPC_IOCON, nPort, nPin);
    
}


/**
  * @brief  输出IO状态设置
  * @param  nPort IO端口号
  * @param  nPin  IO引脚号
  * @param  bState IO状态
  * @retval None
  */
void HW_GPIO_SetOutputState(uint16_t nPort, uint16_t nPin, bool bState)
{
    Chip_GPIO_SetPinState(LPC_GPIO, nPort, nPin, bState);
    
}


/**
  * @brief  IO状态获取
  * @param  nPort IO端口号
  * @param  nPin  IO引脚号
  * @retval bool IO状态
  */
bool HW_GPIO_GetIOState(uint16_t nPort, uint16_t nPin)
{
    
    return Chip_GPIO_GetPinState(LPC_GPIO, nPort, nPin);
}
