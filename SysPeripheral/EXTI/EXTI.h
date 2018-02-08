#ifndef  __EXTI_H
#define  __EXTI_H

#include "DataType/DataType.h"

//中断触发源触发
#define EXTI_TRG_RISING         (1) //上升沿触发
#define EXTI_TRG_FALLING        (2) //下降沿触发


#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************
 * EXTI相关控制接口
 ****************************************************************************/

/**
  * @brief  EXTI 初始化
  * @param  ulIntputNO IO资源表中输入IO组中的编号
  * @param  uTrgSource 触发源
  * @retval 0-成功 非0-失败
  */
uBit32 EXTI_Init(uBit32 ulIntputNO, uBit8 uTrgSource);


#ifdef __cplusplus
}
#endif

#endif /* __EXTI_H */
