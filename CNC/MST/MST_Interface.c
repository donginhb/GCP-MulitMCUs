/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：MST_Interface.c

摘    要：mst模块接口函数

运行环境：LPC43xx 

修改描述：

当前版本：1.0

修改作者：毛军

修改日期：2015年10月13日

*/

#include "MST_Interface.h"

MST_INTERFACE m_MSTInterface;



//成功返回0，否则返回非0
int MST_InitInterface(MST_INTERFACE* pMSTInterface)
{
      if (pMSTInterface == NULL)
    {
        return 1;
    }



    //
    m_MSTInterface = *pMSTInterface;

    return 0;
}
