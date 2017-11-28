/*
Copyright (c) 2013, 东莞华科精机有限公司 All rights reserved.

文件名称：CMU_ExApi.h 

摘    要：下位机通信模块外部依赖接口内部调用函数声明

运行环境：Windows Xp / WINCE / LPC43xx / LPC1751

修改描述：

当前版本：3.0

修改作者：Alan

修改日期：2015年9月7日
*/

#ifndef _CMU_EX_API_H_
#define _CMU_EX_API_H_
#include "..\\CMU_DataStructDef.h"

/*
函数名称：void CMU_InitExApi(void)
功    能：初始化外部依赖接口
参    数：
返 回 值：无

注意事项：通信模块初始化调用，内部函数，为通信模块提供一套默认的外部接口
*/
//初始化外部接口，通信模块初始化调用，内部函数
extern void CMU_InitExApi(void);

extern CMU_EXTERNAL_FUN_TEBLE m_sExternalFunTable;
#endif

