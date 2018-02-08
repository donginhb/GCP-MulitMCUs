#ifndef  __HW_DAC_H
#define  __HW_DAC_H

#include "chip.h"

#ifdef __cplusplus
extern "C"
{
#endif


/*****************************************************************************
 * DAC相关控制接口
 ****************************************************************************/

/**
  * @brief  DAC初始化
  * @param  None
  * @retval None
  */
void HW_DAC_Init(void);


/**
  * @brief  更新DAC输出(根据指定的数字量值)
  * @param  ulDigitalValue DAC数字量值
  * @retval None
  * @note   实际DAC输出模拟量电压 = (ulDigitalValue/硬件分辨率)*参考电压
  */
void HW_DAC_UpdateByDigitalValue(uint32_t ulDigitalValue);


/**
  * @brief  更新DAC输出(根据指定的模拟量值)
  * @param  uDacValue DAC模拟电压值,单位:毫伏
  * @retval None 
  * @note   注意电压范围最高为参考电压,超过该值均视为要输出参考电压
  */
void HW_DAC_UpdateByAnalogValue(uint32_t ulAnalogValue);


#ifdef __cplusplus
}
#endif

#endif /* __HW_DAC_H */
