/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：MST_TCode.h

摘    要：T代码处理模块

运行环境：LPC43xx 

修改描述：

当前版本：1.0

修改作者：毛军

修改日期：2015年10月12日

*/

#ifndef MST_TCODE_H
#define MST_TCODE_H



#ifdef __cplusplus
extern "C" {
#endif


    void MST_InitTCodeFun();

    uBit32 MST_ExeTCode(Bit32 iCrdSysIndex, uBit16 nTCode);





#ifdef __cplusplus
}
#endif



#endif