#ifndef _SEND_DATA_PROCESS_H_
#define _SEND_DATA_PROCESS_H_
#include "ComDataStructDef.h"

//测试----------
//extern int g_iSendTick;
//--------------

#ifdef __cplusplus
extern "C" {
#endif
  
    /*
    函数名称：BooLean CMU_IsSendFinished()

    功    能：检查是否还有数据未发送

    参    数：

    返 回 值：false--数据已经发送完成
              ture---尚有数据未发送

    注意事项：在有数据未发送完成时暂停发送周期性查询数据和错误管理模块中的错误信息
    */
    BooLean CMU_IsSendFinished();

    /*
    函数名称：void CMU_ResetSendCtrlData(uBit32 ulID, uBit8* pBuf, uBit32 ulLen)

    功    能：重置发送控制数据区

    参    数：ulID---发送数据包ID
              pBuf---待发送的配置数据，当pBuf==NULL时，使用系统默认的发送缓冲区(必须将要发送的数据直接保存在默认发送缓冲区中)
              ulLen--待发数据长度

    返 回 值：0--成功 非0--错误编码

    注意事项：可以配置发送缓冲区为默认发送缓冲区，适合发送临时数据
              也可以配置外部发送缓冲区，适合发送固定地址段数据如控制参数等
    */
    uBit32 CMU_ResetSendCtrlData(uBit32 ulID, uBit8* pBuf, uBit32 ulLen);
    
    /*
    函数名称：uBit32 CMU_SendResponsePack(uBit32 ulID, uBit32 ulErrCode)

    功    能：创建回应数据包,并立即发送，在设置指令处理完成后和接收出错时调用

    参    数：ulID---回应包ID
              ulErrCode---回应包错误码

    返 回 值：0--正确  非0--错误码

    注意事项：
    */
    uBit32 CMU_SendResponsePack(uBit32 ulID, uBit32 ulErrCode);
    
    /*
    函数名称：uBit32 CMU_AddToSendCtrlData(uBit8* pBuf, uBit32 ulLen)

    功    能：将数据缓冲区添加到发送控制数据缓冲区中

    参    数：pBuf---待添加的数据首地址
              ulLne--待添加的数据长度

    返 回 值：0--正确  非0--错误码

    注意事项：重置发送控制数据区后调用，如果pBuf不为NULL，表示要将待发数据转移到发送控制数据区，适合将不连续地址的数据添加到发送控制数据区;
              如果pBuf为NULL，表示只修改发送控制数据区剩余空间首指针，适合外部函数直接使用默认发送缓冲区；
    */
    uBit32 CMU_AddToSendCtrlData(uBit8* pBuf, uBit32 ulLen);
    
    /*
    函数名称：BooLean CMU_CheckSendBufFreeSize(uBit32 ulLen)

    功    能：检查默认发送缓冲区剩余空间是否足够

    参    数：ulLen---检查长度

    返 回 值：0--正确  非0--错误码

    注意事项：
    */
    BooLean CMU_CheckSendBufFreeSize(uBit32 ulLen);
    
    /*
    函数名称：uBit8* CMU_GetSendBufAddr()

    功    能：获取发送缓冲区剩余空间首地址

    参    数：ulLen---检查长度

    返 回 值：NULL--空间不够
              其他--剩余空间首地址
    注意事项：
    */
    uBit8* CMU_GetSendBufAddr();
    
    /*
    函数名称：uBit32 CMU_MainSendProc(void);

    功    能：获取发送缓冲区剩余空间首地址

    参    数：ulLen---检查长度

    返 回 值：NULL--空间不够
              其他--剩余空间首地址
    注意事项：
    */
    uBit32 CMU_MainSendProc(void);

#ifdef __cplusplus
}
#endif

#endif