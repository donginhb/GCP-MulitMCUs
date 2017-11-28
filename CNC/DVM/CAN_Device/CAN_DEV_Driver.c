
#include "string.h"
#include "CAN_DEV_Driver.h"
#include "CAN_DEV_ComMana.h"
#include "CAN_DEV_Interface.h"

#include "..\\..\\ErrorCodeDef\\DVM_ErrorCodeDef.h"
#include "..\\..\\DataStructDef\\IPO_DataStructDefPub.h"

//#include "..\\..\\GPIO\\GPIOCtrl.h"

#define MAX_PULSE_INC            (511)    //单周期最大允许脉冲数

//------------------------CAN总线驱动数据定义---------------------------------------------------
SOFTWARE_VERSION            m_sCanBusDriverSoftVer;            //CAN总线驱动库软件版本
const AXIS_PARM                *m_pCanDevAxisParm = NULL;            //电机控制参数数据区
const AXIS_PITCH_CMP_PARM    *m_pCanDevAxisPitchCmpParm = NULL;    //电机螺距补偿参数数据区
const SYS_CTRL_PARM                *m_pCanSysCtrlParm=NULL;                //系统控制参数，设置系统控制参数时更新

DEVICE_STATE                *m_pCanDeviceState = NULL;        //设备状态数据区
//uBit8                                m_uMaxDevStateCount = 0;        //最大设备状态数据数量

DEVICE_INFO            *m_pCanDeviceInfo    = NULL;            //当前系统设备资源信息数据区
uBit8                        m_uCanExsitDevCount = 0;            //系统已经存在的设备数量(上次扫描系统所有设备数量)

COM_NO_MAP_DATA                m_pComNoMap[DEV_MAX_COUNT];        //保存设备编号对应的通信编号，pDevNoMap[0]保存设备1对应的通信编号（从0开始）
CANBUS_CH_DATA                m_pChData[CAN_CHANNEL_MAX_COUNT];//通道数据

CAN_INTERFACE               m_sCanComInterface;
CAN_DEV_EX_INTERFACE        m_sCanDevExInterface;
//----------------------------------------------------------------------------------------------

#ifdef RS_MONITOR_ENALBE
BUS_RS_MS_DATA m_sCanBusRsMsData;    //CAN总线测试数据，重新编号后清除

BUS_RS_MS_DATA CAN_DEV_GetBusRsMsData(void)
{
    return m_sCanBusRsMsData;
}
#endif


/*
函 数 名：CAN_DEV_BusInit
功    能：CAN总线初始化
参    数：iBusNO --总线编号
返 回 值：成功返回0，否则返回错误码
调用位置：系统初始化时调用，只调用一次
注意事项：
*/
uBit32 CAN_DEV_BusInit(uBit32 ulBusChMask)
{
    int i;

#ifdef RS_MONITOR_ENALBE
    memset(&m_sCanBusRsMsData, 0, sizeof(BUS_RS_MS_DATA));
#endif

    memcpy(m_sCanBusDriverSoftVer.cType, "CAN0", 4);
    m_sCanBusDriverSoftVer.cSoftVer[0] = 1;
    m_sCanBusDriverSoftVer.cSoftVer[1] = 0;
    m_sCanBusDriverSoftVer.cPart = 10;
    m_sCanBusDriverSoftVer.cFlag = 'T';

    m_pCanDevAxisParm = NULL;                //电机控制参数数据区
    m_pCanDevAxisPitchCmpParm = NULL;        //电机螺距补偿参数数据区
    m_pCanDeviceState = NULL;                //设备状态数据区
    m_pCanSysCtrlParm = NULL;
    //m_uMaxDevStateCount = 0;                //最大设备状态数据数量
    m_pCanDeviceInfo    = NULL;                //设备资源信息数据区
    m_uCanExsitDevCount = 0;                //最大设备资源信息数量

    for (i=0;  i<DEV_MAX_COUNT;  i++)
    {
        m_pComNoMap[i].uBusChIndex = 1;
        m_pComNoMap[i].uComNo = 0X7F;
    }


    memset(m_pChData, 0, CAN_CHANNEL_MAX_COUNT*sizeof(CANBUS_CH_DATA));        //通道数据

    //添加CAN总线通信模块初始化代码
    if (m_sCanComInterface.pf_CAN_Open)
    {
        if (ulBusChMask & DVM_CH0_MASK)//CAN0
        {
            m_sCanComInterface.pf_CAN_Open(CAN0_NODE_ID, CANBAUD_500K, 0X10000000, 0);
            m_pChData[CAN0_NODE_ID].bValidFlag = 1;
        }

        if (ulBusChMask & DVM_CH1_MASK)
        {
            m_sCanComInterface.pf_CAN_Open(CAN1_NODE_ID, CANBAUD_500K, 0X10000000, 0);
            m_pChData[CAN1_NODE_ID].bValidFlag = 1;
        }
    }else
    {
        return DVM_ERR_BUS_INIT_FAIL;
    }

    return DVM_ERR_SUCCESS;
}

