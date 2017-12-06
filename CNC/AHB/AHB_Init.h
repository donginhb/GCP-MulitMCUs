/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：AHB_Init.h

摘    要：AHB初始化

运行环境：LPC43xx 

修改描述：

当前版本：1.0

修改作者：毛军

修改日期：2015年10月16日
*/

#ifndef __AHB_INIT_H
#define __AHB_INIT_H


#include "AHB_DataStructDef.h"

extern AHB_DATA_STRUCT* g_pAHBDataStruct;


#ifdef __cplusplus
extern "C" {
#endif

/**
  * @brief  AHB CNC系统配置参数初始化
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 AHB_Init(void);

#ifdef __cplusplus
}
#endif


#endif /* __AHB_INIT_H */
