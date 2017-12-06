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

#include <string.h>
#include "AHB_DataStructDef.h"
#include "CNC/MemoryAddr/CNC_AddrDef.h"


/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

/*****************************************************************************
 * CNC系统状态相关控制接口
 ****************************************************************************/

AHB_DATA_STRUCT* g_pAHBDataStruct = NULL;


/**
  * @brief  AHB CNC系统配置参数初始化
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 AHB_Init(void)
{
    //缓冲区长度校验
    if (sizeof(AHB_DATA_STRUCT) > AHB_BUF_TOTAL_LEN)    //sizeof(AHB_DATA_STRUCT) == 3416
    {
        return 1;
    }
    
    //指向缓冲区
    g_pAHBDataStruct = (AHB_DATA_STRUCT*)AHB_BUF_START_ADDR;
    
    //初始化缓冲区
    memset(g_pAHBDataStruct, 0, sizeof(AHB_DATA_STRUCT));

    return 0;
}