/*
函 数 名：CAN_DEV_SetConfig
功    能：配置设备编号
参    数：pDeviceConfig --设备配置数据存储地址
          ulDeviceCount --要配置的设备数量
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：配置设备编号前需先调用CAN_DEV_SetDeviceInfoAddr和CAN_DEV_SetDeviceStateAddr
*/
uBit32 CAN_DEV_SetConfig(DEVICE_CONFIG* pDeviceConfig, uBit32 ulDeviceCount)
{
    uBit8 i;
    uBit32 ulRet;
    uBit8 uDevIndex;
    uBit8 uComNo;
    uBit8 uBusChIndex;
    const AXIS_PARM *pAxisParm;
    DEVICE_STATE *pDevState;
    DEVICE_INFO *pDeviceInfo = NULL;
    MAINCTRL_COM_DATA_PACK sSetCmdPack;
    uBit8  pComNo[CAN_CHANNEL_MAX_COUNT];//通道通信编号

#ifdef SYS_FUN_TEST
    GPIO_FunTestStart(DVM_CAN_DEV_SetConfig);
#endif

    //总线空闲检查，检查所有有效通道是否空闲
    if (CAN_DEV_IsBusFree() == 0)
        return DVM_ERR_BUS_BUSY;

    //请求有效通道收发权限，并收光通道所有数据----------------
    for (uBusChIndex=0; uBusChIndex<CAN_CHANNEL_MAX_COUNT; uBusChIndex++)
    {
        if(m_pChData[uBusChIndex].bValidFlag==1)//通道有效
        {
            if(AskForRsPermit(uBusChIndex) != DVM_ERR_SUCCESS)//请求通道
            {
                if(uBusChIndex)
                {
                   for(int i=uBusChIndex-1; i>=0; i--)
                   {
                        if(m_pChData[i].bValidFlag==1)
                            ReleaseRsPermit(i);
                   }
                }

                return DVM_ERR_BUS_BUSY;
            }
        }
    }

    //重新初始化配置数据
    SetConfigDataToDefault();
    memset(pComNo, 0, CAN_CHANNEL_MAX_COUNT);

    for (uDevIndex=0; uDevIndex<ulDeviceCount; uDevIndex++, pDeviceConfig++)
    {
        //检查设备是否属于该总线设备-----------
        ulRet = 0;
        pDeviceInfo = m_pCanDeviceInfo;
        for (i=0; i<m_uCanExsitDevCount; i++, pDeviceInfo++)
        {
            if (pDeviceInfo->nDevieID != pDeviceConfig->nDevieID)
                continue;

            if (pDeviceInfo->nBusType != DEV_BUS_TYPE_CAN)//不是该总线设备
                continue;

            ulRet = 1;
            break;
        }

        if (ulRet==0)//该设备不属于该总线
            continue;

        //配置通道命令数据区
        memset(&m_pChData[pDeviceInfo->nBusNO].sLastSetCmdData, 0, sizeof(CAN_SET_CMD_DATA));
        m_pChData[pDeviceInfo->nBusNO].sLastSetCmdData.uCmdType = DEV_CMD_SET_NO;
        m_pChData[pDeviceInfo->nBusNO].sLastSetCmdData.uDevNo = pDeviceConfig->nDeviceNO;
        m_pChData[pDeviceInfo->nBusNO].sLastSetCmdData.uExeStep = CMD_EXE_STEP_WAIT_RES;
        m_pChData[pDeviceInfo->nBusNO].sLastSetCmdData.ulExeRes = DVM_ERR_SUCCESS;

        //配置设置指令ID
        memset(&sSetCmdPack, 0, sizeof(MAINCTRL_COM_DATA_PACK));
        sSetCmdPack.ID.CmdID.ulMsgType = CAN_MSG_TYPE_CMD;
        sSetCmdPack.ID.CmdID.ulCmdType = DEV_CMD_SET_NO;
        sSetCmdPack.ID.CmdID.ulReceiveObj = CAN_DEVICE_RECEIVE;
        sSetCmdPack.ID.CmdID.ulDevComNO = pComNo[pDeviceInfo->nBusNO];

        //配置设置指令数据
        sSetCmdPack.Data.CmdData.SetNO.ulID = pDeviceConfig->nDevieID;
        sSetCmdPack.Data.CmdData.SetNO.nDeviceNO = pDeviceConfig->nDeviceNO;
#ifdef ENABLE_EX_PULSE_BIT
        sSetCmdPack.Data.CmdData.SetNO.uAxisCount = m_pChData[pDeviceInfo->nBusNO].uMotorDevCount;
        sSetCmdPack.Data.CmdData.SetNO.uPulseBitLen = m_pChData[pDeviceInfo->nBusNO].uPulseBitLen;
#endif

        //发送配置数据包
        ulRet = SendSinglePack(pDeviceInfo->nBusNO, &sSetCmdPack);
        if (ulRet!=DVM_ERR_SUCCESS)
            break;

        pDeviceInfo->nDeviceNO = pDeviceConfig->nDeviceNO;                                                //修改设备信息数据区设备编号
        m_pChData[pDeviceInfo->nBusNO].pDevInfo[pComNo[pDeviceInfo->nBusNO]] = pDeviceInfo;        //设置相应通信编号位置对应的设备信息地址
        m_pComNoMap[pDeviceInfo->nDeviceNO].uBusChIndex = pDeviceInfo->nBusNO;                //保存设备的总线序号
        m_pComNoMap[pDeviceInfo->nDeviceNO].uComNo = pComNo[pDeviceInfo->nBusNO];        //保存设备的通信编号
        pComNo[pDeviceInfo->nBusNO] += 1;
        m_pChData[pDeviceInfo->nBusNO].uCodedDevCount++;

        //处理设置指令回包，如果在规定时间内没有等到回包，则设置出错退出
        ulRet = RcvProcess(pDeviceInfo->nBusNO, CAN_FLASH_TIME_OUT);
        if (ulRet!=DVM_ERR_SUCCESS)
            break;
        
        //可能收到两个
        if (m_pChData[pDeviceInfo->nBusNO].sLastSetCmdData.uExeStep != CMD_EXE_STEP_END)
        {
                ulRet = DVM_ERR_RCV_TIMEOUT;
                break;
        }

        if (m_pChData[pDeviceInfo->nBusNO].sLastSetCmdData.ulExeRes!=DVM_ERR_SUCCESS)
        {
            ulRet = m_pChData[pDeviceInfo->nBusNO].sLastSetCmdData.ulExeRes;
            break;
        }

        if (pDeviceInfo->nDeviceType==DEV_TYPE_AXIS || pDeviceInfo->nDeviceType==DEV_TYPE_SV)
        {
            m_pCanDeviceState[pDeviceInfo->nDeviceNO].AxisStateData.ulRunningStatus |= AXIS_STATUS_ZERO_SPEED|AXIS_STATUS_FSTOP;
            //测试 -------------------------------------
            //m_pCanDeviceState[pDeviceInfo->nDeviceNO].AxisStateData.ulSvAlarm=0;
            // -------------------------------------
        }
    }

    //设置硬件配置信息成功
    if (ulRet==DVM_ERR_SUCCESS)
    {
        //更新电机参数
        for (uBusChIndex=0; uBusChIndex<CAN_CHANNEL_MAX_COUNT; uBusChIndex++)
        {
            if (m_pChData[uBusChIndex].uCodedDevCount==0)
                continue;

            for(uComNo=0; uComNo<m_pChData[uBusChIndex].uCodedDevCount; uComNo++)
            {
                pDeviceInfo = m_pChData[uBusChIndex].pDevInfo[uComNo];

                if (pDeviceInfo->nDeviceType==DEV_TYPE_AXIS || pDeviceInfo->nDeviceType==DEV_TYPE_SV)
                {
                    //更新轴控参数
                    pAxisParm = m_pCanDevAxisParm+pDeviceInfo->nDeviceNO;
                    pDevState = m_pCanDeviceState + pDeviceInfo->nDeviceNO;

                    ulRet = SendAxisParm(m_pComNoMap[pDeviceInfo->nDeviceNO], (uBit8*)pAxisParm, sizeof(AXIS_PARM));
                    
                    if (ulRet!=DVM_ERR_SUCCESS)
                        break;

                    //重新设置指令位置
                    pDevState->AxisStateData.dCmdPos = pDevState->AxisStateData.lQeiPos*m_pChData[uBusChIndex].pAxisScaleRatio[uComNo];
                    pDevState->AxisStateData.dPulsePos = pDevState->AxisStateData.dCmdPos*pAxisParm->AxisScaleParm.lGearRatioPlusScale/pAxisParm->AxisScaleParm.lGearRatioPosScale;

                    //更新系统控制参数
                    ulRet = SendSysCtrlParm(m_pComNoMap[pDeviceInfo->nDeviceNO], (uBit8*)m_pCanSysCtrlParm,sizeof(SYS_CTRL_PARM));
                    if (ulRet!=DVM_ERR_SUCCESS)
                        break;
                }
            }

            if (ulRet!=DVM_ERR_SUCCESS)
                break;
        }
    }

    if (ulRet != DVM_ERR_SUCCESS)
    {
        SetBusDevNoToDefault();
        SetConfigDataToDefault();
    }

    //释放收发权限
    for (uBusChIndex=0; uBusChIndex<CAN_CHANNEL_MAX_COUNT; uBusChIndex++)
        ReleaseRsPermit(uBusChIndex);

#ifdef SYS_FUN_TEST
    GPIO_FunTestEnd(DVM_CAN_DEV_SetConfig);
#endif

#ifdef RS_MONITOR_ENALBE
    memset(&m_sCanBusRsMsData, 0, sizeof(BUS_RS_MS_DATA));
#endif

    return ulRet;
}

/*
函 数 名：CAN_DEV_SetAxisParmReadAddr
功    能：设置轴参数读取起始地址
参    数：pAxisParm --轴参数读取起始地址
返 回 值：无
调用位置：
注意事项：发送轴进给指令前必须先设置轴参数
*/
void CAN_DEV_SetAxisParmReadAddr(const AXIS_PARM* pAxisParm)
{
    m_pCanDevAxisParm = pAxisParm;
}

/*
函 数 名：CAN_DEV_SetSysCtrlParmReadAddr
功    能：设置系统控制参数读取起始地址
参    数：pSysCtrlParm --系统控制参数读取起始地址
返 回 值：无
调用位置：
注意事项：发送轴进给指令前必须先设置轴参数
*/
void CAN_DEV_SetSysCtrlParmReadAddr(const SYS_CTRL_PARM* pSysCtrlParm)
{
    m_pCanSysCtrlParm = pSysCtrlParm;
}

/*
函 数 名：CAN_DEV_SetDeviceInfoAddr
功    能：设置设备信息数据的存储地址
参    数：pDeviceInfo    --设备信息数据存储起始地址
          ulLen            --设备信息数据缓冲区长度（表示之前已经扫描到的有效设备信息数量）
返 回 值：无
调用位置：
注意事项：
*/
void CAN_DEV_SetDeviceInfoAddr(DEVICE_INFO* pDeviceInfo, uBit32 ulLen)
{
    m_pCanDeviceInfo = pDeviceInfo;
    m_uCanExsitDevCount = ulLen;
}

