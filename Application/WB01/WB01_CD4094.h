#ifndef  __WB01_CD4094_H
#define  __WB01_CD4094_H

#include "DataType/DataType.h"

#ifdef __cplusplus
extern "C"
{
#endif


/*****************************************************************************
 * CD4094相关控制接口
 ****************************************************************************/

/**
  * @brief  CD4094 设置
  * @param  nValue 要写入的数据
  * @retval None
  */
void WB01_CD4094_WriteValue(uBit16 nValue);



#ifdef __cplusplus
}
#endif

#endif /* __WB01_CD4094_H */
