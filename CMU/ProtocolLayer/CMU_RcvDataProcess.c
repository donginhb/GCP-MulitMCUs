
#include "string.h"
#include "CMU_ExApi.h"
#include "CMU_ErrorMana.h"
#include "CMU_CmdProcess.h"
#include "CMU_SendDataProcess.h"
#include "CMU_RcvDataProcess.h"
#include "..\\CMU_ErrorCodeDef.h"


//#include "..\\..\\GPIO\\GPIOCtrl.h"
#include "..\\AbstractionLayer\\AbstractionLayer.h"


COM_RCV_CTRL_DATA            m_RcvCtrlData;            //接收控制数据
RCV_TIME_OUT_CTRL_DATA        m_sRcvTimeOutCtrlData;    //数据重发控制数据    
COM_DATA_PACK                m_sPackData;            //数据包缓存区
#ifdef WIN32_VER
uBit8                        m_pRcvBuf[ENET_DATA_MAX_LENGTH];        //    
#else
uBit8                        *m_pRcvBuf=NULL;        //    
#endif




//抽象层内部函数声明-------------------------------------
uBit32 COM_PL_ResetRcvCtrlData(uBit32 ulRcvId, uBit8 *pRcvBuf, uBit32 ulLen);//重置接收控制数据区，收到起始帧或独立帧时调用
uBit32 COM_PL_WaitForProcResult(uBit32 *ulErrCode);    //等待指令处理结果，调用外部指令接口后调用
uBit32 COM_PL_SavePackToRcvCtrlData(void);            //保存数据包到接收控制数据区，接收到数据包时调用
uBit32 COM_PL_PackDataProc(void);                    //数据包处理，接收到数据包时调用

/*
函数名称：uBit32 CMU_InitRecCtrlData(void)

功    能：初始化接收控制数据区

参    数：无

返 回 值：0--成功 非0--错误编码

注意事项：通信模块初始化时调用
*/  
uBit32 CMU_InitRecCtrlData(void)
{
    memset(&m_RcvCtrlData , 0 , sizeof(COM_RCV_CTRL_DATA));
#ifdef WIN32_VER
    memset(&m_pRcvBuf , 0 , ENET_DATA_MAX_LENGTH);
    memset(&m_sRcvTimeOutCtrlData , 0 , sizeof(RCV_TIME_OUT_CTRL_DATA));
#else
    if(m_pRcvBuf==NULL)
    {
        m_pRcvBuf = CMU_Malloc(COM_TRANSMIT_BUF_SIZE);
        memset(&m_sRcvTimeOutCtrlData , 0 , sizeof(RCV_TIME_OUT_CTRL_DATA));
    }

    if (m_pRcvBuf)
    {
        memset(m_pRcvBuf, 0, COM_TRANSMIT_BUF_SIZE);
    }else
    {
        return CMU_ERR_RS_MALLOC_FAIL;
    }
#endif
    return CMU_ERR_SUCCESS;
}

/*-------------------------------------------------------------
函 数 名：uBit32 COM_PL_ResetRcvCtrlData()
函数说明：重置接收缓冲区
参数说明：
            ulRcvId            -- 接收数据的ID
            pRcvBuf            -- 接收数据缓冲区
            ulLen            -- 缓冲区数据总长度
返 回 值：0--成功 1--错误编码
注意事项：
*/
uBit32 COM_PL_ResetRcvCtrlData(uBit32 ulRcvId, uBit8 *pRcvBuf, uBit32 ulLen)
{
    uBit32 ulRet;

    ulRet = CMU_InitRecCtrlData();

    if (ulRet)
        return ulRet;

    m_RcvCtrlData.ulRevID.ulFrameID = ulRcvId;
    m_RcvCtrlData.cReceiving = 1;            //置接收中标志
    
    if (pRcvBuf)
    {
        m_RcvCtrlData.pRevBuf = pRcvBuf;    //设置接收缓冲区
        m_RcvCtrlData.ulFreeLen = ulLen;    //接收缓冲区长度
    }
    else
        {
            m_RcvCtrlData.pRevBuf = m_pRcvBuf;    //设置接收缓冲区
            m_RcvCtrlData.ulFreeLen = ulLen;    //接收缓冲区长度
        }
    
    return CMU_ERR_SUCCESS;
}