/*
函 数 名：CAN_DEV_SetDeviceStateAddr
功    能：设置设备状态数据的存储地址
参    数：pDeviceState --设备状态数据存储起始地址
          ulLen --设备状态数据缓冲区长度（表示可存放多少个设备的状态数据）
返 回 值：无
调用位置：
注意事项：
*/
void CAN_DEV_SetDeviceStateAddr(DEVICE_STATE* pDeviceState, uBit32 ulLen)
{
    m_pCanDeviceState = pDeviceState;
    //m_uMaxDevStateCount = ulLen;
}

//-------------------------------------------获取硬件信息---------------------------------------------------
/*
函 数 名：CAN_DEV_Scan
功    能：扫描总线上的设备
参    数：
          pDeviceInfo            --设备硬件信息存储缓冲区起始地址
          ulDevBufLen            --设备硬件信息存储缓冲区长度（表示缓存区最大可以存放多少个设备的硬件信息）
          *pRecordDevCount --已经记录到硬件信息缓冲区的设备数量（出参）
          *pTotalDevCount    --扫描发现的设备总数量（出参）
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 CAN_DEV_Scan(DEVICE_INFO* pDeviceInfo, uBit32 ulDevBufLen, uBit32* pRecordDevCount, uBit32* pTotalDevCount)
{
    uBit32 ulRet;
    uBit8 uBusChIndex;
    double dWaitTime;
    CanFrame sPackData;
    uBit32 ulRecordDevCount=0;                
    uBit32 ulTotalDevCount=0;
    DEV_CMD_ANS_PACK_ID  *pDevAnsPackID;
    DEV_CMD_ANS_PACK_DATA *pDevAnsPack;
    DEVICE_INFO              *pCurDevInfo;
    MAINCTRL_COM_DATA_PACK sSetCmdPack;
    
#ifdef ENABLE_EX_PULSE_BIT
    uBit32 ulTmp;
    uBit8 uChDevCount[CAN_CHANNEL_MAX_COUNT]={0};
#endif

#ifdef SYS_FUN_TEST
    GPIO_FunTestStart(DVM_CAN_DEV_Scan);
#endif

    *pRecordDevCount=0;
    *pTotalDevCount=0;
    
    //检查总线空闲
    if (CAN_DEV_IsBusFree()==0)
        return DVM_ERR_BUS_BUSY;
    
    pCurDevInfo = pDeviceInfo;
    memset(&sSetCmdPack, 0 , sizeof(MAINCTRL_COM_DATA_PACK));

    //配置设置指令ID

    sSetCmdPack.ID.CmdID.ulMsgType = CAN_MSG_TYPE_CMD;
    sSetCmdPack.ID.CmdID.ulCmdType = DEV_CMD_INQ_INFO;
    sSetCmdPack.ID.CmdID.ulReceiveObj = CAN_DEVICE_RECEIVE;

    for (uBusChIndex=0; uBusChIndex<CAN_CHANNEL_MAX_COUNT; uBusChIndex++)
    {
        if (AskForRsPermit(uBusChIndex)!=DVM_ERR_SUCCESS)
            continue;

#ifdef SYS_FUN_TEST
        GPIO_FunTestStart(DVM_CAN_DEV_SingleCh);
#endif

        //收光总线通道上的回应包
        //RcvProcess(uBusChIndex, CAN_GETRES_MAX_TIME);

        //发送获取设备信息广播包---------------------------------
        ulRet = SendSinglePack(uBusChIndex, &sSetCmdPack);
        
        if (ulRet != DVM_ERR_SUCCESS)
        {
            ReleaseRsPermit(uBusChIndex);
            continue;
            //return DVM_ERR_CMD_SEND;
        }
        //----------------------------------------------------------

#ifdef ENABLE_EX_PULSE_BIT
        m_pChData[uBusChIndex].uMotorDevCount=0;
        m_pChData[uBusChIndex].uPulseBitLen=0;
        m_pChData[uBusChIndex].ulPulseBitMask=0;
        uChDevCount[uBusChIndex]=0;
#endif
        //接收硬件信息----------------------------------------------
        dWaitTime = m_sCanDevExInterface.pf_SYS_GetSysTickCount();

        while(1)
        {
            m_sCanComInterface.pf_CAN_RcvHandle(uBusChIndex);

            if (m_sCanComInterface.pf_CAN_GetFrame(uBusChIndex, &sPackData) != CAN_ERR_SUCCESS)//未收到数据包
            {
                if (m_sCanDevExInterface.pf_SYS_GetSysTickCount()-dWaitTime>=CAN_SCAN_TIME_OUT)
                    break;    
            }else//接收到数据包                                    
            {
                pDevAnsPackID = (DEV_CMD_ANS_PACK_ID*)&sPackData.ulID;

                if (pDevAnsPackID->ulMsgType != CAN_MSG_TYPE_CMD||pDevAnsPackID->ulCmdType != DEV_CMD_INQ_INFO)
                {    //非查询设备信息指令回应包
                    DealWithRcvPack(uBusChIndex, &sPackData);
                }else
                {
                    pDevAnsPack = (DEV_CMD_ANS_PACK_DATA*)sPackData.ucDatBuf;

                    //查找该ID设备是否已经记录,如果已经记录则忽略
                    //if (IsDevExist(pDevAnsPack->InqInfoAxisAnsData.ulID, pDeviceInfo, ulRecordDevCount))
                    //    continue;

                    ulRet = DVM_ERR_SUCCESS;

                    switch(pDevAnsPackID->ulDevType)
                    {
                    case DEV_TYPE_AXIS:                    //轴卡
                    case DEV_TYPE_SV:                        //伺服设备
                        pCurDevInfo->nBusType = DEV_BUS_TYPE_CAN;
                        pCurDevInfo->nBusNO = uBusChIndex;
                        pCurDevInfo->nDeviceType = pDevAnsPackID->ulDevType;
                        pCurDevInfo->nDevieID = pDevAnsPack->InqInfoAxisAnsData.ulID;
                        pCurDevInfo->nDeviceNO = pDevAnsPack->InqInfoAxisAnsData.ulDevNO;
                        //pCurDevInfo->nDeviceNOLast = pDevAnsPack->InqInfoAxisAnsData.ulDevNO;
#ifdef ENABLE_EX_PULSE_BIT
                        m_pChData[uBusChIndex].uMotorDevCount++;
#endif
                        break;
                    case DEV_TYPE_OUTPUT:                //输出卡
                    case DEV_TYPE_INPUT:                    //输入卡
                        pCurDevInfo->nBusType = DEV_BUS_TYPE_CAN;
                        pCurDevInfo->nBusNO = uBusChIndex;
                        pCurDevInfo->nDeviceType = pDevAnsPackID->ulDevType;
                        pCurDevInfo->nDevieID = pDevAnsPack->InaInfoIOAnsData.ulID;
                        pCurDevInfo->nDeviceNO = pDevAnsPack->InaInfoIOAnsData.uDevNo;
                        //pCurDevInfo->nDeviceNOLast = pDevAnsPack->InaInfoIOAnsData.uDevNo;
                        
                        pCurDevInfo->IOResource.nInputCount = (pDevAnsPack->InaInfoIOAnsData.uCountInfo&0x0F)*8;
                        pCurDevInfo->IOResource.nOutputCount = ((pDevAnsPack->InaInfoIOAnsData.uCountInfo>>4)&0x0F)*8;
                        pCurDevInfo->IOResource.nPWMOutPutStartPos = pDevAnsPack->InaInfoIOAnsData.nPWMOutPutStartPos;
                        pCurDevInfo->IOResource.ulPWMOutputMask = pDevAnsPack->InaInfoIOAnsData.ulPWMOutputMask;
                        break;
                    case DEV_TYPE_ADDA:                    //AD/DA卡
                        pCurDevInfo->nBusType = DEV_BUS_TYPE_CAN;
                        pCurDevInfo->nBusNO = uBusChIndex;
                        pCurDevInfo->nDeviceType = pDevAnsPackID->ulDevType;
                        pCurDevInfo->nDevieID = pDevAnsPack->InaInfoIOAnsData.ulID;
                        pCurDevInfo->nDeviceNO = pDevAnsPack->InaInfoIOAnsData.uDevNo;
                        //pCurDevInfo->nDeviceNOLast = pDevAnsPack->InaInfoIOAnsData.uDevNo; 
                        pCurDevInfo->ADDAResource.nADCount = (pDevAnsPack->InaInfoADDAAnsData.uCountInfo&0x0F);
                        pCurDevInfo->ADDAResource.nDACount = ((pDevAnsPack->InaInfoADDAAnsData.uCountInfo>>4)&0x0F);
                        pCurDevInfo->ADDAResource.nADBitLen = pDevAnsPack->InaInfoADDAAnsData.nADBitLen;
                        pCurDevInfo->ADDAResource.nDABitLen = pDevAnsPack->InaInfoADDAAnsData.nDABitLen;
                        break;
                    default:
                        ulRet = DVM_ERR_UNKNOW_DEV;//无法识别的设备，不处理
                        break;
                    }

                    if (ulRet==DVM_ERR_UNKNOW_DEV)
                        continue;

#ifdef ENABLE_EX_PULSE_BIT
                    uChDevCount[uBusChIndex]++;
#endif
                    ulTotalDevCount++;

                    if (ulRecordDevCount<ulDevBufLen)//缓冲区够用
                    {
                        ulRecordDevCount++;
                        pCurDevInfo++;
                    }

                    //检查该ID设备是否已经在设备管理层设备信息数据区
                    //if (IsDevExist(pDevAnsPack->InqInfoAxisAnsData.ulID, m_pCanDeviceInfo, m_uCanExsitDevCount))
                    //    ulRecordDevCount++;

                    dWaitTime = m_sCanDevExInterface.pf_SYS_GetSysTickCount();//重新计时
                }
            }
        }

#ifdef SYS_FUN_TEST
        GPIO_FunTestEnd(DVM_CAN_DEV_SingleCh);
#endif
        ReleaseRsPermit(uBusChIndex);
    }


#ifdef ENABLE_EX_PULSE_BIT
    //设置通道脉冲位宽数据
    for (uBusChIndex=0; uBusChIndex<CAN_CHANNEL_MAX_COUNT; uBusChIndex++)
    {
        m_pChData[uBusChIndex].uPulseBitLen=8;
        ulTmp = CAN_DEV_MAX_COUNT-uChDevCount[uBusChIndex];
        

        if (ulTmp && m_pChData[uBusChIndex].uMotorDevCount)//通道未插满设备且有轴设备，则平均分配空闲数据位
        {
            ulTmp *= 8;
            ulTmp /= m_pChData[uBusChIndex].uMotorDevCount;
            m_pChData[uBusChIndex].uPulseBitLen += ulTmp;

            if (m_pChData[uBusChIndex].uPulseBitLen>30)//最大30位,加上ID2位数据位，则总共32位，含符号位
                m_pChData[uBusChIndex].uPulseBitLen=30;
        }

        //计数脉冲数据屏蔽码
        m_pChData[uBusChIndex].ulPulseBitMask = (1<<m_pChData[uBusChIndex].uPulseBitLen)-1;    
        m_pChData[uBusChIndex].ulMaxPulseInc = (1<<m_pChData[uBusChIndex].uPulseBitLen+1)-1;    
    }
#endif

    *pRecordDevCount = ulRecordDevCount;
    *pTotalDevCount  = ulTotalDevCount;
#ifdef SYS_FUN_TEST
    GPIO_FunTestEnd(DVM_CAN_DEV_Scan);
#endif
    return DVM_ERR_SUCCESS;
}

//-------------------------------------------执行控制指令---------------------------------------------------

/*
函 数 名：pf_Axis_SendFeed
功    能：设置轴进给
参    数：ulAxisNO --轴编号(设备编号)
                IPOAxisState --插补状态数据
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 CAN_DEV_SendFeed(uBit32 ulAxisNO, IPO_AXIS_STATE_DATA IPOAxisState)
{
    DEVICE_STATE *pAxisState;

#ifdef ENABLE_EX_PULSE_BIT
    uBit8 uBusChIndex=m_pComNoMap[ulAxisNO].uBusChIndex;
#endif

#ifdef SYS_FUN_TEST
    GPIO_FunTestStart(DVM_CAN_SetFeed);
#endif

    pAxisState = m_pCanDeviceState + ulAxisNO;
    pAxisState->AxisStateData.dCmdPos = IPOAxisState.dCmdPos;
    pAxisState->AxisStateData.dPulsePos = IPOAxisState.dPulsePos;
    pAxisState->AxisStateData.dCmdSpeed = IPOAxisState.dCmdSpeed;
    pAxisState->AxisStateData.dAcc = IPOAxisState.dAcc;
    pAxisState->AxisStateData.lPinc = IPOAxisState.lPinc;
    m_pChData[m_pComNoMap[ulAxisNO].uBusChIndex].pPulseCtrlWords[m_pComNoMap[ulAxisNO].uComNo].nFeedFlag=2;//两个插补周期后才更新速度加速度

    if (pAxisState->AxisStateData.ulRunningStatus&AXIS_STATUS_VERIFY)//校验模式
        pAxisState->AxisStateData.lPinc = 0;

#ifdef ENABLE_EX_PULSE_BIT
    if (IPOAxisState.lPinc<0)
        IPOAxisState.lPinc *=-1;

    //脉冲超限返回错误，单次脉冲最大数为511个
    if (IPOAxisState.lPinc > m_pChData[uBusChIndex].ulMaxPulseInc)
    {
        pAxisState->AxisStateData.lPinc = 0;
        pAxisState->AxisStateData.ulRunningStatus |= AXIS_STATUS_FEED_OUT_OF_RANGE;//超限置位
        return DVM_ERR_PULSE_RANGE;
    }
#else
    //脉冲超限返回错误，单次脉冲最大数为511个
    if (IPOAxisState.lPinc<0)
        IPOAxisState.lPinc *=-1;

    if (IPOAxisState.lPinc  > MAX_PULSE_INC)
    {
        pAxisState->AxisStateData.lPinc = 0;
        pAxisState->AxisStateData.ulRunningStatus |= AXIS_STATUS_FEED_OUT_OF_RANGE;//超限置位
        return DVM_ERR_PULSE_RANGE;
    }
#endif


    pAxisState->AxisStateData.ulRunningStatus &= ~AXIS_STATUS_ZERO_SPEED;
    pAxisState->AxisStateData.ulRunningStatus &= ~AXIS_STATUS_FSTOP;
#ifdef SYS_FUN_TEST
    GPIO_FunTestEnd(DVM_CAN_SetFeed);
#endif
    return DVM_ERR_SUCCESS;
}

/*
函 数 名：CAN_DEV_SetCmd
功    能：设置设备控制指令
参    数：ulDevNO --设备编号
          ulCmdType --指令类型，取值为：
                        DEV_CMD_INQ_INFO                    (0)    //查询设备信息
                        DEV_CMD_SET_NO                    (1)    //设置设备编号
                        DEV_CMD_BLINK_LED                (2)    //设置LED闪烁（设备测试）
                        DEV_CMD_INQ_VER                    (3)    //查询版本信息
                        DEV_CMD_CLR_ALM                    (4)    //报警清除
                        DEV_CMD_ENABLE_SV                (5)    //使能电机
                        DEV_CMD_SET_HOME                (6)    //回零
                        DEV_CMD_SET_LOCK                    (7)    //轴锁定(表示轴正在进行插补运动) （待完善）
                        DEV_CMD_RESET                        (8)    //系统复位
                        DEV_CMD_SET_CMD_POS            (9)    //设置指令位置
                        DEV_CMD_SET_QEI_POS            (10)    //设置编码器位置
                        DEV_CMD_SET_VERIFY_MODE        (11)    //设置进入校验模式
                        DEV_CMD_SET_IO_PWM_MODE    (12)    //设置IO进入PWM模式
                        DEV_CMD_SET_IO_PWM_DUTY    (13)    //设置IO进入PWM占空比
                        DEV_CMD_SET_IO_PWM_FREQ    (14)    //设置IO进入PWM频率
                        DEV_CMD_SET_FEED                    (15)    //设置进给
                        DEV_CMD_SET_OUTPUT                (16)    //设置IO输出
                        DEV_CMD_SET_FSTP                    (17)   //CCW驱动禁止                              
                        DEV_CMD_SET_RSTP                    (18)   //CW驱动禁止                                
                        DEV_CMD_SET_CLESC1ZERO        (19)   //偏差计算清零/速度选择1/零速箝位      
                        DEV_CMD_SET_INH_SC2            (20)   //指令脉冲禁止/速度选择2              
                        DEV_CMD_SET_FIL                    (21)   //CCW转矩限制                            
                        DEV_CMD_SET_RIL                    (22)   //CW转矩限制 
                        DEV_CMD_INQ_SV_ALM                (23)   //查询伺服报警
                        DEV_CMD_SET_IPO_CYCLE            (24)   //设置插补周期
                        DEV_CMD_GET_SV_PARM            (25)    //获取伺服参数

        lCmdPara1，lCmdPara2 --控制参数，对应各控制指令其意义为：
                        DEV_CMD_BLINK_LED                --lCmdPara1：设备ID；lCmdPara2：无效
                        DEV_CMD_INQ_VER                    --lCmdPara1：无效；lCmdPara2：无效
                        DEV_CMD_CLR_ALM                    --lCmdPara1：无效；lCmdPara2：无效
                        DEV_CMD_ENABLE_SV                --lCmdPara1：0-断使能，1-使能；lCmdPara2：无效
                        DEV_CMD_SET_HOME                --lCmdPara1：0-回零停止，1-回零开始；lCmdPara2：无效
                        DEV_CMD_SET_LOCK                    --lCmdPara1：0-解除锁定，1-锁定；lCmdPara2：无效
                        DEV_CMD_RESET                        --lCmdPara1：无效；lCmdPara2：无效
                        DEV_CMD_SET_CMD_POS            --lCmdPara1：指令位置；lCmdPara2：无效
                        DEV_CMD_SET_QEI_POS            --lCmdPara1：编码器位置；lCmdPara2：无效
                        DEV_CMD_SET_VERIFY_MODE        --lCmdPara1：0-关闭校验模式，1-打开校验模式；lCmdPara2：无效
                        DEV_CMD_SET_IO_PWM_MODE    --lCmdPara1：要设置PWM模式的IO点编号；lCmdPara2：0-关闭PWM模式，1-开启PWM模式；
                        DEV_CMD_SET_IO_PWM_DUTY    --lCmdPara1：IO点编号；lCmdPara2：占空比
                        DEV_CMD_SET_IO_PWM_FREQ    --lCmdPara1：IO点编号；lCmdPara2：频率
                        DEV_CMD_SET_OUTPUT                --当为OUTPUT输出设备时，lCmdPara1：输出状态值；lCmdPara2：输出位掩码
                                                                    --当为DA输出设备时，lCmdPara1：DA输出值；lCmdPara2：DA通道编号
                        DEV_CMD_SET_FSTP                    --lCmdPara1：0-CCW驱动禁止关闭，1-CCW驱动禁止打开；lCmdPara2：无效
                        DEV_CMD_SET_RSTP                    --lCmdPara1：0-CW驱动禁止关闭，1-CW驱动禁止打开；lCmdPara2：无效
                        DEV_CMD_SET_CLESC1ZERO        --lCmdPara1：无效；lCmdPara2：无效
                        DEV_CMD_SET_INH_SC2            --lCmdPara1：0-关闭，1-开启；lCmdPara2：无效
                        DEV_CMD_SET_FIL                    --lCmdPara1：0-关闭，1-开启；lCmdPara2：无效
                        DEV_CMD_SET_RIL                    --lCmdPara1：0-关闭，1-开启；lCmdPara2：无效
                        DEV_CMD_INQ_SV_ALM                --lCmdPara1：无效；lCmdPara2：无效
                        DEV_CMD_GET_SV_PARM            --lCmdPara1：要获取的伺服参数编号 lCmdPara2:无效

        iWaitExeResult --标记是否等待执行结果，0-不等待，1-等待
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：内部限制同一总线上的设备同一时间只有一条控制指令在执行,如果是设置IO输出，则直接设置设备状态信息中的指令输出后
          返回，如果是设置ADDA的DA输出，则先判断该是否正在设置DA值，如果是则设置指令位置后清除DA的脉冲输出控制
*/
uBit32 CAN_DEV_SetCmd(uBit32 ulDevNO, uBit32 ulCmdType, Bit32 lCmdPara1, Bit32 lCmdPara2, Bit32 iWaitExeResult)
{
    uBit32 ulRet;
    uBit32 ulBusChIndex;
    DEVICE_INFO *pDevInfo;
    DEVICE_STATE *pDevState;
    MAINCTRL_COM_DATA_PACK sSendPack={0};
    const AXIS_PARM *pAxisParm;

#ifdef SYS_FUN_TEST
    GPIO_FunTestStart(DVM_CAN_DEV_SetCmd);
#endif

    //如果是IO和ADDA输出状态指令，则更新设备状态数据区数据后直接返回
    if (ulCmdType == DEV_CMD_SET_OUTPUT)
        return HandleOutputCmd(ulDevNO, lCmdPara1, lCmdPara2);

    //如果是测试设备指令(设备编号无效,总线通道序号由上层接口提供)
    if (ulCmdType == DEV_CMD_BLINK_LED)
        return HandleBlinkLedCmd(lCmdPara1, lCmdPara2);

    //如果是校验模式
    if (ulCmdType == DEV_CMD_SET_VERIFY_MODE)
        return HandleVerifyCmd(ulDevNO, lCmdPara1);

    ulBusChIndex = m_pComNoMap[ulDevNO].uBusChIndex;

    //检查通道上一条指令执行状态，如果未执行完则不能再发送设置指令
    if (m_pChData[ulBusChIndex].sLastSetCmdData.uExeStep != CMD_EXE_STEP_END)
    {
#ifdef SYS_FUN_TEST
        GPIO_FunTestEnd(DVM_CAN_DEV_SetCmd);
#endif
        return DVM_ERR_BUS_BUSY;
    }

    //检查设备是否锁定,锁定设备不允许再发设置指令
    if (IsDevLocked(ulBusChIndex,  m_pComNoMap[ulDevNO].uComNo))
    {
#ifdef SYS_FUN_TEST
        GPIO_FunTestEnd(DVM_CAN_DEV_SetCmd);
#endif
        return DVM_ERR_BUS_BUSY;
    }

    //请求发送权限
    if (AskForRsPermit(ulBusChIndex) != DVM_ERR_SUCCESS)
    {
        //请求不成功，说明总线忙，但设备未锁定，执行发送附加指令
        return HandleAddCmd(ulDevNO, ulCmdType, lCmdPara1, lCmdPara2, iWaitExeResult);
    }
        
    //配置通道设置命令数据区
    memset(&m_pChData[ulBusChIndex].sLastSetCmdData, 0, sizeof(CAN_SET_CMD_DATA));
    m_pChData[ulBusChIndex].sLastSetCmdData.uCmdType = ulCmdType;
    m_pChData[ulBusChIndex].sLastSetCmdData.uDevNo = ulDevNO;
    m_pChData[ulBusChIndex].sLastSetCmdData.uExeStep = CMD_EXE_STEP_END;
    m_pChData[ulBusChIndex].sLastSetCmdData.ulExeRes = DVM_ERR_BUS_BUSY;

    //设置轴的指令位置
    if (ulCmdType==DEV_CMD_SET_CMD_POS)
    {
        pDevState = m_pCanDeviceState + ulDevNO;
        pDevInfo = m_pChData[ulBusChIndex].pDevInfo[m_pComNoMap[ulDevNO].uComNo];
        m_pChData[ulBusChIndex].sLastSetCmdData.ulExeRes = DVM_ERR_SUCCESS;

        if (pDevInfo->nDeviceType!=DEV_TYPE_AXIS &&
            pDevInfo->nDeviceType!=DEV_TYPE_SV )//非轴设备的设置指令位置
        {
            ReleaseRsPermit(ulBusChIndex);//释放发送权限
            m_pChData[ulBusChIndex].sLastSetCmdData.ulExeRes = DVM_ERR_UNKNOW_CMD;
#ifdef SYS_FUN_TEST
            GPIO_FunTestEnd(DVM_CAN_DEV_SetCmd);
#endif
            return DVM_ERR_UNKNOW_CMD;
        }

        //修改轴指令位置
        pAxisParm = m_pCanDevAxisParm + ulDevNO;
        pDevState->AxisStateData.dCmdPos = lCmdPara1;
        //修改轴脉冲指令位置
        pDevState->AxisStateData.dPulsePos= lCmdPara1 * pAxisParm->AxisScaleParm.lGearRatioPlusScale / pAxisParm->AxisScaleParm.lGearRatioPosScale;
        ReleaseRsPermit(ulBusChIndex);//释放发送权限
#ifdef SYS_FUN_TEST
        GPIO_FunTestEnd(DVM_CAN_DEV_SetCmd);
#endif
        return DVM_ERR_SUCCESS;
    }    

    //配置发送ID
    sSendPack.ID.CmdID.ulMsgType = CAN_MSG_TYPE_CMD;    //
    sSendPack.ID.CmdID.ulCmdType = ulCmdType;
    sSendPack.ID.CmdID.ulDevComNO = m_pComNoMap[ulDevNO].uComNo;
//        sSendPack.ID.CmdID.ulReceiveObj = CAN_DEVICE_RECEIVE;
    
    //配置发送数据
    memcpy(&sSendPack.Data.ulDataBuf[0], &lCmdPara1, sizeof(Bit32));
    memcpy(&sSendPack.Data.ulDataBuf[1], &lCmdPara2, sizeof(Bit32));

    //发送命令数据包
    ulRet = SendSinglePack(ulBusChIndex, &sSendPack);
    m_pChData[ulBusChIndex].sLastSetCmdData.ulExeRes = ulRet;
    m_pChData[ulBusChIndex].sLastSetCmdData.dStartTick = m_sCanDevExInterface.pf_SYS_GetSysTickCount();

    //指令发送失败
    if (ulRet == DVM_ERR_SUCCESS)
    {
        if (iWaitExeResult==0)//如果不等待回应包
        {
            m_pChData[ulBusChIndex].sLastSetCmdData.uExeStep = CMD_EXE_STEP_WAIT_RES;
        }else    //等待回应包
        {
            ulRet = RcvProcess(ulBusChIndex, CAN_GETRES_MAX_TIME);
        }
    }

    //释放发送权限
    ReleaseRsPermit(ulBusChIndex);
    return ulRet;
}

