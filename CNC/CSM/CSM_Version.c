/*
Copyright (c) 2016, 东莞华科精机有限公司 All rights reserved.

文件名称：CSM_Version.c

摘    要：通道管理模块版本信息

运行环境： LPC43xx 

修改描述：

当前版本：1.0

修改作者：毛军

修改日期：2016年5月23日
*/

#include "string.h"
#include "CSM_Interface.h"
#include "..\\DataStructDef\\DataType.h"

#include "..\\..\\Version\\VER_DataStructDef.h"



/*
函 数 名：uBit32 CSM_GetVersion(SOFTWARE_VERSION* pVersion)
功    能：获取通道管理模块版本信息
参    数：无
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 CSM_GetVersion(SOFTWARE_VERSION* pVersion)
{
    SOFTWARE_VERSION CSMVersion;

    memset(&CSMVersion, 0, sizeof(CSMVersion));

    memcpy(CSMVersion.cType, "NC20", 4);
    CSMVersion.cPart = SYS_PART_CSM;
    CSMVersion.cSoftVer[1] = 2;            
    CSMVersion.cFlag = 0;

    ////////////////////////////////////////////////////////////////
    //2016-05-23
    CSMVersion.cSoftVer[0] = 1;            

    ////////////////////////////////////////////////////////////////

    *pVersion = CSMVersion;

    return 0;
}
