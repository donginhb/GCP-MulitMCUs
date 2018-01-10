#ifndef __LCD1602_H
#define __LCD1602_H

#include "DataType/DataType.h"


#ifdef __cplusplus
extern "C"
{
#endif


/*****************************************************************************
 * LCD1602相关控制接口
 ****************************************************************************/

/**
  * @brief  NRF24L01 硬件接口
  * @param  pf_LCD_PORT     LCD并口数据脚控制
  * @param  pf_LCD_RS       LCD复位脚控制
  * @param  pf_LCD_EN       LCD使能脚控制
  * @param  pf_LCD_RW       LCD读写使能脚控制
  * @param  pf_LCD_DelayMs  LCD毫秒级延时
  * @retval 0-成功 非0-失败
  */
uBit32 LCD1602_InitInterfaces(void (*pf_LCD_PORT)(uBit8 uData),
                              void (*pf_LCD_RS)(bool bState),
                              void (*pf_LCD_EN)(bool bState),
                              void (*pf_LCD_RW)(bool bState),
                              void (*pf_LCD_DelayMs)(uBit32 ulMs));


/**
  * @brief  字符串输入
  * @param  pStr 字符串,必须以'\0'结束
  * @retval 无
  */
void LCD1602_WriteStr(uBit8 *pStr);


#ifdef __cplusplus
}
#endif

#endif /* __LCD1602_H */
