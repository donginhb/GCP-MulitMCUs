#ifndef  __HAL_EXTI_H
#define  __HAL_EXTI_H

#include "DataType/DataType.h"

//中断触发源触发
#define HAL_EXTI_TRG_RISING         (1) //上升沿触发
#define HAL_EXTI_TRG_FALLING        (2) //下降沿触发

#ifdef __cplusplus
extern "C"
{
#endif

/**
  * @brief  外部中断配置
  * @param  nPort 端口号
  * @param  nPin  引脚号
  * @param  uTrgSource 触发源
  * @retval 0-成功 非0-失败
  */
uBit32 HAL_EXTI_Init(uBit16 nPort, uBit16 nPin, uBit8 uTrgSource);


#ifdef __cplusplus
}
#endif

#endif /* __HAL_EXTI_H */
