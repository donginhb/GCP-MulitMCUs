
/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：ECM_DataStructDef.h

摘    要：错误管理模块内部数据结构定义

运行环境： LPC43xx 

修改描述：

当前版本：

修改作者：毛军

修改日期：2015年10月15日
*/

#ifndef ECM_DATA_STRUCT_DEF_H
#define ECM_DATA_STRUCT_DEF_H

#include "..\\DataStructDef\\DataType.h"


//错误码缓冲区个数宏定义
#define ERROR_CODE_MAX_COUNT          (20)   //错误码缓冲区最大错误码个数


//错误码管理数据结构体
typedef struct _ErrorCodeManData {
    ERROR_CODE  ErrorCode[ERROR_CODE_MAX_COUNT];    //错误码
    uBit16        nReadPos;                            //当前读位置
    uBit16      nWritePos;                            //当前写位置
    uBit16      nErrorCount;                        //当前存有条数
}ERROR_CODE_MAN_DATA;





#endif
