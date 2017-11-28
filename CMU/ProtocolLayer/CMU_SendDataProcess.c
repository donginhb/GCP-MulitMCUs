
#include "string.h"
#include "CMU_ErrorMana.h"
#include "CMU_CmdProcess.h"
#include "CMU_SendDataProcess.h"
#include "..\\CMU_Interface.h"

#include "..\\CMU_ErrorCodeDef.h"


//#include "..\\..\\GPIO\\GPIOCtrl.h"
#include "..\\AbstractionLayer\\AbstractionLayer.h"


#define MAX_PACK_COUNT_PER_CICLE        (1)        //单周期最大发送的数据包个数

COM_SLAVE_SEND_CTRL_DATA    m_SendCtrlData;        //发送控制数据

#ifdef WIN32_VER
uBit8                        m_pSendBuf[ENET_PACK_MAX_LEN];
#else
uBit8                        *m_pSendBuf=NULL;
#endif

//测试----------
//int g_iSendTick=0;
//--------------

//检查是否还有数据未发送
BooLean CMU_IsSendFinished(void)    
{
    if (m_SendCtrlData.ulRestLen != 0)
        return false;

    return true;
}


/*
函数名称：void CMU_ResetSendCtrlData(uBit32 ulID, uBit8* pBuf, uBit32 ulLen)

功    能：重置发送控制数据区

参    数：ulID---发送数据包ID
          pBuf---待发送的配置数据，当pBuf==NULL时，使用系统默认的发送缓冲区(必须将要发送的数据直接保存在默认发送缓冲区中)
          ulLen--待发数据长度

返 回 值：0--成功 非0--错误编码

注意事项：可以配置发送缓冲区为默认发送缓冲区，适合发送临时数据
          也可以配置外部发送缓冲区，适合发送固定地址段数据如控制参数等
*/
uBit32 CMU_ResetSendCtrlData(uBit32 ulID, uBit8* pBuf, uBit32 ulLen)
{
    memset(&m_SendCtrlData, 0, sizeof(COM_SLAVE_SEND_CTRL_DATA));
#ifdef WIN32_VER
    memset(&m_pSendBuf, 0, ENET_PACK_MAX_LEN);

    if (pBuf==NULL) //使用系统默认缓冲区
    {
        m_SendCtrlData.pBuf = m_pSendBuf;
        m_SendCtrlData.ulFreeLen = COM_TRANSMIT_BUF_SIZE;
    }
    else           //使用指定缓冲区
    {
        m_SendCtrlData.pBuf = pBuf;
        m_SendCtrlData.ulRestLen = ulLen;
    }
#else
    if (pBuf==NULL) //使用系统默认缓冲区
    {
        //为默认发送缓冲区分配空间
        if(m_pSendBuf==NULL)
            m_pSendBuf = CMU_Malloc(COM_TRANSMIT_BUF_SIZE);

        if (m_pSendBuf)
        {
            m_SendCtrlData.pBuf = m_pSendBuf;
            m_SendCtrlData.ulFreeLen = COM_TRANSMIT_BUF_SIZE;
        }else
        {
            return CMU_ERR_RS_MALLOC_FAIL;       
        }
    }
    else           //使用指定缓冲区
    {
        m_SendCtrlData.pBuf = pBuf;
        m_SendCtrlData.ulRestLen = ulLen;
    }
#endif

    m_SendCtrlData.ulSendID.ulFrameID = ulID;
    return CMU_ERR_SUCCESS;
}

//发送缓冲区数据溢出检查
BooLean CMU_CheckSendBufFreeSize(uBit32 ulLen)
{
    if (m_SendCtrlData.ulFreeLen < ulLen)
        return false;    //发送数据溢出

    return true;
}

//获取当前发送控制数据缓冲区剩余空间首地址
uBit8* CMU_GetSendBufAddr()
{
    if(m_SendCtrlData.ulFreeLen==0)
        return NULL;

    if(m_SendCtrlData.pBuf==NULL)
        return NULL;

    return m_SendCtrlData.pBuf + m_SendCtrlData.ulRestLen;
}


