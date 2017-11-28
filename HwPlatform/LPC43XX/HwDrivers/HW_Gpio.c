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


static const PINMUX_GRP_T  m_PinMuxGroup[164] = 
{
    {0x00,  0, FUNC0 | MD_EZI},
    {0x00,  1, FUNC0 | MD_EZI},
    
    {0x01,  0, FUNC0 | MD_EZI},
    {0x01,  1, FUNC0 | MD_EZI},
    {0x01,  2, FUNC0 | MD_EZI},
    {0x01,  3, FUNC0 | MD_EZI},
    {0x01,  4, FUNC0 | MD_EZI},
    {0x01,  5, FUNC0 | MD_EZI},
    {0x01,  6, FUNC0 | MD_EZI},
    {0x01,  7, FUNC0 | MD_EZI},
    {0x01,  8, FUNC0 | MD_EZI},
    {0x01,  9, FUNC0 | MD_EZI},
    {0x01, 10, FUNC0 | MD_EZI},
    {0x01, 11, FUNC0 | MD_EZI},
    {0x01, 12, FUNC0 | MD_EZI},
    {0x01, 13, FUNC0 | MD_EZI},
    {0x01, 14, FUNC0 | MD_EZI},
    {0x01, 15, FUNC0 | MD_EZI},
    {0x01, 16, FUNC0 | MD_EZI},
    {0x01, 17, FUNC0 | MD_EZI},
    {0x01, 18, FUNC0 | MD_EZI},
    //{0x01, 19, FUNC0 | MD_EZI},
    {0x01, 20, FUNC0 | MD_EZI},
    
    {0x02,  0, FUNC4 | MD_EZI},
    {0x02,  1, FUNC4 | MD_EZI},
    {0x02,  2, FUNC4 | MD_EZI},
    {0x02,  3, FUNC4 | MD_EZI},
    {0x02,  4, FUNC4 | MD_EZI},
    {0x02,  5, FUNC4 | MD_EZI},
    {0x02,  6, FUNC4 | MD_EZI},
    {0x02,  7, FUNC0 | MD_EZI},
    {0x02,  8, FUNC4 | MD_EZI},
    {0x02,  9, FUNC0 | MD_EZI},
    {0x02, 10, FUNC0 | MD_EZI},
    {0x02, 11, FUNC0 | MD_EZI},
    {0x02, 12, FUNC0 | MD_EZI},
    {0x02, 13, FUNC0 | MD_EZI},
    
    //{0x03,  0, FUNC0 | MD_EZI},
    {0x03,  1, FUNC4 | MD_EZI},
    {0x03,  2, FUNC4 | MD_EZI},
    //{0x03,  3, FUNC0 | MD_EZI},
    {0x03,  4, FUNC0 | MD_EZI},
    {0x03,  5, FUNC0 | MD_EZI},
    {0x03,  6, FUNC0 | MD_EZI},
    {0x03,  7, FUNC4 | MD_EZI},
    {0x03,  8, FUNC4 | MD_EZI},
    
    {0x04,  0, FUNC0 | MD_EZI},
    {0x04,  1, FUNC0 | MD_EZI},
    {0x04,  2, FUNC0 | MD_EZI},
    {0x04,  3, FUNC0 | MD_EZI},
    {0x04,  4, FUNC0 | MD_EZI},
    {0x04,  5, FUNC0 | MD_EZI},
    {0x04,  6, FUNC0 | MD_EZI},
    //{0x04,  7, FUNC0 | MD_EZI},
    {0x04,  8, FUNC4 | MD_EZI},
    {0x04,  9, FUNC4 | MD_EZI},
    {0x04, 10, FUNC4 | MD_EZI},
    
    {0x05,  0, FUNC0 | MD_EZI},
    {0x05,  1, FUNC0 | MD_EZI},
    {0x05,  2, FUNC0 | MD_EZI},
    {0x05,  3, FUNC0 | MD_EZI},
    {0x05,  4, FUNC0 | MD_EZI},
    {0x05,  5, FUNC0 | MD_EZI},
    {0x05,  6, FUNC0 | MD_EZI},
    {0x05,  7, FUNC0 | MD_EZI},
    
    //{0x06,  0, FUNC0 | MD_EZI},
    {0x06,  1, FUNC0 | MD_EZI},
    {0x06,  2, FUNC0 | MD_EZI},
    {0x06,  3, FUNC0 | MD_EZI},
    {0x06,  4, FUNC0 | MD_EZI},
    {0x06,  5, FUNC0 | MD_EZI},
    {0x06,  6, FUNC0 | MD_EZI},
    {0x06,  7, FUNC4 | MD_EZI},
    {0x06,  8, FUNC4 | MD_EZI},
    {0x06,  9, FUNC0 | MD_EZI},
    {0x06, 10, FUNC0 | MD_EZI},
    {0x06, 11, FUNC0 | MD_EZI},
    {0x06, 12, FUNC0 | MD_EZI},
    
    {0x07,  0, FUNC0 | MD_EZI},
    {0x07,  1, FUNC0 | MD_EZI},
    {0x07,  2, FUNC0 | MD_EZI},
    {0x07,  3, FUNC0 | MD_EZI},
    {0x07,  4, FUNC0 | MD_EZI},
    {0x07,  5, FUNC0 | MD_EZI},
    {0x07,  6, FUNC0 | MD_EZI},
    {0x07,  7, FUNC0 | MD_EZI},
    
    {0x08,  0, FUNC0 | MD_EZI},
    {0x08,  1, FUNC0 | MD_EZI},
    {0x08,  2, FUNC0 | MD_EZI},
    {0x08,  3, FUNC0 | MD_EZI},
    {0x08,  4, FUNC0 | MD_EZI},
    {0x08,  5, FUNC0 | MD_EZI},
    {0x08,  6, FUNC0 | MD_EZI},
    {0x08,  7, FUNC0 | MD_EZI},
    //{0x08,  8, FUNC0 | MD_EZI},
    
    {0x09,  0, FUNC0 | MD_EZI},
    {0x09,  1, FUNC0 | MD_EZI},
    {0x09,  2, FUNC0 | MD_EZI},
    {0x09,  3, FUNC0 | MD_EZI},
    {0x09,  4, FUNC4 | MD_EZI},
    {0x09,  5, FUNC4 | MD_EZI},
    {0x09,  6, FUNC0 | MD_EZI},
    
    //{0x0A,  0, FUNC0 | MD_EZI},
    {0x0A,  1, FUNC0 | MD_EZI},
    {0x0A,  2, FUNC0 | MD_EZI},
    {0x0A,  3, FUNC0 | MD_EZI},
    {0x0A,  4, FUNC4 | MD_EZI},
    
    {0x0B,  0, FUNC4 | MD_EZI},
    {0x0B,  1, FUNC4 | MD_EZI},
    {0x0B,  2, FUNC4 | MD_EZI},
    {0x0B,  3, FUNC4 | MD_EZI},
    {0x0B,  4, FUNC4 | MD_EZI},
    {0x0B,  5, FUNC4 | MD_EZI},
    {0x0B,  6, FUNC4 | MD_EZI},
   
    //{0x0C,  0, FUNC0 | MD_EZI},
    {0x0C,  1, FUNC4 | MD_EZI},
    {0x0C,  2, FUNC4 | MD_EZI},
    {0x0C,  3, FUNC4 | MD_EZI},
    {0x0C,  4, FUNC4 | MD_EZI},
    {0x0C,  5, FUNC4 | MD_EZI},
    {0x0C,  6, FUNC4 | MD_EZI},
    {0x0C,  7, FUNC4 | MD_EZI},
    {0x0C,  8, FUNC4 | MD_EZI},
    {0x0C,  9, FUNC4 | MD_EZI},
    {0x0C, 10, FUNC4 | MD_EZI},
    {0x0C, 11, FUNC4 | MD_EZI},
    {0x0C, 12, FUNC4 | MD_EZI},
    {0x0C, 13, FUNC4 | MD_EZI},
    {0x0C, 14, FUNC4 | MD_EZI},
    
    {0x0D,  0, FUNC4 | MD_EZI},
    {0x0D,  1, FUNC4 | MD_EZI},
    {0x0D,  2, FUNC4 | MD_EZI},
    {0x0D,  3, FUNC4 | MD_EZI},
    {0x0D,  4, FUNC4 | MD_EZI},
    {0x0D,  5, FUNC4 | MD_EZI},
    {0x0D,  6, FUNC4 | MD_EZI},
    {0x0D,  7, FUNC4 | MD_EZI},
    {0x0D,  8, FUNC4 | MD_EZI},
    {0x0D,  9, FUNC4 | MD_EZI},
    {0x0D, 10, FUNC4 | MD_EZI},
    {0x0D, 11, FUNC4 | MD_EZI},
    {0x0D, 12, FUNC4 | MD_EZI},
    {0x0D, 13, FUNC4 | MD_EZI},
    {0x0D, 14, FUNC4 | MD_EZI},
    {0x0D, 15, FUNC4 | MD_EZI},
    {0x0D, 16, FUNC4 | MD_EZI},
    
    {0x0E,  0, FUNC4 | MD_EZI},
    {0x0E,  1, FUNC4 | MD_EZI},
    {0x0E,  2, FUNC4 | MD_EZI},
    {0x0E,  3, FUNC4 | MD_EZI},
    {0x0E,  4, FUNC4 | MD_EZI},
    {0x0E,  5, FUNC4 | MD_EZI},
    {0x0E,  6, FUNC4 | MD_EZI},
    {0x0E,  7, FUNC4 | MD_EZI},
    {0x0E,  8, FUNC4 | MD_EZI},
    {0x0E,  9, FUNC4 | MD_EZI},
    {0x0E, 10, FUNC4 | MD_EZI},
    {0x0E, 11, FUNC4 | MD_EZI},
    {0x0E, 12, FUNC4 | MD_EZI},
    {0x0E, 13, FUNC4 | MD_EZI},
    {0x0E, 14, FUNC4 | MD_EZI},
    {0x0E, 15, FUNC4 | MD_EZI},
    
    //{0x0F,  0, FUNC0 | MD_EZI},
    {0x0F,  1, FUNC4 | MD_EZI},
    {0x0F,  2, FUNC4 | MD_EZI},
    {0x0F,  3, FUNC4 | MD_EZI},
    //{0x0F,  4, FUNC0 | MD_EZI},
    {0x0F,  5, FUNC4 | MD_EZI},
    {0x0F,  6, FUNC4 | MD_EZI},
    {0x0F,  7, FUNC4 | MD_EZI},
    {0x0F,  8, FUNC4 | MD_EZI},
    {0x0F,  9, FUNC4 | MD_EZI},
    {0x0F, 10, FUNC4 | MD_EZI},
    {0x0F, 11, FUNC4 | MD_EZI},
    
};



