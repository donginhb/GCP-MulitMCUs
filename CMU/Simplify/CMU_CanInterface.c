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
  * 对于要发送大数据的情况(例如升级数据包),上位机可以将其拆分多包的数据.格式如下:
  *
  * 首帧,
  *     数据区发送8个字节数据,前4个字节表示中间数据帧的总帧数,后4个字节预留; 
  * 中间数据帧,
  *     数据区存放被拆分的数据,考虑到防止数据包顺序颠倒的情况,ID之中的Bit[19:22]
  *     用于存放当前包的索引,范围是0~15,当超过15便归零重算.可以有效避免相邻数据包
  *     的错位.(若不假入此机制,若是数据包错位/丢包,单单靠校验和也难以排查)
  *
  * 校验帧,
  *     数据区发送8个字节数据,前4个字节表示中间数据帧的校验和,后4个字节预留;
  * 
  * 例如,上位机需要通过CAN发送0~15共16个字节的数据到本机,数据帧如下:
  * 首帧: 
  *     ID的Bit[23:24] = TRANSMIT_FIRST_FRAME, 
  *     数据区为32位的数据帧帧数(低位在前) 0x10, 0x00, 0x00, 0x00..
  *
  * 中间数据帧: 
  *     ID的Bit[23:24] = TRANSMIT_BEING_FRAME,
  *     Bit[19:22] = 当前包序号%16,
  *     数据区发送实际要发送的数据(第一帧为0~7, 第二帧为8~15)
  * 
  * 校验帧: 
  *     ID的Bit[23:24] = TRANSMIT_VERIFY_FRAME,
*     数据区为
  * 
  * 
  *  
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "CMU_CanInterface.h"
#include "CMU_DataStructDef.h"
#include "DataType/DataType.h"

#include <string.h>
      

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/
#define CAN_MAX_SNED_LEN        (8)             //最大发送长度
#define CAN_MAX_RECV_LEN        (512+128)       //最大接收长度

typedef struct 
{
    uBit32 ulID;
    uBit32 ulDataLen;
    uBit8  uDataBuff[CAN_MAX_RECV_LEN];
}CAN_DATA_PACK;

//CAN相关参数
static CAN_DATA_PACK m_CanRecvPack = {0};           //接收缓冲区
static uBit8 m_uRecvPackValidFlag = 0;              //接收数据包有效标志
static CMU_CAN_INTERFACE m_CanInterface = {0};      //CAN控制接口
static bool bCanInterfaceValidFlags = false;        //CAN接口有效标志
static uBit8 m_uCanNode = 0;                        //CAN节点

//数据包传输相关参数
static uBit32 m_ulCanPackNumber = 0;                //数据包数
static uBit32 m_ulCurCanPackIndex = 0;              //当前数据包数
static bool m_bPackTransferStart = false;           //传输开始标志

/*****************************************************************************
 * CMU CAN相关控制接口
 ****************************************************************************/

/**
  * @brief  CMU CAN 接口设置
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
  * @brief  CMU CAN 接口有效性检测
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
    COM_DATA_ID ComID = {0};
    uBit8 uRBuff[8] = {0};
    uBit32 ulLenght = 0;
    
    m_CanInterface.pf_CAN_RecvHandler(m_uCanNode);
    
    if (m_CanInterface.pf_CAN_RecvPack(m_uCanNode, &ComID.ulFrameID, 
                                       uRBuff,
                                       &ulLenght) == 0)
    {
        switch (ComID.ulCanDataID.ulTransmitFrame)
        {
        case TRANSMIT_FIRST_FRAME:  //连续数据帧的首帧数据
            m_ulCanPackNumber = *((uBit32 *)(&uRBuff[0]));  //获取总包数
            
            memset(&m_CanRecvPack, 0, sizeof(m_CanRecvPack));
            m_ulCurCanPackIndex = 0;       //复位当前包
            m_bPackTransferStart = true;   //设置传输开始标志
            break;
        case TRANSMIT_BEING_FRAME:  //连续数据帧的中间数据帧
            
            //判断当前包序号是否正确
            if ((m_ulCurCanPackIndex < m_ulCanPackNumber) && (m_bPackTransferStart) && (ComID.ulCanDataID.ulFrameIndex == (m_ulCurCanPackIndex%16)))
            {
                //将参数存储到对应的位置
                memcpy(&m_CanRecvPack.uDataBuff[m_ulCurCanPackIndex*8], uRBuff, ulLenght);
                m_ulCurCanPackIndex++;
                m_CanRecvPack.ulDataLen += ulLenght;

            }
            else    //传输出错(丢包/错包等)
            {
                //复位相关参数
                memset(&m_CanRecvPack, 0, sizeof(m_CanRecvPack));
                m_ulCanPackNumber = 0;
                m_ulCurCanPackIndex = 0;
                m_bPackTransferStart = false;
            }
            break;
        case TRANSMIT_VERIFY_FRAME: //连续数据帧的校验帧
            
            if (m_bPackTransferStart)
            {
                uBit32 ulTrueCheckSum = 0;  //实际校验和
                
                //计算校验值
                for (int i = 0; i < m_CanRecvPack.ulDataLen; i++)
                {
                    ulTrueCheckSum += m_CanRecvPack.uDataBuff[i];
                }
                
                //校验数据
                if (ulTrueCheckSum == *((uBit32 *)(&uRBuff[0])))
                {
                    ComID.ulComDataID.ulTransmitFrame = TRANSMIT_SELF_FRAME;    //改成独立帧
                    m_CanRecvPack.ulID = ComID.ulFrameID;
                    m_uRecvPackValidFlag = 1;
                }
                
                m_bPackTransferStart = false;   //清除传输开始标志
            }
            
            break;
        case TRANSMIT_SELF_FRAME:   //独立发送帧
            m_CanRecvPack.ulID = ComID.ulFrameID;
            m_CanRecvPack.ulDataLen = ulLenght;
            memcpy(m_CanRecvPack.uDataBuff, uRBuff, ulLenght);
            m_uRecvPackValidFlag = 1;
            break;
        default: break;
        }
        
    }
    
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
    if (m_uRecvPackValidFlag)
    {
        m_uRecvPackValidFlag = 0;
        
        *pID = m_CanRecvPack.ulID;
        *pRcvLen = m_CanRecvPack.ulDataLen;
        *pRcvBuf = m_CanRecvPack.uDataBuff;
        
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
uBit32 CMU_CAN_SendPack(uBit32 ulID, uBit8* pDataBuff, uBit32 ulBuffLen)
{
    if (ulBuffLen > CAN_MAX_SNED_LEN)
    {
        return 1;
    }
    
    m_CanInterface.pf_CAN_SendPack(m_uCanNode, ulID, pDataBuff, ulBuffLen);
    
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