//等待指令执行结果
//返回值：0--等待结果完成 CMU_ERR_SYS_BUSY--未完成
uBit32 COM_PL_WaitForProcResult(uBit32 *pErrCode)
{
    uBit32 ulRet;
    //uBit32 ulExeRes;
    uBit32 ulCmdType=0;
    double64 dCurTickCount;

    dCurTickCount = m_sExternalFunTable.pf_SYS_GetSysTickCount();
    dCurTickCount -= m_RcvCtrlData.dStartTick;

    if (m_RcvCtrlData.uCmdModule==0)//设备管理模块
    {
        ulRet = m_sExternalFunTable.pf_DEV_CheckLastCmdExeState(m_RcvCtrlData.uDevNo, &ulCmdType, pErrCode);
    }else//通道管理模块
    {
        ulRet = m_sExternalFunTable.pf_LAX_CheckLastCmdExeState(m_RcvCtrlData.uCrdNo, m_RcvCtrlData.uDevNo, &ulCmdType, pErrCode);
    }
    
    //已经获取结果
    if (ulRet==0)
    {    
        if (*pErrCode == 0)
        {
            //指令不匹配，则说明指令失败
            if (ulCmdType!=m_RcvCtrlData.uCmdType)
                *pErrCode = CMU_ERR_CMD_DISMATCH;
        }

        return CMU_ERR_SUCCESS;
    }

    //超时
    if (dCurTickCount>m_RcvCtrlData.ulTimeOutCount)
    {
        *pErrCode = CMU_ERR_TIME_OUT;
        return CMU_ERR_SUCCESS;
    }

    return CMU_ERR_SYS_BUSY;
}

/*-------------------------------------------------------------
函 数 名：uBit32 COM_PL_SavePackToRcvCtrlData()
函数说明：    保存接收到的数据包到接收控制数据区
参数说明：
            pPack                -- 数据包
            pRcvCtrlData        -- 接收控制数据区
返 回 值：  CMU_ERR_SUCCESS        --    成功
            CMU_ERR_RCV_OVERFLOW    --  接收数据区无法容纳接收到的数据
注意事项：
*/
uBit32 COM_PL_SavePackToRcvCtrlData()
{
    uBit8 *pStart;

    if (m_RcvCtrlData.ulFreeLen < m_sPackData.ulDataLen)
    {
        return CMU_ERR_RCV_OVERFLOW;
    }

    pStart = m_RcvCtrlData.pRevBuf + m_RcvCtrlData.ulRevLen; 
    memcpy(pStart, m_sPackData.pDataBuf, m_sPackData.ulDataLen);

    m_RcvCtrlData.ulRevLen += m_sPackData.ulDataLen;
    m_RcvCtrlData.ulFreeLen -= m_sPackData.ulDataLen;
    m_RcvCtrlData.ulRevID = m_sPackData.ulID;

    //计算校验和
    for (unsigned long i=0; i<m_sPackData.ulDataLen; i++)
        m_RcvCtrlData.cCheckNum += *(m_sPackData.pDataBuf + i);

    return CMU_ERR_SUCCESS;
}

