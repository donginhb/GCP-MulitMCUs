#ifndef __BS08M_MAIN_PROC_H
#define __BS08M_MAIN_PROC_H

#include "DataType/DataType.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
  * @brief  格子柜相关资源初始化
  * @param  None
  * @retval None
  */
void BS08M_Init(void);


/**
  * @brief  格子柜主任务处理
  * @param  None
  * @retval None
  */
void BS08M_MainProc(void);

#ifdef __cplusplus
}
#endif


#endif /* __BS08M_MAIN_PROC_H */
