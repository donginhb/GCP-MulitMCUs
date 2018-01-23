#ifndef __BMC_MAIN_PROC_H
#define __BMC_MAIN_PROC_H

#include "DataType/DataType.h"

#ifdef __cplusplus
extern "C" {
#endif


/*****************************************************************************
 * 售货机主任务相关控制接口
 ****************************************************************************/

/**
  * @brief  售货机初始化
  * @param  None
  * @retval None
  */
void BMC_Init(void);


/**
  * @brief  售货机主任务处理
  * @param  None
  * @retval None
  */
void BMC_MainProc(void);


#ifdef __cplusplus
}
#endif


#endif /* __BMC_MAIN_PROC_H */
