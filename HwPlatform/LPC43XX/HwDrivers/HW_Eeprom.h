#ifndef  __HW_EEPROM_H
#define  __HW_EEPROM_H

#include "chip.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
  * @brief  EEPROM初始化
  * @param  None
  * @retval None
  */
void HW_EEPROM_Init(void);


/**
  * @brief  EEPROM字写入
  * @param  ulPage   页,范围为[0-127],最后一页不可写
  * @param  ulOffice 字偏移,范围为[0-31]
  * @param  ulValue  要写入的字
  * @retval None
  */
void HW_EEPROM_WriteWord(uint32_t ulPage, uint32_t ulOffice, uint32_t ulValue);


/**
  * @brief  EEPROM字读取
  * @param  ulPage   页,范围为[0-127],最后一页不可写
  * @param  ulOffice 字偏移,范围为[0-31]
  * @retval 读取到的字
  */
uint32_t HW_EEPROM_ReadWord(uint32_t ulPage, uint32_t ulOffice);


#ifdef __cplusplus
}
#endif

#endif /* __HW_EEPROM_H */

