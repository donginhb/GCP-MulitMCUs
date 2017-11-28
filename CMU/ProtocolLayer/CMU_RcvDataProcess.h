#ifndef _REC_DATA_PROCESS_H_
#define _REC_DATA_PROCESS_H_

#include "ComDataStructDef.h"

#ifdef __cplusplus
extern "C" {
#endif
    /*
    函数名称：uBit32 CMU_InitRecCtrlData(void)

    功    能：初始化接收控制数据区

    参    数：无

    返 回 值：0--成功 非0--错误编码

    注意事项：通信模块初始化时调用
    */  
    uBit32 CMU_InitRecCtrlData(void);

    /*
    函数名称：uBit32 CMU_MainRcvProc(void);

    功    能：接收数据处理线程，接收并处理接收数据包，处理完成后发送回包

    参    数：

    返 回 值：0--成功 非0--错误编码

    注意事项：通信模块主线程调用
    */
    uBit32 CMU_MainRcvProc(void);
#ifdef __cplusplus
}
#endif

#endif