/*
函 数 名：CAN_DEV_CheckFree
功    能：检测设备是否空闲（没有指令在执行）
参    数：ulDevNO --设备编号
返 回 值：空闲返回1，否则返回0
调用位置：
注意事项：
*/
uBit32 CAN_DEV_CheckFree(uBit32 ulDevNO)
{
    //检查总线是否有指令正在执行
    if (m_pChData[m_pComNoMap[ulDevNO].uBusChIndex].sLastSetCmdData.uExeStep != CMD_EXE_STEP_END)
        return 0;

    //检查设备是否锁定
    if (m_pChData[m_pComNoMap[ulDevNO].uBusChIndex].uDevLockBit && (0x01<<m_pComNoMap[ulDevNO].uComNo))
        return 0;
    
    return 1;
}

/*
函 数 名：CAN_DEV_IsBusFree
功    能：检查总线是否空闲(总线所有有效通道都空闲，且没有要等待的指令回包，没有正在发生数据)
参    数：
返 回 值：空闲返回1，否则返回0
调用位置：
注意事项：
*/
uBit32 CAN_DEV_IsBusFree(void)        
{
    uBit8 uBusChIndex;

    for (uBusChIndex=0; uBusChIndex<CAN_CHANNEL_MAX_COUNT; uBusChIndex++)
    {
        if (m_pChData[uBusChIndex].bValidFlag==1)//通道有效
        {
            if (m_pChData[uBusChIndex].uDevLockBit !=0 ||        //有设备锁定或有指令未执行完成
                m_pChData[uBusChIndex].sLastSetCmdData.uExeStep != CMD_EXE_STEP_END||
                m_pChData[uBusChIndex].bRsAllowFlag ==1)
            {
                return 0;
            }
        }
    }

    return 1;
}

