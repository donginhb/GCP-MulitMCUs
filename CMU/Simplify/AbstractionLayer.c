/**
  ******************************************************************************
  * @file    AbstractionLayer.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.11.29
  * @brief   CMU Abstraction Layer
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include <string.h>
#include "AbstractionLayer.h"
#include "CMU_CanInterface.h"
//#include "CMU_EnetInterface.h"
#include "CMU_UartInterface.h"
#include "CMU_ErrorMana.h"


/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/
#define COM_CAN_MAX_PACK_PER_TIME           (8)     //CAN 单次连续可传输的数据包数量
#define COM_ENET_MAX_PACK_PER_TIME          (16)    //ENET单次连续可传输的数据包数量
#define COM_UART_MAX_PACK_PER_TIME          (64)    //UART单次连续可传输的数据包数量

#define COM_CAN_VALID_MASK                  (0x01)  //CAN  有效掩码
#define COM_ENET_VALID_MASK                 (0x02)  //ENET 有效掩码
#define COM_UART_VALID_MASK                 (0x04)  //UART 有效掩码

static uBit8   m_ucComInvalidFlag = 0;              //通信模块可用标志,在相应的通信接口挂接成功后置位
static uBit32  m_ulComType = COM_TYPE_NULL;         //连接方式
static BooLean m_bALInitFlag = false;               //数据初始化标志


/*****************************************************************************
 * CMU抽象层相关控制接口
 ****************************************************************************/

/**
  * @brief  抽象数据初始化
  * @param  None
  * @retval None
  */
void COM_AL_Init(void)
{
    m_ulComType = COM_TYPE_NULL;
    m_bALInitFlag = true;
}


/**
  * @brief  通信端口打开
  * @param  ulComType 通信类型
  *   @arg COM_TYPE_NULL 无通信接口
  *   @arg COM_TYPE_CAN  CAN通信
  *   @arg COM_TYPE_ENET ENET通信
  *   @arg COM_TYPE_UART UART通信模式
  * @retval 当前通信类型
  */
uBit32 COM_AL_Open(uBit32 ulComType)
{
    uBit32 ulRet = CMU_ERR_SUCCESS;

    if (m_bALInitFlag==false)
    {
        COM_AL_Init();
    }

    //通信类型校验
    if ((ulComType == COM_TYPE_NULL) || (ulComType > COM_TYPE_MAX))
    {
        return CMU_ERR_CONNECT;
    }

    //假如通信接口已经打开
    if (ulComType == m_ulComType)
    {
        return CMU_ERR_SUCCESS;
    }

    //校验接口是否挂接
    switch(m_ulComType)
    {
    case COM_TYPE_CAN:
        if (!CMU_CAN_CheckInterfaceValid())
        {
            return CMU_ERR_CONNECT;
        }
        break;
#if 0
    case COM_TYPE_ENET:
        if (!CMU_ENET_CheckInterfaceValid())
        {
            return CMU_ERR_CONNECT;
        }
        break;
#endif
    case COM_TYPE_UART:
        if (!CMU_UART_CheckInterfaceValid())
        {
            return CMU_ERR_CONNECT;
        }
        break;
    default:break;
    }

    //关闭已经建立的连接
    switch(m_ulComType)
    {
    case COM_TYPE_CAN:
        CMU_CAN_Close();
        break;
#if 0
    case COM_TYPE_ENET:
        CMU_ENET_Close();
        break;
#endif
    case COM_TYPE_UART:
        CMU_UART_Close();
        break;
    default:break;
    }
    
    //更新最新的接口
    m_ulComType = ulComType;

    //打开新的通信连接
    switch(m_ulComType)
    {
    case COM_TYPE_CAN:
        ulRet = CMU_CAN_Open();
        break;
#if 0
    case COM_TYPE_ENET:
        ulRet = CMU_ENET_Open();
        break;
#endif
    case COM_TYPE_UART:
        ulRet = CMU_UART_Open();
        break;
    default:break;
    }
    
    if (ulRet!= CMU_ERR_SUCCESS)
    {
        ulComType = COM_TYPE_NULL;
    }

    return ulRet;
}


/**
  * @brief  当前通信类型获取
  * @param  None
  * @retval 当前通信类型
  */
uBit32 COM_AL_GetComType(void)
{
    
    return m_ulComType;
}


/**
  * @brief  通信接口挂接检测
  * @param  ulComType 接口
  * @retval true-已挂接 false-未挂接
  */
