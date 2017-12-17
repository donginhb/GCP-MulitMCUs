/**
  ******************************************************************************
  * @file    CMU_EnetInterface.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.11.26
  * @brief   CMU ENET Interface
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "CMU_EnetInterface.h"
#include "DataType/DataType.h"
#include "CMU_Unpack.h"

#include <string.h>
      

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/
#define ENET_MAX_SNED_LEN       (128)
#define ENET_MAX_RECV_LEN       (128)

#define FRAME_HAND              ("HCZN")            //帧头
#define FRAME_HAND_SIZE         (4)                 //帧头长度
#define FRAME_LEN_SIZE          (2)                 //帧长长度
#define FRAME_ID_SIZE           (4)                 //ID长度
#define FRAME_CHECK_SIZE        (2)                 //校验和长度
#define FRAME_MIN_SIZE          (FRAME_HAND_SIZE + FRAME_LEN_SIZE + FRAME_ID_SIZE + FRAME_CHECK_SIZE)   //帧最小长度

#define FRAME_HAND_INDEX        (0)                 //帧头索引
#define FRAME_LAN_INDEX         (4)                 //帧长索引
#define FRAME_ID_INDEX          (6)                 //帧ID索引
#define FRAME_PACK_INDEX        (10)                //帧数据区索引

typedef struct 
{
    uBit32 ulID;
    uBit32 ulDataLen;
    uBit8  uDataBuff[ENET_MAX_SNED_LEN];
}ENET_DATA_PACK;

static ENET_DATA_PACK m_EnetRecvPack = {0};         //接收的数据包缓存(已解包)
static uBit8 m_uSendBuff[ENET_MAX_SNED_LEN] = {0};  //发送缓冲区
static uBit8 m_uRecvBuff[ENET_MAX_RECV_LEN] = {0};  //接收缓冲区
static uBit8 m_uRecvPackValidFlag = 0;              //接收数据包有效标志

static CMU_ENET_INTERFACE m_EnetInterface = {0};    //串口控制接口
static bool bEnetInterfaceValidFlags = false;       //串口接口有效标志


//解包接收适配
static uBit32 CMU_ENET_UnpackRecvAdapt(uBit8 *pRBuff, uBit32 ulSize)
{
    if (bEnetInterfaceValidFlags)
    {
        return m_EnetInterface.pf_ENET_RecvBuff(pRBuff, ulSize);
    }
    
    return 0;
}


/*****************************************************************************
 * CMU ENET相关控制接口
 ****************************************************************************/

/**
  * @brief  CMU ENET 接口设置
  * @param  pEnetInterface 以太网接口指针
  * @retval 0-成功 非0-失败
  */
uBit32 CMU_ENET_SetInterface(CMU_ENET_INTERFACE *pEnetInterface)
{
    if (pEnetInterface != NULL)
    {
        bEnetInterfaceValidFlags = true;
        m_EnetInterface = *pEnetInterface;
        
        CMU_SetUnpackRecvInterface(CMU_ENET_UnpackRecvAdapt);
        return 0;
    }
    
    CMU_SetUnpackRecvInterface(CMU_ENET_UnpackRecvAdapt);
    return 1;
}


/**
  * @brief  CMU ENEI 接口有效性检测
  * @param  None
  * @retval true-有效 false-无效
  */
bool CMU_ENET_CheckInterfaceValid(void)
{
    
    return bEnetInterfaceValidFlags;
}


/**
  * @brief  端口打开
  * @param  None
  * @retval 0-成功  非0-失败
  */
uBit32 CMU_ENET_Open(void)
{
    m_EnetInterface.pf_ENET_Open();
    
    return 0;
}


/**
  * @brief  端口关闭
  * @param  None
  * @retval 0-成功  非0-失败
  */
uBit32 CMU_ENET_Close(void)
{
    m_EnetInterface.pf_ENET_Close();
    
    return 0;
}


/**
  * @brief  数据接收处理
  * @param  None
  * @retval None
  */
void CMU_ENET_RecvHandler(void)
{
    uBit32 ulPackLen = 0;
    
    //获取数据包
    if (CMU_UnPack(m_uRecvBuff, &ulPackLen) == 0)
    {
#if 0
        memcpy(&m_EnetRecvPack, m_uRecvBuff, ulPackLen);
#endif
        m_EnetRecvPack.ulID = *(uBit32 *)m_uRecvBuff;
        m_EnetRecvPack.ulDataLen = ulPackLen;
        
        memcpy(m_EnetRecvPack.uDataBuff, &m_uRecvBuff[4], ulPackLen);
        
        m_uRecvPackValidFlag = 1;
    }
    
}


/**
  * @brief  数据包获取
  * @param  pID(出参)
  * @param  pRcvBuf 要发送的数据缓冲区(出参)
  * @param  pRcvLen 数据缓冲区长度(出参)
  * @retval 0-成功  非0-失败
  */
uBit32 CMU_ENET_GetPack(uBit32 *pID, uBit8** pRcvBuf, uBit32* pRcvLen)
{
    if (m_uRecvPackValidFlag)
    {
        m_uRecvPackValidFlag = 0;
        
        *pID = m_EnetRecvPack.ulID;
        *pRcvLen = m_EnetRecvPack.ulDataLen;
        *pRcvBuf = m_EnetRecvPack.uDataBuff;
        
        return 0;
    }
    
    return 1;
}


/**
  * @brief  数据包发送
  * @param  ulID ID
  * @param  pDataBuf 要发送的数据缓冲区
  * @param  ulDataBufLen 数据缓冲区长度
  * @retval 0-成功  非0-失败
  */
uBit32 CMU_ENET_SendPack(uBit32 ulID, uBit8* pDataBuff, uBit32 ulBuffLen)
{
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
    m_EnetInterface.pf_ENET_SendBuff((uBit8 *)&m_uSendBuff, uFrameLen);
    
    return 0;
}


/**
  * @brief  单次能发送的最大数据长度获取
  * @param  None
  * @retval 数据长度
  */
uBit32 CMU_ENET_GetMaxSendLen(void)
{
    
    return ENET_MAX_SNED_LEN;
}

