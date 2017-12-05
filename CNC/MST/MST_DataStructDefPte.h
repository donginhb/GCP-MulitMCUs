/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：MST_DataStructDef.h 

摘    要：MST模块内部数据结构定义

运行环境：LPC43xx

修改描述：

当前版本：1.0

修改作者：毛军

修改日期：2015年10月08日

*/

#ifndef MST_DATA_STRUCT_DEFINE_H
#define MST_DATA_STRUCT_DEFINE_H

#include "CNC/DataStructDef/DataType.h"
#include "CNC/DataStructDef/IPR_DataStructDef.h"


#pragma pack(push)
#pragma pack(8)        

//
typedef struct _MCodeExeState {
    uBit16 nExeStep;        //M代码执行步骤
    double64 dTime;            //执行时间
}MCODE_EXE_STATE;


//M代码数据结构体
typedef struct _MCODE_DATA {
    uBit16 nMCode;                    //M代码值
    uBit16 nValid;                    //标记M代码是否有效（执行完成后标记为无效）

    MCODE_PARM MCodeParm;            //M代码参数
    MCODE_EXE_STATE MCodeExeState;    //M代码执行状态

    uBit32 (*pfMCodeExe)(Bit32 iCrdSysIndex, MCODE_PARM* pMCodeParm, MCODE_EXE_STATE* pExeState);  //M代码执行函数指针
}MCODE_DATA;

//
#define PARA_MCODE_MAX_COUNT        (20)        //并行M代码最大数量

//并行M代码管理数据结构体
typedef struct _PARA_MCODE_MAN {
    //uBit16 nReadPos;
    //uBit16 nWritePos;
    uBit16 nCount;

    MCODE_DATA MCode[PARA_MCODE_MAX_COUNT];
}PARA_MCODE_MAN;


//
#define MST_EXE_END            (0)                //MST执行结束
#define MST_EXE_SCODE        (1)                //执行S代码
#define MST_EXE_TCODE        (2)                //执行T代码
#define MST_EXE_MCODE        (3)                //执行M代码


//
typedef struct _MST_CTRL_DATA {
    uBit16 nExeStep;


}MST_CTRL_DATA;













#pragma pack(pop)



#endif