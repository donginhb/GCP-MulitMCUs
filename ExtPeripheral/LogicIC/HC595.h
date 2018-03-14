#ifndef  __HC595_H
#define  __HC595_H

#include "DataType/DataType.h"

#ifdef __cplusplus
extern "C"
{
#endif
    

/*****************************************************************************
 * 74HC595相关控制接口
 ****************************************************************************/

/**
  * @brief  HC595初始化
  * @param  ulSHCPNO SHCP时钟信号IO编号
  * @param  ulSTCPNO STCP数据锁存IO编号
  * @param  ulDATANO DATA串型数据IO编号
  * @param  uParBitNum 并行位数
  * @retval None
  */
void HC595_Init(uBit32 ulSHCPNO, uBit32 ulSTCPNO, uBit32 ulDATANO, uBit8 uParBitNum);


/**
  * @brief  HC595初始化
  * @param  ulData 要输出的并行数据
  * @retval None
  */
void HC595_SetData(uBit32 ulData);
    

#ifdef __cplusplus
}
#endif

#endif /* __HC595_H */