/**
  * @brief  GPIO初始化
  * @param  None
  * @retval None
  */
void HW_GPIO_Init(void)
{
    Chip_SCU_SetPinMuxing(m_PinMuxGroup, sizeof(m_PinMuxGroup)/sizeof(m_PinMuxGroup[0]));
    
}


/**
  * @brief  开漏IO配置
  * @param  nPort IO端口号
  * @param  nPin  IO引脚号
  * @retval None
  */
void HW_GPIO_ConfigOD(uint16_t nPort, uint16_t nPin)
{
    Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, nPort, nPin);
    //Chip_IOCON_EnableOD(LPC_IOCON, nPort, nPin);
    
}


/**
  * @brief  输入IO配置
  * @param  nPort IO端口号
  * @param  nPin  IO引脚号
  * @retval None
  */
void HW_GPIO_ConfigInput(uint16_t nPort, uint16_t nPin)
{
    Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, nPort, nPin);
    //Chip_IOCON_DisableOD(LPC_IOCON, nPort, nPin);
    
}


/**
  * @brief  输出IO配置
  * @param  nPort IO端口号
  * @param  nPin  IO引脚号
  * @retval None
  */
void HW_GPIO_ConfigOutput(uint16_t nPort, uint16_t nPin)
{
    Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, nPort, nPin);
    //Chip_IOCON_DisableOD(LPC_IOCON, nPort, nPin);
    
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
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, nPort, nPin, bState);
    
}


/**
  * @brief  IO状态获取
  * @param  nPort IO端口号
  * @param  nPin  IO引脚号
  * @retval bool IO状态
  */
bool HW_GPIO_GetIOState(uint16_t nPort, uint16_t nPin)
{
    
    return Chip_GPIO_GetPinState(LPC_GPIO_PORT, nPort, nPin);
}