/*
函 数 名：CAN_DEV_LockForIPO
功    能：锁定(解锁)总线设备
参    数：
        ulDevNo            ---设备编号
        ulLockFlag        ---锁定标志 0-解锁 1-锁定
        ulWaitExeResult    ---是否获取回应包再返回 0-立即返回 1-获取回应包再返回
返 回 值：空闲返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 CAN_DEV_LockForIPO(uBit32 ulDevNo, uBit32 ulLockFlag, uBit32 ulWaitExeResult)
{
    if (ulLockFlag==1)    //锁定    
    {
        //该设备正在等待指令回应包或发送附加指令,不能被锁定
        if(ulDevNo == m_pChData[m_pComNoMap[ulDevNo].uBusChIndex].sLastSetCmdData.uDevNo &&
           m_pChData[m_pComNoMap[ulDevNo].uBusChIndex].sLastSetCmdData.uExeStep != CMD_EXE_STEP_END)
        {
            return DVM_ERR_BUS_BUSY;  
        }
        
        m_pChData[m_pComNoMap[ulDevNo].uBusChIndex].uDevLockBit |= (0x01<<m_pComNoMap[ulDevNo].uComNo);
    }
    else    //解锁    
    {
        m_pChData[m_pComNoMap[ulDevNo].uBusChIndex].uDevLockBit &= ~(0x01<<m_pComNoMap[ulDevNo].uComNo);
    }

    
    return DVM_ERR_SUCCESS;
}

/*
函 数 名：CAN_DEV_CheckLastCmdExeState
功    能：检测上一次的指令执行状态
参    数：ulDevNO --设备编号
          *pCmdType --指令类型（出参）
返 回 值：执行成功返回0，否则返回错误码（执行失败、正在执行）
调用位置：
注意事项：
*/
uBit32 CAN_DEV_CheckLastCmdExeState(uBit32 ulDevNO, uBit32* pCmdType)
{
    //if (m_pChData[m_pComNoMap[ulDevNO].uBusChIndex].sLastSetCmdData.uDevNo != ulDevNO)
    //    return DVM_ERR_DEVNO_DISMATCH;

    if (m_pChData[m_pComNoMap[ulDevNO].uBusChIndex].sLastSetCmdData.uExeStep!=CMD_EXE_STEP_END)//指令执行未结束
        return DVM_ERR_EXECUTTING;

    *pCmdType = m_pChData[m_pComNoMap[ulDevNO].uBusChIndex].sLastSetCmdData.uCmdType;
    return m_pChData[m_pComNoMap[ulDevNO].uBusChIndex].sLastSetCmdData.ulExeRes;
}

