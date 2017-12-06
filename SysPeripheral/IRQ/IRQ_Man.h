#ifndef  __IRQ_MAN_H
#define  __IRQ_MAN_H

#include "DataType/DataType.h"
#include "HAL/HAL_IRQHandler.h"

#ifdef __cplusplus
extern "C"
{
#endif

    
/**
  * @brief  中断接口初始化
  * @param  None
  * @retval None
  * @note   本函数必须在所有硬件模块配置之前调用(重要)
  *         
  */
void IRQ_Init(void);


/**
  * @brief  设置中断触发回调
  * @param  ptr 回调指针
  * @param  ulTrgSource 触发源
  * @retval None
  */
void IRQ_SetTrgCallback(void (*ptr)(void), uBit32 ulTrgSource);


/**
  * @brief  释放中断触发回调
  * @param  ulTrgSource 触发源
  * @retval None
  */
void IRQ_ReleaseTrgCallback(uBit32 ulTrgSource);


#ifdef __cplusplus
}
#endif

#endif /* __IRQ_MAN_H */
