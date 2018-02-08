/**
  ******************************************************************************
  * @file    HAL_Dac.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2018.02.07
  * @brief   Hal dac driver
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "Dac.h"
#include "DataType/DataType.h"
#include "HAL/HAL_Dac.h"


/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/


/*****************************************************************************
 * DAC相关控制接口
 ****************************************************************************/

/**
  * @brief  DAC初始化
  * @param  None
  * @retval None
  */
void DAC_Init(void)
{
    HAL_DAC_Init();
    
}


/**
  * @brief  更新DAC输出(根据指定的数字量值)
  * @param  ulDigitalValue DAC数字量值
  * @retval None
  * @note   实际DAC输出模拟量电压 = (ulDigitalValue/硬件分辨率)*参考电压
  */
void DAC_UpdateByDigitalValue(uBit32 ulDigitalValue)
{
    HAL_DAC_UpdateByDigitalValue(ulDigitalValue);
    
}


/**
  * @brief  更新DAC输出(根据指定的模拟量值)
  * @param  uDacValue DAC模拟电压值,单位：毫伏
  * @retval None 
  * @note   注意电压范围最高为参考电压,超过该值均视为要输出参考电压
  */
void DAC_UpdateByAnalogValue(uBit32 ulAnalogValue)
{
    HAL_DAC_UpdateByAnalogValue(ulAnalogValue);
    
}

