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


AHB_DATA_STRUCT  m_pAHBDataStruct;

AHB_DATA_STRUCT* g_pAHBDataStruct = NULL;


//成功返回0，否则返回非0
uBit32 AHB_Init()
{
    /*
    if (sizeof(AHB_DATA_STRUCT) > AHB_SHARED_MEM_SIZE)
    {
        return 1;
    }

    g_pAHBDataStruct = (AHB_DATA_STRUCT*)AHB_SHARED_MEM_BASE_ADDR;
    */
    g_pAHBDataStruct = &m_pAHBDataStruct;

    memset(g_pAHBDataStruct, 0, sizeof(AHB_DATA_STRUCT));

    return 0;
}
