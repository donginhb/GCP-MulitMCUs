#ifndef _ABSTRACTION_LAYER_H_
#define _ABSTRACTION_LAYER_H_

#include "..\\UIN_GlobalDataDef.h"
#include "..\\..\\DataType\\DataType.h"
#include "..\\ProtocolLayer\\ComDataStructDef.h"

typedef struct _COM_DATA_PACK
{
    COM_DATA_ID ulID;
    uBit32 ulDataLen;
    uBit8 *pDataBuf;
}COM_DATA_PACK;

/*
函 数 名：uBit32 COM_AL_Open(uBit32 ulComType)
函数说明：打开通信方式
参数说明：1--CAN通信 2--以太网通信 其他--未设置
返 回 值：0成功 非0-失败
注意事项：初始化通信模块时调用
*/
extern uBit32 COM_AL_Open(uBit32 ulComType);

/*----------------------------------
函 数 名：void COM_AL_Close()
函数说明：关闭通信
参数说明：无
返 回 值：无
注意事项：
*/
extern void COM_AL_Close(void);


/*----------------------------------
函 数 名：BooLean COM_AL_IsComTypeValid(uBit32 ulComType)
函数说明：检查通信方式相应的接口是否已经挂接
参数说明：无
返 回 值：无
注意事项：需要时调用
*/
extern BooLean COM_AL_IsComTypeValid(uBit32 ulComType);


/*----------------------------------
函 数 名：uBit32 COM_AL_GetPack(COM_DATA_PACK* pDataPack)
函数说明：获取数据包
参数说明：pDataPack--数据包地址
返 回 值：0成功 非0-失败
注意事项：
*/
extern    uBit32 COM_AL_GetPack(COM_DATA_PACK* pDataPack);

/*----------------------------------
函 数 名：Bit32 COM_AL_SendPack(unsigned long ulID, char* pDataBuf, Bit32 iDataBufLen)
函数说明：发送数据包
参数说明：ulID -- 数据包ID
          pDataBuf -- 数据区
          iDataBufLen -- 数据区长度
返 回 值：0成功 非0-失败
注意事项：
*/
extern uBit32 COM_AL_SendPack(uBit32 ulID, uBit8* pDataBuf, uBit32 ulDataBufLen);

/*----------------------------------
函 数 名：Bit32 COM_AL_GetMaxSendLen()
函数说明：抽象层获取单次最大能发送的数据长度
参数说明：无
返 回 值：单包最大允许发送长度
注意事项：
*/
extern uBit32 COM_AL_GetMaxSendLen(void);

/*
功      能： 底层接收数据
参      数： 无 
返  回  值： 无
调 用 位置： 在循环中调用
注意 事 项：   
所在文件名：    
*/
extern void COM_AL_RcvHandle(void);


/*----------------------------------
函 数 名：uBit32 COM_AL_GetMaxSendPacks()
函数说明：获取抽象层可连续发送的数据包数量
参数说明：无
返 回 值：连续可发送的数据包个数
注意事项：最大连续可发送的数据包个数，对于CAN来说可以是多包，但是考虑到效率，必须指定合适值，对于以太网，必须每次
          只能发一包，否则上载大数据时如上传G代码，用户缓冲区数据等，会有丢包，lwip发送成功但实际网络上并没有收到数据包
*/
extern uBit32 COM_AL_GetMaxSendPacks(void);


extern void* CMU_Malloc(uBit32 ulSize);
extern uBit32 COM_AL_GetComType(void);
#endif