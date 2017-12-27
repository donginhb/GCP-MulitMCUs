#ifndef  __CMU_INIT_H
#define  __CMU_INIT_H

#include "DataType/DataType.h"

#ifdef __cplusplus
extern "C"
{
#endif


/**
  * @brief  CMU模块初始化
  * @param  uComType 端口类型
  * @retval 0-成功 非0-失败
  */
uBit32 CMU_Init(uBit8 uComType);


#ifdef __cplusplus
}
#endif

#endif /* __CMU_INIT_H */
