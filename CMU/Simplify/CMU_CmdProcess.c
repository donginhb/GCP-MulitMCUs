/**
  ******************************************************************************
  * @file    CMU_CmdProcess.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.12.14
  * @brief   CMU Cmd Process
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "AbstractionLayer.h"
#include "CMU_CmdProcess.h"
#include "CMU_DataStructDef.h"
#include "CMU_ErrorMana.h"

#include "DataType/DataType.h"
#include "Version/Version.h"

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/
#define CMU_SNED_BUFF_LEN       (128)
static uBit8 m_uSendBuff[CMU_SNED_BUFF_LEN] = {0};
static COM_DATA_PACK   m_sRecvPackData = {0};     //接收数据缓冲区
static COM_DATA_PACK   m_sSendPackData = {{0}, 0, m_uSendBuff};     //发送数据缓冲区


/**
  * @brief  系统APP更新
  * @param  pBuff 数据缓冲区
  * @param  ulSize 缓冲区长度
  * @retval 0-成功 非0-失败
  */
uBit32 SYS_UpdateAPP(uBit8 *pBuff, uBit32 ulSize);


/**
  * @brief  系统副APP更新
  * @param  pBuff 数据缓冲区
  * @param  ulSize 缓冲区长度
  * @retval 0-成功 非0-失败
  */
uBit32 SYS_UpdateSubAPP(uBit8 *pBuff, uBit32 ulSize);


/**
  * @brief  延时复位
  * @param  ulDelayMs 延时的时间(单位:MS)
  * @retval None
  */
void CoreCtrl_ResetSystemDelay(uBit32 ulDelayMs);


/**
  * @brief  系统逻辑控制版本获取
  * @param  pVersion 版本结构指针
  * @retval 0-成功 非0-失败
  */
uBit32 SYS_GetSLCVersion(SOFTWARE_VERSION* pVersion);


/*****************************************************************************
 * 内部指令实现接口
 ****************************************************************************/

/**
  * @brief  SYS设置指令处理
  * @param  pRcvCtrlData 接收控制指令指针
  * @retval 0-成功 非0-失败
  */
static uBit32 Sys_SetNomalCmdProcess(COM_DATA_PACK *pRcvCtrlData)
{
    uBit32 ulRet = CMU_ERR_INVALID_CMD;
    
    switch(pRcvCtrlData->ulID.ulComDataID.ulCmdIndex)
    {
    case SYS_SETCMD_UPDATE_SLC:                        //SLC升级数据
        ulRet = SYS_UpdateSubAPP((uBit8*)pRcvCtrlData->pDataBuf, pRcvCtrlData->ulDataLen);
        
        if (ulRet == CMU_ERR_SUCCESS)
        {
            ulRet = CMU_ERR_NO_ACK;
        }
        
        break;
    case SYS_SETCMD_UPDATE_IPO:                        //IPO升级数据
        ulRet = SYS_UpdateAPP((uBit8*)pRcvCtrlData->pDataBuf, pRcvCtrlData->ulDataLen);
        
        if (ulRet == CMU_ERR_SUCCESS)
        {
            ulRet = CMU_ERR_NO_ACK;
        }
        
        break;
    case SYS_SETCMD_SYS_RESET:
        {
            //设置回发数据
            m_sSendPackData.ulID.ulFrameID = pRcvCtrlData->ulID.ulFrameID;
            m_sSendPackData.ulDataLen = 0;
            
            //执行指令
            CoreCtrl_ResetSystemDelay(500);
            ulRet = CMU_ERR_SUCCESS;
            break;
        }
    case SYS_SETCMD_SET_SLC_FLAG:
        
        ulRet = CMU_ERR_SUCCESS;
        break;
        
    default:break;
    }
    
    return ulRet;
}


/**
  * @brief  SYS获取指令处理
  * @param  pRcvCtrlData 接收控制指令指针
  * @retval 0-成功 非0-失败
  */
