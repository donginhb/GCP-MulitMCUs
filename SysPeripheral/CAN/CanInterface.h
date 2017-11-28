/*    
Copyright (c) 2013, 东莞华中科技大学制造工程研究院 All rights reserved.
文件名称：CanRS.h
摘    要：CAN模块接口函数头文件
运行环境：IAR6.3
当前版本：1.0 
完成日期：2013年8月19日
*/
#ifndef _CAN_INTERFACE_H_
#define _CAN_INTERFACE_H_

#include "CanDataStructDef.h"


#ifdef __cplusplus
extern "C"
{
#endif
///////////////////////////////////////////////////////////////////  
    /*
    功      能：    打开CAN节点通信
    参      数：    nCanNode：CAN结点,取值0或者1
                             ulBaudRate：波特率
                             ulFilterID:        滤波ID
                             ulFilterMask:    滤波屏蔽码

    返  回  值：
                CAN_ERR_SUCCESS：打开CAN节点成功
                CAN_ERR_CONNECT：打开CAN节点失败
    注意 事 项：    无
    */
    uBit32 CAN_Open(uBit8 nCanNode, uBit8 ulBaudRate, uBit32 ulFilterID, uBit32  ulFilterMask);

    /*
    功      能：    关闭CAN节点通信
    参      数：    nCanNode：CAN结点,取值0或者1

    返  回  值：
                            CAN_ERR_SUCCESS：关闭CAN节点成功
                            CAN_ERR_CONNECT：关闭开CAN节点失败
    注意 事 项：    无
    */
    uBit32 CAN_Close(uBit8 nCanNode);

    /*
    功      能：    底层接收数据包
    参      数：    nCanNode：CAN结点,取值0或者1

    返  回  值：
                    CAN_ERR_SUCCESS：关闭CAN节点成功
                    CAN_ERR_CONNECT：关闭开CAN节点失败
    注意 事 项：    尽可能多的调用，以免丢失数据包
    */
    void CAN_RcvHandle(uBit8 nCanNode);

    /*
    功      能：    获取CAN节点最大允许发送的单包数据长度
    参      数：    nCanNode：CAN结点,取值0或者1

    返  回  值：        最大单包数据长度
    注意 事 项：   
    */
    uBit32 CAN_GetMaxSendLen(void);


    /*
    功      能：   即时发送CAN数据包
    参      数：   nCanNode        ：    CAN结点,取值0或者1
                            ulID                :    发送数据ID
                            pData                :    发送缓冲区
                            ulDataBufLen    ：待发送数据长度
    返  回  值：
                    CAN_ERR_SUCCESS    ：发送成功
                    CAN_ERR_SEND        ：    发送失败
                    CAN_ERR_CONNECT    ：连接错误
    注意 事 项：    无
    */
    uBit32 CAN_SendPack(uBit8 nCanNode, uBit32 ulID , uBit8* ucPData , uBit32 ulDataBufLen);

    /*
    功      能：   读取接收缓冲区接收到的数据包
    参      数：   nCanNode        ：    CAN结点,取值0或者1
                            pID                :    保存接收数据ID
                            pRcvLen            :    保存接收数据长度
                            pRcvBuf            ：保存接收数据地址
    返  回  值：
                    CAN_ERR_SUCCESS    ：接收成功
                    CAN_ERR_SEND        ：接收失败
                    CAN_ERR_CONNECT    ：连接错误
    注意 事 项：    无
    */
    uBit32 CAN_GetPack(uBit8 nCanNode, uBit32 *pID, uBit32* pRcvLen,  uBit8** pRcvBuf);
    
    /*
    功      能：   读取接收缓冲区接收到的数据包
    参      数：   nCanNode        ：    CAN结点,取值0或者1
                            pRcvFrame        :    保存到的数据帧
    返  回  值：
                            CAN_ERR_SUCCESS    ：接收成功
                            CAN_ERR_SEND        ：接收失败
                            CAN_ERR_CONNECT    ：连接错误
    注意 事 项：    无
    */
    uBit32 CAN_GetFrame(uBit8 nCanNode, CanFrame *pRcvFrame);

    /*
    功      能：   检查最后一帧数据是否发送完成，连续发送时调用
    参      数：   nCanNode        ：    CAN结点,取值0或者1
    返  回  值：        ture--发送完成 false--发送未完成 
    注意 事 项：      
    
    BooLean CAN_IsLastFrameSent(uBit8 nCanNode);*/
//////////////////////////////////////////////////////////////////// 
  
    /*
    功      能：   导出CAN通信接口
    参      数：   pCanInterface        ：保存导出的CAN通信接口
    返  回  值：
    注意 事 项：    无    
    */
    void CAN_ExportInterface(CAN_INTERFACE *pCanInterface);


#ifdef __cplusplus
}
#endif

#endif

/*===========================================End===========================================*/