/*
函数名称：uBit32 CMU_AddToSendCtrlData(uBit8* pBuf, uBit32 ulLen)

功    能：将数据缓冲区添加到发送控制数据缓冲区中

参    数：pBuf---待添加的数据首地址
          ulLne--待添加的数据长度

返 回 值：0--正确  非0--错误码

注意事项：重置发送控制数据区后调用，如果pBuf不为NULL，表示要将待发数据转移到发送控制数据区，适合将不连续地址的数据添加到发送控制数据区;
          如果pBuf为NULL，表示只修改发送控制数据区剩余空间首指针，适合外部函数直接使用默认发送缓冲区；
*/
uBit32 CMU_AddToSendCtrlData(uBit8* pBuf, uBit32 ulLen)
{
    uBit8 *pSendBuf;

    if (pBuf)
    {
        if(m_SendCtrlData.ulFreeLen < ulLen)
            return CMU_ERR_SEND_OVERFLOW;

        pSendBuf = m_SendCtrlData.pBuf + m_SendCtrlData.ulRestLen;
        memcpy(pSendBuf, pBuf, ulLen);
    }

    m_SendCtrlData.ulRestLen += ulLen;
    m_SendCtrlData.ulFreeLen -= ulLen;
    return CMU_ERR_SUCCESS;
}

/*
函数名称：uBit32 CMU_SendResponsePack(uBit32 ulID, uBit32 ulErrCode)

功    能：创建回应数据包,并立即发送，在设置指令处理完成后和接收出错时调用

参    数：ulID---回应包ID
          ulErrCode---回应包错误码

返 回 值：错误码

注意事项：0--正确  非0--错误编码
*/
uBit32 CMU_SendResponsePack(uBit32 ulID, uBit32 ulErrCode)
{
    uBit32 ulRet;
    COM_DATA_ID ulComDataID;

    ulComDataID.ulFrameID = ulID;
    ulComDataID.ulComDataID.ulTransmitFrame = TRANSMIT_SELF_FRAME;
    ulComDataID.ulComDataID.ulReceiveObj = SCR_RECEIVE;

    if (ulErrCode)
    {
        ulComDataID.ulComDataID.ulErrFlag = 1;
        
        if(ulErrCode < 0xFF)//内部产生的错误，转换错误码
            ulErrCode = CMU_CreateError(ulErrCode, ulID);
    }

    //测试----------
    //g_iSendTick=0;
    //--------------
#ifdef WIN32_VER
    ulRet = COM_AL_CMUSendPack(ulComDataID.ulFrameID, (uBit8*)&ulErrCode, sizeof(uBit32));
#else
    ulRet = COM_AL_SendPack(ulComDataID.ulFrameID, (uBit8*)&ulErrCode, sizeof(uBit32));
#endif


#ifdef RS_MONITOR_ENALBE
    g_sCmuRsMonitorData.ulSendBlockCount++;

    if (ulRet != CMU_ERR_SUCCESS)
        g_sCmuRsMonitorData.ulSendFailCount++;
    else
        g_sCmuRsMonitorData.ulBlockSendSucCount++;
#endif

    return ulRet;
}


