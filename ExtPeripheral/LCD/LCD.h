#ifndef  __LCD_H
#define  __LCD_H

#include "DataType/DataType.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
  * @brief  LCD 初始化
  * @param  pDataNoGroup 并口IO编号组
  * @param  ulENNo EN 脚编号
  * @param  ulRWNo RW 脚编号
  * @param  ulRSNo RS 脚编号
  * @retval 0-成功 非0-失败
  */
uBit32 LCD_Init(uBit32 *pDataNoGroup, uBit32 ulENNo, uBit32 ulRWNo, uBit32 ulRSNo);


/**
  * @brief  字符串输入
  * @param  uRowIndex 显示行索引
  * @param  uColIndex 显示列索引
  * @param  pStr 字符串,必须以'\0'结束
  * @retval 无
  */
void LCD_WriteStr(uBit8 uRowIndex, uBit8 uColIndex, uBit8 *pStr);


#ifdef __cplusplus
}
#endif

#endif /* __LCD_H */
