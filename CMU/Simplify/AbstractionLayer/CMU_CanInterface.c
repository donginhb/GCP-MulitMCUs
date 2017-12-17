/**
  ******************************************************************************
  * @file    CMU_CanInterface.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.11.26
  * @brief   CMU CAN Interface
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "CMU_CanInterface.h"
#include "DataType/DataType.h"

#include <string.h>
      

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/
#define CAN_MAX_SNED_LEN       (8)
#define CAN_MAX_RECV_LEN       (128)


typedef struct 
{
    uBit32 ulID;
    uBit32 ulDataLen;
    uBit8  uDataBuff[CAN_MAX_SNED_LEN];
}CAN_DATA_PACK;

#if 0
static CAN_DATA_PACK m_CanRecvPack = {0};           //接收缓冲区
static uBit8 m_uSendBuff[CAN_MAX_SNED_LEN] = {0};   //发送缓冲区
static uBit8 m_uRecvPackValidFlag = 0;              //接收数据包有效标志
#endif

static CMU_CAN_INTERFACE m_CanInterface = {0};      //CAN控制接口
static bool bCanInterfaceValidFlags = false;       //CAN接口有效标志
static uBit8 m_uCanNode = 0;


/*****************************************************************************
 * CMU CAN相关控制接口
 ****************************************************************************/

/**
  * @brief  CMU UART 接口设置
  * @param  uCanNode CAN节点
  * @param  pCanInterface CAN接口指针
  * @retval 0-成功 非0-失败
  */
uBit32 CMU_CAN_SetInterface(uBit8 uCanNode, CMU_CAN_INTERFACE *pCanInterface)
{
    if (pCanInterface != NULL)
    {
        m_uCanNode = uCanNode;
        bCanInterfaceValidFlags = true;
        m_CanInterface = *pCanInterface;
        
        return 0;
    }
    
    return 1;
}


/**
  * @brief  CMU ENEI 接口有效性检测
  * @param  None
  * @retval true-有效 false-无效
  */
bool CMU_CAN_CheckInterfaceValid(void)
{
    
    return bCanInterfaceValidFlags;
}


/**
  * @brief  端口打开
  * @param  None
  * @retval 0-成功  非0-失败
  */
uBit32 CMU_CAN_Open(void)
{
    m_CanInterface.pf_CAN_Open(m_uCanNode);
    
    return 0;
}


/**
  * @brief  端口关闭
  * @param  None
  * @retval 0-成功  非0-失败
  */
uBit32 CMU_CAN_Close(void)
{
    m_CanInterface.pf_CAN_Open(m_uCanNode);
    
    return 0;
}


/**
  * @brief  数据接收处理
  * @param  None
  * @retval None
  */
void CMU_CAN_RecvHandler(void)
{
#if 0
    uBit32 ulPackLen = 0;
    uBit8 uRecvBuff[CAN_MAX_RECV_LEN] = {0};
    
    //获取数据包
    if (CMU_UnPack(uRecvBuff, &ulPackLen) == 0)
    {
#if 0
        memcpy(&m_CanRecvPack, uRecvBuff, ulPackLen);
#endif
        m_CanRecvPack.ulID = *(uBit32 *)uRecvBuff;
        m_CanRecvPack.ulDataLen = ulPackLen;
        
        memcpy(m_CanRecvPack.uDataBuff, &uRecvBuff[4], ulPackLen);
        
        m_uRecvPackValidFlag = 1;
    }
#endif
    
}


/**
  * @brief  数据包获取
  * @param  pID(出参)
  * @param  pRcvBuf 要发送的数据缓冲区(出参)
  * @param  pRcvLen 数据缓冲区长度(出参)
  * @retval 0-成功  非0-失败
  */
uBit32 CMU_CAN_GetPack(uBit32 *pID, uBit8** pRcvBuf, uBit32* pRcvLen)
{
#if 0
    if (m_uRecvPackValidFlag)
    {
        m_uRecvPackValidFlag = 0;
        
        *pID = m_CanRecvPack.ulID;
        *pRcvLen = m_CanRecvPack.ulDataLen;
        *pRcvBuf = m_CanRecvPack.uDataBuff;
        
        return 0;
    }
#endif
    
    return 1;
}


/**
  * @brief  数据包发送
  * @param  ulID ID
  * @param  pDataBuf 要发送的数据缓冲区
  * @param  ulDataBufLen 数据缓冲区长度
  * @retval 0-成功  非0-失败
  */
uBit32 CMU_CAN_SendPack(uBit32 ulID, uBit8* pDataBuff, uBit32 ulBuffLen)
{
#if 0
    uBit16 uFrameLen = 0;
    uBit16 nFrameCheckValue = 0;
    
    //清空发送缓冲区
    memset(&m_uSendBuff, 0, sizeof(m_uSendBuff));
    
    //计算帧长
    uFrameLen = FRAME_MIN_SIZE + ulBuffLen;
    
    //打包数据
    memcpy(&m_uSendBuff[FRAME_HAND_INDEX], FRAME_HAND, FRAME_HAND_SIZE);    //帧头
    memcpy(&m_uSendBuff[FRAME_LAN_INDEX], &uFrameLen, FRAME_LEN_SIZE);       //帧长
    memcpy(&m_uSendBuff[FRAME_ID_INDEX], &ulID, FRAME_ID_SIZE);              //帧ID
    memcpy(&m_uSendBuff[FRAME_PACK_INDEX], pDataBuff, ulBuffLen);           //帧数据区
    
    //计算校验和
    for(int i = 0; i < (uFrameLen-2); i++)
    {
        nFrameCheckValue += m_uSendBuff[i];
    }
    
    memcpy(&m_uSendBuff[uFrameLen-2], (uBit8 *)&nFrameCheckValue, FRAME_CHECK_SIZE);
    
    //发送数据
    m_CanInterface.pf_CAN_SendBuff(m_uCanNode, (uBit8 *)&m_uSendBuff, uFrameLen);
    
#endif
    
    return 0;
}


/**
  * @brief  单次能发送的最大数据长度获取
  * @param  None
  * @retval 数据长度
  */
uBit32 CMU_CAN_GetMaxSendLen(void)
{
    
    return CAN_MAX_SNED_LEN;
}

