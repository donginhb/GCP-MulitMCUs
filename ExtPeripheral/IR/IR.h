#ifndef  __IR_H
#define  __IR_H

#include "DataType/DataType.h"

#ifdef __cplusplus
extern "C"
{
#endif
    
    
/**
  * @brief  红外值获取获取
  * @param  None
  * @retval 0-成功 非0-失败
  */
void IR_GetValue(uBit32 ulValue);


#ifdef __cplusplus
}
#endif

#endif /* __IR_H */
