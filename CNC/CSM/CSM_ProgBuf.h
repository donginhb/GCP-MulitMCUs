
/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：CSM_ProgBuf.h

摘    要：程序缓冲区管理模块

运行环境： LPC43xx 

修改描述：

当前版本：2.0

修改作者：毛军

修改日期：2015年9月24日
*/

#ifndef CSM_PROG_BUF_H
#define CSM_PROG_BUF_H


#include "..\\DataStructDef\\IPR_DataStructDef.h"


//----------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

    int CSM_GetProgData(int iCrdSysIndex, unsigned long ulProgLineNO, IIP_BUF* pProgData);
    void CSM_ReleaseProgData(int iCrdSysIndex, unsigned long ulProgLineNO);

    //获取程序缓冲区状态字
    uBit32 CSM_GetProgBufStatus(int iCrdSysIndex);

#ifdef __cplusplus
}
#endif

#endif