/*
函数名称：uBit32 CMU_MainSendProc(void);

功    能：通信模块发送控制主线程

参    数：无

返 回 值：0--正确 非0--错误码

注意事项：通信模块主线程中调用，对于以太网来说，两包数据发送之间必须有一定的延时
*/
uBit32 CMU_MainSendProc(void)
{
    int iDelayCycle=0;
    uBit32 ulRet;
    uBit32 ulMaxLen;    //能发送数据区的最大长度
    uBit32 ulSendCount;    //已发送的数据包
    uBit32 ulMaxPackPerTime=1;    //单次连续可发送数据包数量

    ulSendCount = 0;
#ifdef WIN32_VER
    ulMaxLen = COM_AL_EnetGetMaxSendLen();
#else
    ulMaxLen = COM_AL_GetMaxSendLen();
#endif

    //无数据需要发送
    if (m_SendCtrlData.ulRestLen==0)
        return CMU_ERR_SUCCESS;

#ifdef SYS_FUN_TEST
    GPIO_FunTestStart(CMU_CMU_MainSendProc);
#endif

    //独立数据包发送
    if ((m_SendCtrlData.ulSentLen == 0) && (m_SendCtrlData.ulRestLen <= ulMaxLen))
    {
        m_SendCtrlData.ulSendID.ulComDataID.ulTransmitFrame = TRANSMIT_SELF_FRAME;
        m_SendCtrlData.ulSendID.ulComDataID.ulReceiveObj = SCR_RECEIVE;

#ifdef WIN32_VER
        ulRet = COM_AL_CMUSendPack(m_SendCtrlData.ulSendID.ulFrameID, m_SendCtrlData.pBuf, m_SendCtrlData.ulRestLen);
#else
        ulRet = COM_AL_SendPack(m_SendCtrlData.ulSendID.ulFrameID, m_SendCtrlData.pBuf, m_SendCtrlData.ulRestLen);
#endif

#ifdef RS_MONITOR_ENALBE
        g_sCmuRsMonitorData.ulSendBlockCount++;

        if (ulRet==CMU_ERR_SUCCESS)//发送成功
        {
            g_sCmuRsMonitorData.ulBlockSendSucCount++;
            m_SendCtrlData.ulRestLen = 0;
            m_SendCtrlData.ulSentLen = m_SendCtrlData.ulRestLen;
        }else
        {
            g_sCmuRsMonitorData.ulSendFailCount++;
        }
#else
        if (ulRet==CMU_ERR_SUCCESS)//发送成功
        {
            m_SendCtrlData.ulRestLen = 0;
            m_SendCtrlData.ulSentLen = m_SendCtrlData.ulRestLen;
        }
#endif

#ifdef SYS_FUN_TEST
        GPIO_FunTestEnd(CMU_CMU_MainSendProc);
#endif
        return ulRet;
    }

#ifndef WIN32_VER
    ulMaxPackPerTime = COM_AL_GetMaxSendPacks();
#endif
      
    //尚未开始发送数据，则先发送起始数据包
    if (m_SendCtrlData.ulSentLen==0)
    {
        m_SendCtrlData.ulSendID.ulComDataID.ulTransmitFrame = TRANSMIT_FIRST_FRAME;

#ifdef WIN32_VER
        ulRet = COM_AL_CMUSendPack(m_SendCtrlData.ulSendID.ulFrameID, m_SendCtrlData.pBuf, ulMaxLen);
#else
        ulRet = COM_AL_SendPack(m_SendCtrlData.ulSendID.ulFrameID, m_SendCtrlData.pBuf, ulMaxLen);
#endif

#ifdef RS_MONITOR_ENALBE
        g_sCmuRsMonitorData.ulSendBlockCount++;
#endif

        if (ulRet==CMU_ERR_SUCCESS)//发送成功
        {
            m_SendCtrlData.ulRestLen -= ulMaxLen;
            m_SendCtrlData.ulSentLen += ulMaxLen;
            m_SendCtrlData.ulSendID.ulComDataID.ulPackIndex += 1;

            //计算校验和,并重新定位发送数据区
            for (uBit32 i=0; i<ulMaxLen; i++,m_SendCtrlData.pBuf++)
                m_SendCtrlData.cCheckNum += *m_SendCtrlData.pBuf;

            ulSendCount++;
#ifndef WIN32_VER
            //需要加适当的延时，否则会出现发送失败,连续发送时硬件响应没那么快（约4us）
            if(ulSendCount<ulMaxPackPerTime)
            {
                for(iDelayCycle=0; iDelayCycle<CAN_CONTINUE_SEND_GAP; iDelayCycle++);
            }
#endif
        }
        else
            {
#ifdef SYS_FUN_TEST
                GPIO_FunTestEnd(CMU_CMU_MainSendProc);
#endif

#ifdef RS_MONITOR_ENALBE
                g_sCmuRsMonitorData.ulSendFailCount++;
#endif
                return ulRet;
            }
    }

    //发送中间数据包、结束包和校验包
    for (uBit32 i=ulSendCount; i<ulMaxPackPerTime; i++)
    {
        if (m_SendCtrlData.ulRestLen<=ulMaxLen)//最后一包数据时发送结束包和校验包
        {
            m_SendCtrlData.ulSendID.ulComDataID.ulTransmitFrame = TRANSMIT_BEING_FRAME;

#ifdef WIN32_VER
            ulRet = COM_AL_CMUSendPack(m_SendCtrlData.ulSendID.ulFrameID, m_SendCtrlData.pBuf , m_SendCtrlData.ulRestLen);
#else
            ulRet = COM_AL_SendPack(m_SendCtrlData.ulSendID.ulFrameID, m_SendCtrlData.pBuf , m_SendCtrlData.ulRestLen);
            
            //需要加适当的延时，否则会出现发送失败,连续发送时硬件响应没那么快（约4us）
            for(iDelayCycle=0; iDelayCycle<CAN_CONTINUE_SEND_GAP; iDelayCycle++);
#endif
            
            if (ulRet==CMU_ERR_SUCCESS)
            {
                //计算校验值
                for (uBit32 j=0; j<m_SendCtrlData.ulRestLen; j++,m_SendCtrlData.pBuf++)
                    m_SendCtrlData.cCheckNum += *m_SendCtrlData.pBuf;

                m_SendCtrlData.ulRestLen = 0;
                m_SendCtrlData.ulSentLen += m_SendCtrlData.ulRestLen;
                m_SendCtrlData.ulSendID.ulComDataID.ulPackIndex += 1;

                //发送校验帧
                m_SendCtrlData.ulSendID.ulComDataID.ulTransmitFrame = TRANSMIT_VERIFY_FRAME;
#ifdef WIN32_VER
                ulRet = COM_AL_CMUSendPack(m_SendCtrlData.ulSendID.ulFrameID, &m_SendCtrlData.cCheckNum , 1);
#else
                ulRet = COM_AL_SendPack(m_SendCtrlData.ulSendID.ulFrameID, &m_SendCtrlData.cCheckNum , 1);
#endif

#ifdef RS_MONITOR_ENALBE
                if (ulRet==CMU_ERR_SUCCESS)
                    g_sCmuRsMonitorData.ulBlockSendSucCount++;
                else
                    g_sCmuRsMonitorData.ulSendFailCount++;
#endif
                break;
            }else
            {
#ifdef RS_MONITOR_ENALBE
                g_sCmuRsMonitorData.ulSendFailCount++;
#endif
            }
        }
        else//发送中间包
            {
                m_SendCtrlData.ulSendID.ulComDataID.ulTransmitFrame = TRANSMIT_BEING_FRAME;
#ifdef WIN32_VER
                ulRet = COM_AL_CMUSendPack(m_SendCtrlData.ulSendID.ulFrameID, m_SendCtrlData.pBuf , ulMaxLen);
#else
                ulRet = COM_AL_SendPack(m_SendCtrlData.ulSendID.ulFrameID, m_SendCtrlData.pBuf , ulMaxLen);
#endif

                if (ulRet==CMU_ERR_SUCCESS)
                {
                    m_SendCtrlData.ulRestLen -= ulMaxLen;
                    m_SendCtrlData.ulSentLen += ulMaxLen;
                    m_SendCtrlData.ulSendID.ulComDataID.ulPackIndex += 1;

                    //计算校验值
                    for (uBit32 j=0; j<ulMaxLen; j++,m_SendCtrlData.pBuf++)
                        m_SendCtrlData.cCheckNum += *m_SendCtrlData.pBuf;

                    //防止发生多包数据时丢失要接收的数据，还起到连续数据发送间的时间缓冲
#ifndef WIN32_VER

                    if(COM_AL_GetComType()==COM_TYPE_CAN && i<ulMaxPackPerTime)
                    {    //需要加适当的延时，否则会出现发送失败,连续发送时硬件响应没那么快（约4us）
                        for(iDelayCycle=0; iDelayCycle<CAN_CONTINUE_SEND_GAP; iDelayCycle++);
                    }
#endif
                }else
                {
#ifdef RS_MONITOR_ENALBE
                    g_sCmuRsMonitorData.ulSendFailCount++;
#endif
                }
            }

        if (ulRet)
            break;
    }

#ifdef SYS_FUN_TEST
    GPIO_FunTestEnd(CMU_CMU_MainSendProc);
#endif
    return ulRet;
}