/*
函数名称：uBit32 COM_PL_PackDataProc(void)

功    能：处理接收到的数据包，如果数据块接收完成则处理数据块

参    数：

返 回 值：0--成功 非0--错误码

注意事项：通信模块接收主线程调用
*/
uBit32 COM_PL_PackDataProc(void)
{
    uBit32 ulRet;

    switch (m_sPackData.ulID.ulComDataID.ulTransmitFrame)
    {
        case TRANSMIT_FIRST_FRAME://起始数据包
            {
#ifdef RS_MONITOR_ENALBE
                g_sCmuRsMonitorData.ulRcvBlockCount++;
#endif
                COM_PL_ResetRcvCtrlData(m_sPackData.ulID.ulFrameID, m_pRcvBuf, COM_TRANSMIT_BUF_SIZE);
                m_RcvCtrlData.cReceiving = 1;
                ulRet = COM_PL_SavePackToRcvCtrlData();
            }
            break;
        case TRANSMIT_SELF_FRAME: //独立数据包
            {
#ifdef RS_MONITOR_ENALBE
                g_sCmuRsMonitorData.ulRcvBlockCount++;
#endif
                COM_PL_ResetRcvCtrlData(m_sPackData.ulID.ulFrameID, m_pRcvBuf, COM_TRANSMIT_BUF_SIZE);
                ulRet = COM_PL_SavePackToRcvCtrlData();

                if (ulRet==CMU_ERR_SUCCESS)
                {
                    ulRet = CMU_CmdProcess(&m_RcvCtrlData);
#ifdef RS_MONITOR_ENALBE
                    g_sCmuRsMonitorData.ulBlockRcvSucCount++;
#endif
                }
            }
            break;
        case TRANSMIT_BEING_FRAME:
            {
                if (m_RcvCtrlData.cReceiving==1)
                {
                    if ((m_sPackData.ulID.ulFrameID&SAME_TYPE_PACK_MASK)==\
                        (m_RcvCtrlData.ulRevID.ulFrameID&SAME_TYPE_PACK_MASK))
                    {
                        m_RcvCtrlData.ulRevID.ulComDataID.ulPackIndex +=1;
                        
                        if (m_RcvCtrlData.ulRevID.ulComDataID.ulPackIndex != m_sPackData.ulID.ulComDataID.ulPackIndex)
                        {
                            CMU_InitRecCtrlData();                 
                            ulRet = CMU_ERR_DATA_LOST;
                        }
                        else
                            {
                                ulRet = COM_PL_SavePackToRcvCtrlData();
                                
                                if(ulRet!=CMU_ERR_SUCCESS)//接收缓冲区空间不够
                                    CMU_InitRecCtrlData();
                            }
                    }//
                    else
                        {
                            ulRet = CMU_ERR_DATA_LOST;     //立即返回数据丢失错误
                            CMU_InitRecCtrlData();
                        }
                }
                else
                    {
                        ulRet = CMU_ERR_OUTMODED;    //已经中断接收的过时数据
                    }
            }
            break;
        case TRANSMIT_VERIFY_FRAME://校验帧
            {
                if (m_RcvCtrlData.cReceiving==1)
                {
                    if ((m_sPackData.ulID.ulFrameID&SAME_TYPE_PACK_MASK)==\
                        (m_RcvCtrlData.ulRevID.ulFrameID&SAME_TYPE_PACK_MASK))
                    {
                        if (m_RcvCtrlData.cCheckNum == m_sPackData.pDataBuf[0])
                        {
                            ulRet = CMU_CmdProcess(&m_RcvCtrlData);
#ifdef RS_MONITOR_ENALBE
                            g_sCmuRsMonitorData.ulBlockRcvSucCount++;
#endif
                        }
                        else
                        {
                            ulRet = CMU_ERR_CHECK_FAIL;
                        }
                    }else
                    {
                        ulRet = CMU_ERR_DATA_LOST;//返回数据丢失错误
                    }
                }
                else
                    {
                        ulRet = CMU_ERR_DATA_LOST;//返回数据丢失错误，数据块要等到接收校验包时才返回错误
                    }
            }
            break;
        default: 
            ulRet = CMU_ERR_DATA_WRONG;//无法识别的数据包
            break;
    }

    return ulRet;
}


/*
函数名称：uBit32 CMU_MainRcvProc(void);

功    能：接收数据处理线程，接收并处理接收数据包，处理完成后发送回包

参    数：

返 回 值：0--成功 非0--错误编码

注意事项：通信模块主线程调用
*/
uBit32 CMU_MainRcvProc(void)
{
    uBit32 ulRet;
    
#ifdef SYS_FUN_TEST
    GPIO_FunTestStart(CMU_CMU_MainRcvProc);
#endif
    
    if (m_RcvCtrlData.ulWaitResultCode!=0)      //等待处理结果
    {
        if (COM_PL_WaitForProcResult(&ulRet)!=CMU_ERR_SUCCESS)//未等到处理结果
            return CMU_ERR_SUCCESS;

        //等待外部接口处理结果结束
        m_RcvCtrlData.ulWaitResultCode = 0;
        m_RcvCtrlData.ulTimeOutCount = 0;
        m_RcvCtrlData.uCmdType = 0;
        m_sRcvTimeOutCtrlData.ulExeResult=ulRet;
        ulRet = CMU_SendResponsePack(m_RcvCtrlData.ulRevID.ulFrameID, ulRet);

        if (ulRet)
            return CMU_ERR_SEND_FAIL;
    }
#ifdef WIN32_VER
    ulRet = COM_AL_CMUGetPack(&m_sPackData);
#else
    ulRet = COM_AL_GetPack(&m_sPackData);
#endif

    if (ulRet==CMU_ERR_SUCCESS)//成功收到数据包
    {
        ulRet = COM_PL_PackDataProc();
        
        //接收到数据包，但处理数据包失败，则直接发送错误回包
        if (ulRet!=CMU_ERR_SUCCESS && ulRet != CMU_ERR_OUTMODED)
        {
            ulRet = CMU_SendResponsePack(m_sPackData.ulID.ulFrameID, ulRet);
        }
    }

    if (ulRet==CMU_ERR_CONNECT)
        return CMU_ERR_CONNECT;

#ifdef SYS_FUN_TEST
    GPIO_FunTestEnd(CMU_CMU_MainRcvProc);
#endif
    return CMU_ERR_SUCCESS;
}
