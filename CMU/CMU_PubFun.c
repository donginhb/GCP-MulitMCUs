
#include "string.h"
#include "CMU_Interface.h"
#include ".\\ProtocolLayer\\CMU_ExApi.h"
#include ".\\ProtocolLayer\\CMU_PeriodDataMana.h"
#include ".\\ProtocolLayer\\CMU_SendDataProcess.h"
#include ".\\ProtocolLayer\\CMU_RcvDataProcess.h"
#include ".\\AbstractionLayer\\AbstractionLayer.h"

#include "CMU_ErrorCodeDef.h"


//#include "..\\SysPeripheral\\GPIO\\GPIOCtrl.h"
//#include "..\\SysUpdate\\BootLoader\\BootLoaderDefine.h"
//#include "..\\SysUpdate\\BootLoader\\BootLoader.h"



static uBit8 m_uComSetDelayCycles = 0;

static uBit32 m_ulCmuMemBase = 0;                    //当前可用缓冲区基地址
static uBit32 m_ulCmuMemRestLen = 0;                //当前可用缓冲区剩余长度

/*
函数名称：uBit32 CMU_SetRSBufAddr(uBit32 ulStartAddr, uBit32 ulLen);

功    能：设置通信管理模块收发缓冲区缓冲区空间

参    数：ulStartAddr---起始地址
                ulLen           ---分配到缓冲区长度

返 回 值：0--成功，1-分配缓冲区长度不够

注意事项：必须在通信模块初始化之前调用，外部指定的缓冲区空间至少是两个COM_TRANSMIT_BUF_SIZE的长度,在初始化通信模块之前调用
*/
uBit32 CMU_SetRSBufAddr(uBit32 ulStartAddr, uBit32 ulLen)
{
    if(ulLen<2*COM_TRANSMIT_BUF_SIZE)
        return 1;

    m_ulCmuMemBase = ulStartAddr;
    m_ulCmuMemRestLen = ulLen;
    return 0;
}

/*
函数名称：void* CMU_Malloc(uBit32 ulSize)

功    能：从通信模块申请的缓冲区空间中分配指定大小的空间

参    数：ulSize---需要分配的空间长度

返 回 值：分配到空间的首地址，NULL表示未分配到任何空间

注意事项：缓冲区空间按8字节的地址分配，即分配的地址开始位置都为8的整数倍
*/
void* CMU_Malloc(uBit32 ulSize)
{
    uBit32 ulGap=0;
      uBit8* pCurAddr;

    //计算当前缓冲区基址到下个有效地址间的字节数
    ulGap = m_ulCmuMemBase%8;

    if (ulGap!=0)
    {
        if (ulGap>=m_ulCmuMemRestLen)
            return NULL;

        m_ulCmuMemRestLen -= ulGap;
        m_ulCmuMemBase += ulGap;
    }

    //缓冲区空间不够
    if (ulSize>m_ulCmuMemRestLen)
        return NULL;

    pCurAddr = (uBit8*)m_ulCmuMemBase;
    m_ulCmuMemBase += ulSize;
    m_ulCmuMemRestLen -= ulSize;

    return pCurAddr;
}


/*
函数名称：uBit32 CMU_Init(uBit8 uComType)

功    能：通信模块初始化函数，初始化内容包括：周期性数据管理模块初始化、收发缓冲区初始化，为没有挂接的外部函数
          设置默认值，打开通信连接

参    数：uComType---通信方式，设置系统默认的通信方式
        COM_TYPE_CAN        (1)   //CAN通信模式
        COM_TYPE_ENET        (2)   //以太网通信模式

返 回 值：0--成功 非0--错误编码

注意事项：系统初始化时调用，调用前必须先设置通信模块缓冲区、挂接通信接口、挂接其他外部接口
*/
uBit32 CMU_Init(uBit8 uComType)                                
{
    uBit32 ulRet=0;
    //周期性数据处理初始化
    CMU_PeriodDataManaInit();

    //绑定初始化处理函数
    CMU_InitExApi();

    //接收数据区初始化
    ulRet = CMU_InitRecCtrlData();
    if (ulRet)
        return ulRet;

    //发送数据区初始化
    ulRet = CMU_ResetSendCtrlData(0,NULL,0);

    if (ulRet)
        return ulRet;

    //打开通信连接
    if (COM_AL_Open(uComType))
        return CMU_ERR_CONNECT;

    return CMU_ERR_SUCCESS;
}


/*
函数名称：uBit32 CMU_MainProc();

功    能：通信模块主线程,管理和上位机的通信（收发数据包处理、周期性查询数据上传、下位机错误上传）

参    数：无

返 回 值：0--成功 非0--错误编码

注意事项：系统初始化时调用，调用前必须先设置通信模块缓冲区、挂接通信接口、挂接其他外部接口
*/
uBit32 CMU_MainProc()                            
{
    uBit32 ulRet;

     //切换通信方式
    if (m_uComSetDelayCycles!=0)
    {
        m_uComSetDelayCycles--;

        if (m_uComSetDelayCycles==0)
        {
            //切换通信方式
            //uBit32 ulComType, ulNode;
            //BOOT_GetComType(&ulComType, &ulNode);
            //CMU_Init(ulComType);
            CMU_Init(COM_TYPE_ENET);
        }
    }    
    
    //底层硬件接收数据
#ifdef SYS_FUN_TEST
    GPIO_FunTestStart(CMU_CMU_MainProc);
#endif

    COM_AL_RcvHandle(); 

    if (!CMU_IsSendFinished())  
    {    //上次发送未完成,则继续发送未发送完成的数据
        if(CMU_MainSendProc() != CMU_ERR_SUCCESS)//发送失败（硬件发送失败后）
            CMU_PeriodSendSwitch(false);
        else
            CMU_PeriodSendSwitch(true);
    }

    //数据接收处理
    ulRet = CMU_MainRcvProc();     

    if (ulRet)
        return ulRet;

    if(CMU_IsSendFinished())   
    {//没有发送数据，则发送错误和处理周期性数据发送
        //CMU_ReportErrorProc();
        //ulRet = CMU_MainPeriodInqProc();
    }


#ifdef SYS_FUN_TEST
    GPIO_FunTestEnd(CMU_CMU_MainProc);
#endif

    return ulRet;
}

/*
函数名称：uBit32 CMU_GetComType(void)
功    能：获取当前通信模块使用的通信类型
参    数：无
返 回 值：当前的通信类型
注意事项：
*/
uBit32 CMU_GetComType(void)
{
    return COM_AL_GetComType();
}


/*
函 数 名：uBit32 CMU_SetComType(uBit32 ulComType)
功    能：设置控制器与上位机的通信方式
参    数：ulComType  --通信方式
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：设置通信方式切换标志，并保存通信方式到EPPROM中
*/
uBit32 CMU_SetComType(uBit32 ulComType)
{
#if 0
#ifndef WIN32_VER
    //设置bootloader通信方式
    if (ulComType==COM_TYPE_CAN)
        BOOT_SetComType(BOOT_COM_CAN, CAN1_NODE_ID);
    else
        BOOT_SetComType(BOOT_COM_ENET, 0);

    m_uComSetDelayCycles=10;

#endif
#endif

    return 0;
}