BooLean COM_AL_IsComTypeValid(uBit32 ulComType)
{
    if (m_ulComType==COM_TYPE_CAN && 
        (m_ucComInvalidFlag &= COM_CAN_VALID_MASK)==COM_CAN_VALID_MASK)
        return true;
#if 0
    if (m_ulComType==COM_TYPE_ENET && 
        (m_ucComInvalidFlag &= COM_ENET_VALID_MASK)==COM_ENET_VALID_MASK)
        return true;
#endif
    if (m_ulComType==COM_TYPE_UART && 
        (m_ucComInvalidFlag &= COM_UART_VALID_MASK)==COM_UART_VALID_MASK)
        return true;

    return false;
}


/**
  * @brief  通信关闭
  * @param  None
  * @retval None
  */
void COM_AL_Close(void)
{
    switch(m_ulComType)
    {
    case COM_TYPE_CAN:
        CMU_CAN_Close();
        break;
#if 0
    case COM_TYPE_ENET:
        CMU_ENET_Close();
        break;
#endif
    case COM_TYPE_UART:
        CMU_UART_Close();
        break;
    default:break;
    }
    
    COM_AL_Init();
}


/**
  * @brief  数据包获取
  * @param  pDataPack 数据包地址
  * @retval 0成功 非0-失败
  */
uBit32 COM_AL_GetPack(COM_DATA_PACK* pDataPack)
{
    switch(m_ulComType)
    {

    case COM_TYPE_CAN:
        return CMU_CAN_GetPack(&pDataPack->ulID.ulFrameID, &pDataPack->pDataBuf, &pDataPack->ulDataLen);
#if 0
    case COM_TYPE_ENET:
        return CMU_ENET_GetPack(&pDataPack->ulID.ulFrameID, &pDataPack->pDataBuf, &pDataPack->ulDataLen);
#endif
    case COM_TYPE_UART:
        return CMU_UART_GetPack(&pDataPack->ulID.ulFrameID, &pDataPack->pDataBuf, &pDataPack->ulDataLen);
    default:break;
    }

    return CMU_ERR_CONNECT;
}


/**
  * @brief  数据包发送
  * @param  ulID 数据包ID
  * @param  pDataBuf 数据缓冲区
  * @param  ulDataBufLen 数据区长度
  * @retval 0成功 非0-失败
  */
uBit32 COM_AL_SendPack(uBit32 ulID, uBit8* pDataBuf, uBit32 ulDataBufLen)
{
    switch(m_ulComType)
    {
    case COM_TYPE_CAN:
        return CMU_CAN_SendPack(ulID, pDataBuf, ulDataBufLen);
#if 0
    case COM_TYPE_ENET:
        return CMU_ENET_SendPack(ulID, pDataBuf, ulDataBufLen);
#endif
    case COM_TYPE_UART:
        return CMU_UART_SendPack(ulID, pDataBuf, ulDataBufLen);
    default:break;
    }

    return CMU_ERR_CONNECT;
}


/**
  * @brief  单次最大能发送的数据长度获取
  * @param  None
  * @retval 单包最大允许发送长度
  */
uBit32 COM_AL_GetMaxSendLen(void)
{
    switch(m_ulComType)
    {
    case COM_TYPE_CAN:
        return CMU_CAN_GetMaxSendLen();
#if 0
    case COM_TYPE_ENET:
        return CMU_ENET_GetMaxSendLen();
#endif
    case COM_TYPE_UART:
        return CMU_UART_GetMaxSendLen();
    default:break;
    }

    return 0;
}


/**
  * @brief  抽象层可连续发送的数据包数量获取
  * @param  None
  * @retval 连续可发送的数据包个数
  * @note   最大连续可发送的数据包个数,对于CAN来说可以是多包,但是考虑到效率,必须指定合适值,对于以太网,必须每次
  *         只能发一包,否则在上传数据时,会存在丢包的风险;
  */
uBit32 COM_AL_GetMaxSendPacks(void)
{
    switch(m_ulComType)
    {
    case COM_TYPE_CAN:
        return COM_CAN_MAX_PACK_PER_TIME;
#if 0
    case COM_TYPE_ENET:
        return COM_ENET_MAX_PACK_PER_TIME;
#endif
    case COM_TYPE_UART:
        return COM_UART_MAX_PACK_PER_TIME;
    default:break;
    }

    return 0;
}


/**
  * @brief  底层接收数据(在循环中调用)
  * @param  None
  * @retval None
  */
void COM_AL_RcvHandle(void)
{
    switch(m_ulComType)
    {

    case COM_TYPE_CAN:
        CMU_CAN_RecvHandler();
        break;
#if 0
    case COM_TYPE_ENET:
        CMU_ENET_RecvHandler();
        break;
#endif
    case COM_TYPE_UART:
        CMU_UART_RecvHandler();
        break;
    default:break;
    }
    
}
