/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：SDRAM_AddrDef.c

摘    要：SDRAM地址定义

运行环境：LPC43xx 

修改描述：

当前版本：1.0

修改作者：毛军

修改日期：2015年10月16日
*/

#include <string.h>
#include "SDRAM_AddrDef.h"


//SDRAM初始化，成功返回0，否则返回非0值
int SDRAM_Init()
{
    unsigned long i;
    unsigned long ulLen;
    unsigned long* pData = NULL;

    ulLen = SDRAM_TOTAL_LEN / 4;

    memset((char*)SDRAM_START_ADDR, 0xFF, SDRAM_TOTAL_LEN);

    pData = (unsigned long*)SDRAM_START_ADDR;
    for (i = 0; i < ulLen; i++)
    {
        if (*pData != 0xFFFFFFFF)
        {
            return 1;
        }

        pData++;
    }

    memset((char*)SDRAM_START_ADDR, 0x55, SDRAM_TOTAL_LEN);

    pData = (unsigned long*)SDRAM_START_ADDR;
    for (i = 0; i < ulLen; i++)
    {
        if (*pData != 0x55555555)
        {
            return 1;
        }
        pData++;
    }

    memset((char*)SDRAM_START_ADDR, 0xaa, SDRAM_TOTAL_LEN);

    pData = (unsigned long*)SDRAM_START_ADDR;
    for (i = 0; i < ulLen; i++)
    {
        if (*pData != 0xaaaaaaaa)
        {
            return 1;
        }
        pData++;
    }

    //
    memset((char*)SDRAM_START_ADDR, 0, SDRAM_TOTAL_LEN);


    return 0;    
}

