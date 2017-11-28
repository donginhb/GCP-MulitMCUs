#ifndef __DB_MAIN_PROC_H
#define __DB_MAIN_PROC_H

#include "DataType/DataType.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
  * @brief  格子柜相关资源初始化
  * @param  None
  * @retval None
  */
void DB_Init(void);


/**
  * @brief  格子柜主任务处理
  * @param  None
  * @retval None
  */
void DB_MainProc(void);

#ifdef __cplusplus
}
#endif


#endif /* __DB_MAIN_PROC_H */
