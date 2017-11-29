#ifndef __VERSION_H
#define __VERSION_H

#include "DataType/DataType.h"
#include "GCPVersion.h" 
#include "Application/SLCVersion.h"


//软件版本特殊标示
#define SYS_FLAG_DEBUG                  'D'             //内部测试版本
#define SYS_FLAG_RELEASE                'R'             //发布版本


//软件版本数据结构体
typedef struct _SoftwareVersion 
{
    Bit8    cType[4];               //产品型号
    Bit8    cSoftVer[2];            //软件版本
    Bit8    cPart;                  //组件号(预留)

修改作者： 


#define APP_VER_NO0             (2)
#define APP_VER_NO1             (10)
#endif /* __VERSION_H */
