/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：CSM_AutoCtrl.h

摘    要：通道自动模式运行控制

运行环境：LPC43xx 

修改描述：

当前版本：1.0

修改作者：毛军

修改日期：2015年9月27日

使用说明：

*/

#ifndef CSM_AUTOCTRL_H
#define CSM_AUTOCTRL_H


#ifdef __cplusplus
extern "C" {
#endif

    int CSM_CheckGCodeExeFinish(int iCrdSysIndex);
    void CSM_AutoMainCtrl(int iCrdSysIndex);

#ifdef __cplusplus
}
#endif

#endif