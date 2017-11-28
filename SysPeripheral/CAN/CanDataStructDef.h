#ifndef CAN_DATA_STRUCT_DEF_H_
#define CAN_DATA_STRUCT_DEF_H_

#include "..\\..\\DataType\\DataType.h"

#define CAN0_NODE_ID                (0)
#define CAN1_NODE_ID                (1)

//波特率表索引宏定义
#define CANBAUD_1M                  (0)
#define CANBAUD_500K                (1)
#define CANBAUD_250K                (2)
#define CANBAUD_125K                (3)
#define CANBAUD_100K                (4)
#define CANBAUD_50K                 (5)
#define CANBAUD_25K                 (6)
#define CANBAUD_20K                 (7)
#define CANBAUD_10K                 (8)
#define CANBAUD_5K                  (9)
#define CANBAUD_2K5                 (10)
                                    
#define ENABLE_CAN_NODE_NUM         (2)//使能CAN节点数目
#define _CAN_TX_BUFFER_FUN_         (1)//是否加入发送缓冲区
                                    
#define CAN_SEND_NO_FORCE           (0)
#define CAN_SEND_FORCE              (1)
#define CAN_DATA_MAX_LEN            (8)
                                    
#define CAN_ERR_SUCCESS             (0)//成功
#define CAN_ERR_CONNECT             (1)//表示通信连接出错(接收和发送抽象层硬件出错)
#define CAN_ERR_SEND                (2)//发送数据失败
#define CAN_ERR_EMPTY               (3)//未收到任何数据
                                    
                                    
#define CAN_CONTINUE_SEND_GAP       (10000) //2777个循环约为1us


//定义CAN报文结构体
typedef struct {

    uBit8 ucTtypeFormat;                                //0～3 帧类型及格式 
    uBit8 ucDLC;                                        //数据场长度
    uBit32 ulID;                                        //CAN报文ID
    uBit8 ucDatBuf[CAN_DATA_MAX_LEN];                   //报文数据场
}CanFrame;

typedef struct can_interface
{
///////////////////////////////////////////////////////////////////  
    /*
    功      能：    打开CAN节点通信
    参      数：    nCanNode：CAN结点,取值0或者1
                             uBaudRate：波特率
                             ulFilterID:        滤波ID
                             ulFilterMask:    滤波屏蔽码

    返  回  值：
                CAN_ERR_SUCCESS：打开CAN节点成功
                CAN_ERR_CONNECT：打开CAN节点失败
    注意 事 项：    无
    */
    uBit32 (*pf_CAN_Open)(uBit8 nCanNode, uBit8 uBaudRate, uBit32 ulFilterID, uBit32  ulFilterMask);

    /*
    功      能：    关闭CAN节点通信
    参      数：    nCanNode：CAN结点,取值0或者1

    返  回  值：
                            CAN_ERR_SUCCESS：关闭CAN节点成功
                            CAN_ERR_CONNECT：关闭开CAN节点失败
    注意 事 项：    无
    所在文件名：    CanRS.c
    */
    uBit32 (*pf_CAN_Close)(uBit8 nCanNode);

    /*
    功      能：    底层接收数据包
    参      数：    nCanNode：CAN结点,取值0或者1

    返  回  值：
                    CAN_ERR_SUCCESS：关闭CAN节点成功
                    CAN_ERR_CONNECT：关闭开CAN节点失败
    注意 事 项：    尽可能多的调用，以免丢失数据包
    所在文件名：    CanRS.c
    */
    void (*pf_CAN_RcvHandle)(uBit8 nCanNode);

    /*
    功      能：    获取CAN节点最大允许发送的单包数据长度
    参      数：    nCanNode：CAN结点,取值0或者1

    返  回  值：        最大单包数据长度
    注意 事 项：   
    所在文件名：    CanRS.c
    */
    uBit32 (*pf_CAN_GetMaxSendLen)(void);


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
    所在文件名：    CanRS.c
    */
    uBit32 (*pf_CAN_SendPack)(uBit8 nCanNode, uBit32 ulID , uBit8* ucPData , uBit32 ulDataBufLen);

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
    所在文件名：    CanRS.c
    */
    uBit32 (*pf_CAN_GetPack)(uBit8 nCanNode, uBit32 *pID, uBit32* pRcvLen,  uBit8** pRcvBuf);
    
    /*
    功      能：   读取接收缓冲区接收到的数据包
    参      数：   nCanNode        ：    CAN结点,取值0或者1
                            pRcvFrame        :    保存到的数据帧
    返  回  值：
                            CAN_ERR_SUCCESS    ：接收成功
                            CAN_ERR_SEND        ：接收失败
                            CAN_ERR_CONNECT    ：连接错误
    注意 事 项：    无
    所在文件名：    CanRS.c*/
    
    uBit32 (*pf_CAN_GetFrame)(uBit8 nCanNode, CanFrame *pRcvFrame);

}CAN_INTERFACE;

#endif