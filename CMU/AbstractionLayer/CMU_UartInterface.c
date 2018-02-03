/**
  ******************************************************************************
  * @file    CMU_UartInterface.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.11.26
  * @brief   CMU UART Interface
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "CMU_UartInterface.h"
#include "DataType/DataType.h"
#include "CMU_Unpack.h"

#include <string.h>
      

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/
#define UART_MAX_SNED_LEN       (64)
#define UART_MAX_RECV_LEN       (256)

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
    uBit8  uDataBuff[UART_MAX_RECV_LEN];
    uBit32 ulDataLen;
}UART_DATA_PACK;

static UART_DATA_PACK m_UartRecvPack = {0};         //接收的数据包缓存(已解包)
static uBit8 m_uSendBuff[UART_MAX_SNED_LEN] = {0};  //发送缓冲区
//static uBit8 m_uRecvBuff[UART_MAX_RECV_LEN] = {0};  //接收缓冲区
static uBit8 m_uRecvPackValidFlag = 0;              //接收数据包有效标志

static CMU_UART_INTERFACE m_UartInterface = {0};    //串口控制接口
static bool bUartInterfaceValidFlags = false;       //串口接口有效标志
static uBit8 m_uUartNode = 0;

//解包接收适配
static uBit32 CMU_UART_UnpackRecvAdapt(uBit8 *pRBuff, uBit32 ulSize)
{
    if (bUartInterfaceValidFlags)
    {
        return m_UartInterface.pf_UART_RecvBuff(m_uUartNode, pRBuff, ulSize);
    }
    
    return 0;
}


/*****************************************************************************
 * CMU UART相关控制接口
 ****************************************************************************/

/**
  * @brief  CMU UART 接口设置
  * @param  uUartNode 串口节点
  * @param  pUartInterface 串口接口指针
  * @retval 0-成功 非0-失败
  */
uBit32 CMU_UART_SetInterface(uBit8 uUartNode, CMU_UART_INTERFACE *pUartInterface)
{
    if (pUartInterface != NULL)
    {
        m_uUartNode = uUartNode;
        bUartInterfaceValidFlags = true;
        m_UartInterface = *pUartInterface;
        
        CMU_SetUnpackRecvInterface(CMU_UART_UnpackRecvAdapt);
        return 0;
    }
    
    CMU_SetUnpackRecvInterface(CMU_UART_UnpackRecvAdapt);
    return 1;
}


/**
  * @brief  CMU UART 接口有效性检测
  * @param  None
  * @retval true-有效 false-无效
  */
bool CMU_UART_CheckInterfaceValid(void)
{
    
    return bUartInterfaceValidFlags;
}


/**
  * @brief  端口打开
  * @param  None
  * @retval 0-成功  非0-失败
  */
uBit32 CMU_UART_Open(void)
{
    m_UartInterface.pf_UART_Open(m_uUartNode);
    
    return 0;
}


/**
  * @brief  端口关闭
  * @param  None
  * @retval 0-成功  非0-失败
  */
uBit32 CMU_UART_Close(void)
{
    m_UartInterface.pf_UART_Close(m_uUartNode);
    
    return 0;
}


/**
  * @brief  数据接收处理
  * @param  None
  * @retval None
  */
void CMU_UART_RecvHandler(void)
{
    //获取数据包
    if (CMU_UnPack((uBit8 *)&m_UartRecvPack.ulID, &m_UartRecvPack.ulDataLen) == 0)
    {
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
uBit32 CMU_UART_GetPack(uBit32 *pID, uBit8** pRcvBuf, uBit32* pRcvLen)
{
    if (m_uRecvPackValidFlag)
    {
        m_uRecvPackValidFlag = 0;
        
        *pID = m_UartRecvPack.ulID;
        *pRcvLen = m_UartRecvPack.ulDataLen;
        *pRcvBuf = m_UartRecvPack.uDataBuff;
        
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
uBit32 CMU_UART_SendPack(uBit32 ulID, uBit8* pDataBuff, uBit32 ulBuffLen)
{
    uBit16 uFrameLen = 0;
    uBit16 nFrameCheckValue = 0;
    
    //清空发送缓冲区
    memset(&m_uSendBuff, 0, sizeof(m_uSendBuff));
    
    //计算帧长
    uFrameLen = FRAME_MIN_SIZE + ulBuffLen;
    
    //校验帧长
    if (uFrameLen > sizeof(m_uSendBuff))
    {
        return 1;
    }
    
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
    m_UartInterface.pf_UART_SendBuff(m_uUartNode, (uBit8 *)&m_uSendBuff, uFrameLen);
    
    return 0;
}


/**
  * @brief  单次能发送的最大数据长度获取
  * @param  uUartNode 串口节点号
  * @retval 数据长度
  */
uBit32 CMU_UART_GetMaxSendLen(void)
{
    
    return UART_MAX_SNED_LEN;
}