//-------------------------------------------设置参数---------------------------------------------------
/*
函 数 名：CAN_DEV_SetParm
功    能：设置设备参数
参    数：ulDevNO --设备编号
          ulParmType --参数类型，取值为为：
                        DEV_PARM_TYPE_SV                        //伺服参数
                        DEV_PARM_TYPE_UPDATE_APP                //更新应用程序数据
                        DEV_PARM_TYPE_UPDATE_BOOT                //更新BOOTLOADER数据
                        DEV_PARM_TYPE_AXIS_PARM                    //发送轴控参数
                        DEV_PARM_TYPE_SYS_CTRL_PARM                //发送系统控制参数
          pParmBuf --参数数据缓冲区
          ulParmLen --参数数据长度
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 CAN_DEV_SetParm(uBit32 ulDevNO, uBit32 ulParmType, uBit8* pParmBuf, uBit32 ulParmLen)
{
    uBit32 ulRet;
    AXIS_PARM *pAxisParm;
    DEVICE_STATE *pDevState;

    //检查通道是否空闲
    if (m_pChData[m_pComNoMap[ulDevNO].uBusChIndex].bValidFlag ==0 ||
        m_pChData[m_pComNoMap[ulDevNO].uBusChIndex].uDevLockBit)
    {
        return DVM_ERR_BUS_BUSY;
    }
    
    //请求发送权限
    ulRet = AskForRsPermit(m_pComNoMap[ulDevNO].uBusChIndex);
    if (ulRet != DVM_ERR_SUCCESS)
        return ulRet;


    switch(ulParmType)
    {
    case DEV_PARM_TYPE_SV:
        ulRet = SendSevorParm(m_pComNoMap[ulDevNO], pParmBuf, ulParmLen);
        break;
    case DEV_PARM_TYPE_UPDATE_APP:
        ulRet = SendAppUpdateData(m_pComNoMap[ulDevNO], pParmBuf, ulParmLen);
        break;
    case DEV_PARM_TYPE_UPDATE_BOOT:
        ulRet = SendBootUpdateData(m_pComNoMap[ulDevNO], pParmBuf, ulParmLen);
        break;
    case DEV_PARM_TYPE_AXIS_PARM:
        {
            ulRet = SendAxisParm(m_pComNoMap[ulDevNO], pParmBuf, ulParmLen);

            //重新计数指令位置
            if (ulRet==DVM_ERR_SUCCESS)
            {
                pAxisParm = (AXIS_PARM*)pParmBuf;
                pDevState = m_pCanDeviceState + ulDevNO;

                pDevState->AxisStateData.dCmdPos = pDevState->AxisStateData.lQeiPos*m_pChData[m_pComNoMap[ulDevNO].uBusChIndex].pAxisScaleRatio[m_pComNoMap[ulDevNO].uComNo];
                pDevState->AxisStateData.dPulsePos = pDevState->AxisStateData.dCmdPos*pAxisParm->AxisScaleParm.lGearRatioPlusScale/pAxisParm->AxisScaleParm.lGearRatioPosScale;
            }
        }
        break;
    case DEV_PARM_TYPE_SYS_CTRL_PARM:
        ulRet = SendSysCtrlParm(m_pComNoMap[ulDevNO], pParmBuf, ulParmLen);
        break;
    default:
        ulRet = DVM_ERR_UNKNOW_CMD;
        break;
    }
    
    ReleaseRsPermit(m_pComNoMap[ulDevNO].uBusChIndex);
    return ulRet;
}

//-------------------------------------------服务程序---------------------------------------------------
/*
函 数 名：CAN_DEV_SynCycleService
功    能：同步性周期服务函数
参    数：无
返 回 值：成功返回1，否则返回错误码
调用位置：在插补中断中调用
注意事项：
*/
uBit32 CAN_DEV_SynCycleService(void)
{
    uBit8 uBusChIndex;
    uBit32 ulRet = DVM_ERR_SUCCESS;
    uBit32 ulResult = DVM_ERR_SUCCESS;
    CanFrame sRcvPack;

#ifdef SYS_FUN_TEST
    GPIO_FunTestStart(DVM_CAN_DEV_SynCycleService);
#endif

    for (uBusChIndex=0; uBusChIndex<CAN_CHANNEL_MAX_COUNT; uBusChIndex++)
    {
        if (m_pChData[uBusChIndex].bValidFlag==0)//通道未启用
            continue;

        if (m_pChData[uBusChIndex].bRsAllowFlag==1)//外部调用
            continue;

        if (m_pChData[uBusChIndex].bRsAllowReqFlag==1)//外部请求发送权限，不再发送
            continue;

        if(m_pChData[uBusChIndex].sLastSetCmdData.uExeStep!=CMD_EXE_STEP_END &&
            m_pChData[uBusChIndex].sLastSetCmdData.uAddCmdType==CAN_ADD_CMD_NULL)//外部指令等待指令回应包
            continue;

        ulRet = SysCycleSend(uBusChIndex);        //配置并发送脉冲数据包

        if (ulRet)
            ulResult = ulRet;
    }

    for (uBusChIndex=0; uBusChIndex<CAN_CHANNEL_MAX_COUNT; uBusChIndex++)
    {
        if (m_pChData[uBusChIndex].bValidFlag==0)//通道未启用
            continue;

        if (m_pChData[uBusChIndex].bRsAllowFlag==1)//外部调用
            continue;

        if (m_pChData[uBusChIndex].bPreRsAllowFlag==1)//中断发送恢复，第一包数据只接收外部指令回应包
        {
            if(m_pChData[uBusChIndex].sLastSetCmdData.uExeStep!=CMD_EXE_STEP_END &&
                m_pChData[uBusChIndex].sLastSetCmdData.uAddCmdType==CAN_ADD_CMD_NULL)//外部指令等待指令回应包
            {
                m_sCanComInterface.pf_CAN_RcvHandle(uBusChIndex);

                if (m_sCanComInterface.pf_CAN_GetFrame(uBusChIndex, &sRcvPack)==CAN_ERR_SUCCESS)
                    DealWithRcvPack(uBusChIndex, &sRcvPack);
            }else
            {
                m_pChData[uBusChIndex].bPreRsAllowFlag = 0;
            }
        }else
        {
            if (m_pChData[uBusChIndex].uCodedDevCount)//无编号设备，则不接受指令回应包
            {
            //尽量收到上次的回应包并处理
                SynCycleRcv(uBusChIndex);                //通道接收数据包并处理
            }
        }
            
        SynCycleUpdateState(uBusChIndex);    //刷新设备状态

        if (m_pChData[uBusChIndex].bRsAllowReqFlag==1)//外部请求发送权限
        {
            m_pChData[uBusChIndex].bRsAllowReqFlag = 0;
            m_pChData[uBusChIndex].bRsAllowFlag = 1;
            m_pChData[uBusChIndex].bPreRsAllowFlag = 1;
        }
    }

#ifdef SYS_FUN_TEST
    GPIO_FunTestEnd(DVM_CAN_DEV_SynCycleService);
#endif
    return ulResult;
}