static uBit32 Sys_GetNomalCmdProcess(COM_DATA_PACK *pRcvCtrlData)
{
    uBit32 ulRet = CMU_ERR_INVALID_CMD;
    
    switch(pRcvCtrlData->ulID.ulComDataID.ulCmdIndex)
    {
    case SYS_GETCMD_SLC_VER:                        //获取系统版本
        {
            //设置回发数据
            m_sSendPackData.ulID.ulFrameID = pRcvCtrlData->ulID.ulFrameID;
            m_sSendPackData.ulDataLen = sizeof(SOFTWARE_VERSION);
            m_sSendPackData.ulID.ulComDataID.ulTransmitFrame = TRANSMIT_SELF_FRAME;
            
            ulRet = SYS_GetSLCVersion((SOFTWARE_VERSION *)m_sSendPackData.pDataBuf);
            
            break;
        }
    default:break;
    }
    
    return ulRet;
}


/*****************************************************************************
 * CMU 指令解析相关控制接口
 ****************************************************************************/

/**
  * @brief  指令处理
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 CMU_CmdProcess(COM_DATA_PACK *pRcvCtrlData)
{
    uBit32 ulRet = 0;
    
    switch (pRcvCtrlData->ulID.ulComDataID.ulDataType)
    {
    case DATA_TYPE_SYS:
        if (pRcvCtrlData->ulID.ulComDataID.ulOperType == DATA_OPRE_TYPE_SET) //设置指令
        {
            ulRet = Sys_SetNomalCmdProcess(pRcvCtrlData);
        }
        else    //查询指令
        {
            ulRet = Sys_GetNomalCmdProcess(pRcvCtrlData);
        }
        
        break;
        
    default: break;
    }
    
    return ulRet;
}


/**
  * @brief  生成错误回应包
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 CMU_SendResponsePack(uBit32 ulID, uBit32 ulErrCode)
{
    COM_DATA_ID ulComDataID = {0};

    ulComDataID.ulFrameID = ulID;
    ulComDataID.ulComDataID.ulTransmitFrame = TRANSMIT_SELF_FRAME;
    ulComDataID.ulComDataID.ulReceiveObj = SLC_RECEIVE;

    //生成错误码
    if (ulErrCode)
    {
        ulComDataID.ulComDataID.ulErrFlag = 1;
        
        if(ulErrCode < 0xFF)    //内部产生的错误，转换错误码
        {
            ulErrCode = CMU_CreateError(ulErrCode, ulID);
        }
    }

    //发送数据包
    uBit32 ulRet = COM_AL_SendPack(ulComDataID.ulFrameID, (uBit8*)&ulErrCode, sizeof(uBit32));
    
    return ulRet;
}


/**
  * @brief  重置接收缓冲区
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 COM_PL_ResetRcvCtrlData(uBit32 ulRcvId, uBit8 *pRcvBuf, uBit32 ulLen)
{
    uBit32 ulRet = 0;
    
    return ulRet;
}


/**
  * @brief  接收处理
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 CMU_MainRcvProc(void)
{
    //查询数据包接收情况
    uBit32 ulRet = COM_AL_GetPack(&m_sRecvPackData);
    
    //假如接收到数据,则进行数据解析
    if (ulRet==CMU_ERR_SUCCESS) //成功收到数据包
    {
        ulRet = CMU_CmdProcess(&m_sRecvPackData);
        
        if (ulRet == CMU_ERR_SUCCESS)
        {
            COM_AL_SendPack(m_sSendPackData.ulID.ulFrameID, m_sSendPackData.pDataBuf, m_sSendPackData.ulDataLen);
        }
        //接收到数据包，但处理数据包失败，则直接发送错误回包
        else if (ulRet != CMU_ERR_NO_ACK)
        {
            ulRet = CMU_SendResponsePack(m_sRecvPackData.ulID.ulFrameID, ulRet);
        }
    }
    
    return ulRet;
}


/**
  * @brief  CMU通信管理
  * @param  None
  * @retval None
  */
void CMU_MainProc(void)
{
    uBit32 ulRet = 0;
    
    //接收数据包
    COM_AL_RcvHandle(); 
    
    //获取接收解析情况
    ulRet = CMU_MainRcvProc();
    
    if (ulRet)
    {
        
    }
    
}
