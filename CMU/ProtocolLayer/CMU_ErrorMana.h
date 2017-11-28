#ifndef _CMU_ERROR_MANA_H_
#define _CMU_ERROR_MANA_H_

#include "ComDataStructDef.h"
//#include "..\\..\\DataStructDef\\ERR_DataStructDef.h"

//------------------通信模块内部调用函数---------------------
#ifdef __cplusplus
extern "C" {
#endif

 /*
函数名称：uBit32 CMU_CreateError(uBit32 ulCodeNo, uBit32 ulID);

功    能：通信模块创建错误码

参    数：ulCodeNo--错误编号
          ulID------通信ID

返 回 值：0或错误码

注意事项：数据块接收完成后调用
*/ 
uBit32 CMU_CreateError(uBit32 ulCodeNo, uBit32 ulID);
#ifdef __cplusplus
}
#endif
#endif