/*
函 数 名：CAN_DEV_AsynCycleService
功    能：非同步性周期服务函数
参    数：无
返 回 值：成功返回1，否则返回错误码
调用位置：(大循环中调用)
注意事项：
*/
uBit32 CAN_DEV_AsynCycleService(void)
{
    uBit8 uBusChIndex;

    for (uBusChIndex=0; uBusChIndex<CAN_CHANNEL_MAX_COUNT; uBusChIndex++)
    {
        if (m_pChData[uBusChIndex].bValidFlag==0)//通道未启用
            continue;

        //RcvProcess(uBusChIndex,0);

        if (m_pChData[uBusChIndex].sLastSetCmdData.uExeStep == CMD_EXE_STEP_WAIT_RES)
        {
            //等待的回应包超时
            if (m_sCanDevExInterface.pf_SYS_GetSysTickCount() - m_pChData[uBusChIndex].sLastSetCmdData.dStartTick>=CAN_GETRES_MAX_TIME)
            {
                m_pChData[uBusChIndex].sLastSetCmdData.uExeStep = CMD_EXE_STEP_END;
                m_pChData[uBusChIndex].sLastSetCmdData.ulExeRes = DVM_ERR_RCV_TIMEOUT;
            }
        }          
    }

    return DVM_ERR_SUCCESS;
}


//-------------------------------------------获取状态---------------------------------------------------
/*
函 数 名：CAN_DEV_GetState
功    能：获取设备状态
参    数：ulDevNO --设备编号
          *pDeviceState --设备状态数据（出参）
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：

uBit32 CAN_DEV_GetState(uBit32 ulDevNO, DEVICE_STATE* pDeviceState)
{
    memcpy(pDeviceState, m_pCanDeviceState+ulDevNO, sizeof(DEVICE_STATE));
    return DVM_ERR_SUCCESS;
}*/

