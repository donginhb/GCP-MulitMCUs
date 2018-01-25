#ifndef  __DEMO_H
#define  __DEMO_H

#include "DataType/DataType.h"

#ifdef __cplusplus
extern "C"
{
#endif
    
/*****************************************************************************
 * RF通信相关控制接口
 ****************************************************************************/

/**
  * @brief  RF通信初始化
  * @param  None
  * @retval None
  */
void RF_InitCom(void);


/**
  * @brief  RF通信处理
  * @param  None
  * @retval None
  */
void RF_ComHandler(void);


#ifdef __cplusplus
}
#endif

#endif /* __DEMO_H */