/*
函 数 名：CAN_DEV_GetSvParm
功    能：获取伺服参数
参    数：ulAxisNO        --设备编号
                nParmNo        --伺服参数编号
                pParmData  --保存获取的伺服参数
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 CAN_DEV_GetSvParm(uBit32 ulAxisNO, uBit16 nParmNo, uBit8* pParmData)
{
    uBit32 ulRet;
    double dTimeTick;
    CanFrame sRcvPack;        
    MAINCTRL_COM_DATA_PACK sSendPack;
    DEVICE_FRAME_ID sRcvID;

#ifdef SYS_FUN_TEST
    GPIO_FunTestStart(DVM_CAN_GetSvParm);
#endif

    //请求发送允许
    ulRet = AskForRsPermit(m_pComNoMap[ulAxisNO].uBusChIndex);
    if (ulRet != DVM_ERR_SUCCESS)
        return ulRet;

    //发送请求获取设备软件版本号指令
    memset(&sSendPack, 0, sizeof(MAINCTRL_COM_DATA_PACK));
    sSendPack.ID.CmdID.ulDevComNO = m_pComNoMap[ulAxisNO].uComNo;
    sSendPack.ID.CmdID.ulMsgType = CAN_MSG_TYPE_CMD;
    sSendPack.ID.CmdID.ulCmdType = DEV_CMD_GET_SV_PARM;
    memcpy(sSendPack.Data.cDataBuf, &nParmNo, sizeof(uBit16));

    ulRet = SendSinglePack(m_pComNoMap[ulAxisNO].uBusChIndex, &sSendPack);
    if (ulRet != DVM_ERR_SUCCESS)
    {
        ReleaseRsPermit(m_pComNoMap[ulAxisNO].uBusChIndex);
        return ulRet;
    }

    dTimeTick = m_sCanDevExInterface.pf_SYS_GetSysTickCount();

    while(1)
    {
        if (m_sCanDevExInterface.pf_SYS_GetSysTickCount()-dTimeTick>=CAN_GETRES_MAX_TIME)//接收回应包超时
        {
            ulRet = DVM_ERR_RCV_TIMEOUT;
            break;
        }

        m_sCanComInterface.pf_CAN_RcvHandle(m_pComNoMap[ulAxisNO].uBusChIndex);

        if (m_sCanComInterface.pf_CAN_GetFrame(m_pComNoMap[ulAxisNO].uBusChIndex, &sRcvPack)!=CAN_ERR_SUCCESS)
            continue;

        //检查是否是要获取的回应包数据
        if (IsRequiredRes(sSendPack.ID.ulFrameID, sRcvPack.ulID))
        {
            sRcvID.ulFrameID = sRcvPack.ulID;
            ulRet = sRcvID.CmdAnsID.ulExeResult;

            if (ulRet==DVM_ERR_SUCCESS)
                memcpy(pParmData, sRcvPack.ucDatBuf, 4);

            break;
        }

        DealWithRcvPack(m_pComNoMap[ulAxisNO].uBusChIndex, &sRcvPack);
    }

    ReleaseRsPermit(m_pComNoMap[ulAxisNO].uBusChIndex);
#ifdef SYS_FUN_TEST
    GPIO_FunTestEnd(DVM_CAN_GetSvParm);
#endif
    return ulRet;
}

//-------------------------------------------获取版本信息---------------------------------------------------
/*
函 数 名：CAN_DEV_GetVersion
功    能：获取设备软件版本信息
参    数：ulDevNO --设备编号
          *pVersion --版本信息（出参）
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 CAN_DEV_GetVersion(uBit32 ulDevNO, SOFTWARE_VERSION* pVersion)
{
    uBit32 ulRet;
    double dTimeTick;
    CanFrame sRcvPack;        
    MAINCTRL_COM_DATA_PACK sSendPack;

#ifdef SYS_FUN_TEST
    GPIO_FunTestStart(DVM_CAN_GetDevVer);
#endif

    //请求发送允许
    ulRet = AskForRsPermit(m_pComNoMap[ulDevNO].uBusChIndex);
    if (ulRet != DVM_ERR_SUCCESS)
        return ulRet;

    //发送请求获取设备软件版本号指令
    memset(&sSendPack, 0, sizeof(MAINCTRL_COM_DATA_PACK));
    sSendPack.ID.CmdID.ulDevComNO = m_pComNoMap[ulDevNO].uComNo;
    sSendPack.ID.CmdID.ulMsgType = CAN_MSG_TYPE_CMD;
    sSendPack.ID.CmdID.ulCmdType = DEV_CMD_INQ_VER;

    ulRet = SendSinglePack(m_pComNoMap[ulDevNO].uBusChIndex, &sSendPack);
    if (ulRet != DVM_ERR_SUCCESS)
    {
        ReleaseRsPermit(m_pComNoMap[ulDevNO].uBusChIndex);
        return ulRet;
    }

    dTimeTick = m_sCanDevExInterface.pf_SYS_GetSysTickCount();

    while(1)
    {
        if (m_sCanDevExInterface.pf_SYS_GetSysTickCount()-dTimeTick>=CAN_GETRES_MAX_TIME)//接收回应包超时
        {
            ulRet = DVM_ERR_RCV_TIMEOUT;
            break;
        }

        m_sCanComInterface.pf_CAN_RcvHandle(m_pComNoMap[ulDevNO].uBusChIndex);

        if (m_sCanComInterface.pf_CAN_GetFrame(m_pComNoMap[ulDevNO].uBusChIndex, &sRcvPack)!=CAN_ERR_SUCCESS)
            continue;

        //检查是否是要获取的回应包数据
        if (IsRequiredRes(sSendPack.ID.ulFrameID, sRcvPack.ulID))
        {
            memcpy(pVersion, sRcvPack.ucDatBuf, sizeof(SOFTWARE_VERSION));
            break;
        }

        DealWithRcvPack(m_pComNoMap[ulDevNO].uBusChIndex, &sRcvPack);
    }


    ReleaseRsPermit(m_pComNoMap[ulDevNO].uBusChIndex);
#ifdef SYS_FUN_TEST
    GPIO_FunTestEnd(DVM_CAN_GetDevVer);
#endif
    return ulRet;
}


/*
函 数 名：CAN_DEV_GetDriverVersion
功    能：获取驱动库版本信息
参    数：*pVersion --版本信息（出参）
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 CAN_DEV_GetDriverVersion(SOFTWARE_VERSION* pVersion)
{
    *pVersion = m_sCanBusDriverSoftVer;

    return DVM_ERR_SUCCESS;
}


void CAN_DEV_GetBusDriver(BUS_DEVICE_DRIVE *pDeviceDrive)
{
    pDeviceDrive->pf_DEV_BusInit                = &CAN_DEV_BusInit              ;
    pDeviceDrive->pf_DEV_IsBusFree              = &CAN_DEV_IsBusFree            ;
    pDeviceDrive->pf_DEV_SetConfig              = &CAN_DEV_SetConfig            ;
    pDeviceDrive->pf_DEV_SetAxisParmReadAddr    = &CAN_DEV_SetAxisParmReadAddr  ;
    pDeviceDrive->pf_DEV_SetSysCtrlParmReadAddr = &CAN_DEV_SetSysCtrlParmReadAddr  ;
    pDeviceDrive->pf_DEV_SetDeviceInfoAddr      = &CAN_DEV_SetDeviceInfoAddr    ;
    pDeviceDrive->pf_DEV_SetDeviceStateAddr     = &CAN_DEV_SetDeviceStateAddr   ;
    pDeviceDrive->pf_DEV_Scan                   = &CAN_DEV_Scan                 ;
    pDeviceDrive->pf_DEV_SendFeed               = &CAN_DEV_SendFeed             ;
    pDeviceDrive->pf_DEV_SetCmd                 = &CAN_DEV_SetCmd               ;
    pDeviceDrive->pf_DEV_CheckFree              = &CAN_DEV_CheckFree            ;
    pDeviceDrive->pf_DEV_LockForIPO             = &CAN_DEV_LockForIPO           ;
    pDeviceDrive->pf_DEV_CheckLastCmdExeState   = &CAN_DEV_CheckLastCmdExeState ;
    pDeviceDrive->pf_DEV_SetParm                = &CAN_DEV_SetParm              ;
    pDeviceDrive->pf_DEV_SynCycleService        = &CAN_DEV_SynCycleService      ;
    pDeviceDrive->pf_DEV_AsynCycleService       = &CAN_DEV_AsynCycleService     ;
    pDeviceDrive->pf_DEV_GetSvParm       = &CAN_DEV_GetSvParm     ;
    pDeviceDrive->pf_DEV_GetVersion             = &CAN_DEV_GetVersion           ;
    pDeviceDrive->pf_DEV_GetDriverVersion       = &CAN_DEV_GetDriverVersion;
#ifdef RS_MONITOR_ENALBE
    pDeviceDrive->pf_DEV_GetBusRsMsData       = &CAN_DEV_GetBusRsMsData;
#endif
} 

void CAN_DEV_SetComInterface(CAN_INTERFACE sCanInterface)
{
    m_sCanComInterface = sCanInterface;
}

void CAN_DEV_SetExternalApi(CAN_DEV_EX_INTERFACE sCanDevExInterface)
{
    m_sCanDevExInterface = sCanDevExInterface;
}
