
#include "string.h"
#include "math.h"
#include "CAN_DEV_ComMana.h"
#include "CAN_DEV_Driver.h"
#include "CAN_COM_DataStructDef.h"

#include "..\\..\\ErrorCodeDef\\DVM_ErrorCodeDef.h"


//#include "..\\..\\GPIO\\GPIOCtrl.h"



//数据管理内部接口------------------------------------------------------------
void SetBusDevNoToDefault(void)        //设置总线设备编号为默认值DEV_MAX_COUNT(未编号)
{
    uBit8 uDevIndex;

    if (m_pCanDeviceInfo==NULL)
        return;

    for (uDevIndex=0; uDevIndex<m_uCanExsitDevCount; uDevIndex++)
    {
        if ((m_pCanDeviceInfo+uDevIndex)->nBusType == DEV_BUS_TYPE_CAN)
            (m_pCanDeviceInfo+uDevIndex)->nBusNO = DEV_INVALID_NO;
    }
}

//设置配置信息为默认值
void SetConfigDataToDefault(void)
{
    uBit8 uBusChIndex;
    uBit8 uAxisNo;

    for (uAxisNo=0; uAxisNo<DEV_MAX_COUNT; uAxisNo++)
    {//保存设备编号对应的通信号和设备编号
        m_pComNoMap[uAxisNo].uBusChIndex = 1;
        m_pComNoMap[uAxisNo].uComNo = 0X7F;
    }

    for (uBusChIndex=0; uBusChIndex<CAN_CHANNEL_MAX_COUNT; uBusChIndex++)
    {
        memset(m_pChData[uBusChIndex].pDevInfo, 0, CAN_DEV_MAX_COUNT*sizeof(DEVICE_INFO*));
        memset(m_pChData[uBusChIndex].pPulseCtrlWords, 0, CAN_DEV_MAX_COUNT*sizeof(PL_CTRL_WORDS));    //通道设备脉冲通信控制数据 
        memset(m_pChData[uBusChIndex].pAxisSentPulse, 0, CAN_DEV_MAX_COUNT*sizeof(Bit32));    //轴指令位置，用于指令位置校验
        memset(m_pChData[uBusChIndex].pAxisVerifyCmdPos, 0, CAN_DEV_MAX_COUNT*sizeof(double64));    //轴指令位置，用于指令位置校验
        m_pChData[uBusChIndex].uCodedDevCount = 0;
        m_pChData[uBusChIndex].uDevLockBit = 0;

        for (uAxisNo=0; uAxisNo<CAN_DEV_MAX_COUNT; uAxisNo++)
            m_pChData[uBusChIndex].pAxisScaleRatio[uAxisNo] = 1.0;
    }
}

/*
//检查指定ID设备是否已经存在设备信息数据区
BooLean IsDevExist(uBit32 ulDevID, DEVICE_INFO* pDevInfo, uBit8 uLen)
{
    uBit8 i;

    for (i=0; i<uLen; i++)
    {
        if((pDevInfo+i)->nDevieID == ulDevID)
            return true;
    }

    return false;
}*/

//检查设备是否锁定
BooLean IsDevLocked(uBit8 uBusChIndex, uBit8 cComNo)
{
    if (m_pChData[uBusChIndex].uDevLockBit & (0x01<<cComNo))
        return true;

    return false;
}

//通道是否锁定
BooLean IsChLocked(uBit8 uBusChIndex)
{
    return m_pChData[uBusChIndex].uDevLockBit==0?false:true;
}


//外部请求通道发送权限：当通道锁定时外部调用接口无权限
uBit32 AskForRsPermit(uBit8 uBusChIndex)
{
    double64 dPreTickCount;

    if(m_pChData[uBusChIndex].bValidFlag==0)//总线通道不可用
        return DVM_ERR_INVALID_CH;

    if(m_pChData[uBusChIndex].uDevLockBit)//总线通道有设备被锁定
        return DVM_ERR_BUS_BUSY;
    
    if(m_pChData[uBusChIndex].bRsAllowFlag==1)//总线被其他接口占用还未释放
          return DVM_ERR_BUS_BUSY;
    
    m_pChData[uBusChIndex].bRsAllowReqFlag=1;    //请求外部发送权限

    //等待插补中断处理请求完成，最多等CAN_GETRES_MAX_TIME个插补周期
    dPreTickCount = m_sCanDevExInterface.pf_SYS_GetSysTickCount();
    while (m_pChData[uBusChIndex].bRsAllowReqFlag==1)
    {
        if (m_sCanDevExInterface.pf_SYS_GetSysTickCount()-dPreTickCount > CAN_GETRES_MAX_TIME)
        {
            m_pChData[uBusChIndex].bRsAllowReqFlag = 0;
            return DVM_ERR_ASKFOR_RS_TIMEOUT;
        }
    }

    if (m_pChData[uBusChIndex].bRsAllowFlag==0)
        return DVM_ERR_BUS_BUSY;

    return DVM_ERR_SUCCESS;
}


//外部接口释放收发权限
void ReleaseRsPermit(uBit8 uBusChIndex)
{
    if (uBusChIndex>CAN_CHANNEL_MAX_COUNT)
        return;

    m_pChData[uBusChIndex].bRsAllowFlag = 0;    //置位外部收发标志
}
//-------------------------------------------------------------------------------------------


//接收数据处理接口--------------------------------------------------------------------------

//检查是否为轴脉冲进给/设备输出指令的回应包
BooLean IsRequiredPulseRes(MAINCTRL_PULSE_PACK_ID sSendID, DEV_PULSE_ANS_PACK_ID sRcvID)
{
    if (sSendID.ulCmdExist==sRcvID.AddCmdAnsID.ulCmdExist&&
        sSendID.ulDevComNO==sRcvID.AddCmdAnsID.ulDevComNO&&
        sSendID.ulMsgType==sRcvID.AddCmdAnsID.ulMsgType)
    {
        return true;
    }

    return false;
}

//检查是否为设置指令回应包
BooLean IsRequiredSetCmdRes(MAINCTRL_CMD_PACK_ID sSendID, DEV_CMD_ANS_PACK_ID sRcvID)
{
    if (sSendID.ulCmdType==sRcvID.ulCmdType&&
        sSendID.ulDevComNO==sRcvID.ulDevComNO&&
        sSendID.ulMsgType==sRcvID.ulMsgType)
    {
        return true;
    }

    return false;
}

//检查是否为参数指令回应包
BooLean IsRequiredParmRes(MAINCTRL_PARM_PACK_ID sSendID, DEV_PARM_ANS_PACK_ID sRcvID)
{
    if (sSendID.ulParaType==sRcvID.ulParaType&&
        sSendID.ulDevComNO==sRcvID.ulDevComNO&&
        sSendID.ulMsgType==sRcvID.ulMsgType)
    {
        return true;
    }

    return false;
}

//检查接收的回应包是否为发送指令的回应包
BooLean IsRequiredRes(uBit32 ulSendID, uBit32 ulRcvID)
{
    BooLean bRet=false;
    MAINCTRL_FRAME_ID sSendID;
    DEVICE_FRAME_ID   sRevID;

    sSendID.ulFrameID = ulSendID;
    sRevID.ulFrameID = ulRcvID;

    if (sSendID.CmdID.ulMsgType != sRevID.CmdAnsID.ulMsgType)
        return false;

    switch(sSendID.CmdID.ulMsgType)
    {
    case CAN_MSG_TYPE_PLUSE:    //脉冲指令数据包
        bRet = IsRequiredPulseRes(sSendID.PulseID, sRevID.PulseAnsID);
        break;
    case CAN_MSG_TYPE_CMD:    //控制命令数据包
        bRet = IsRequiredSetCmdRes(sSendID.CmdID,sRevID.CmdAnsID);
        break;
    case CAN_MSG_TYPE_PARM:    //参数数据包
        bRet = IsRequiredParmRes(sSendID.ParmID,sRevID.ParmAnsID);
        break;
    default:break;
    }

    return bRet;
}

//处理脉冲指令回应包中的附加指令
void DealWithAddCmd(uBit8 uBusChIndex, DEV_PULSE_ANS_PACK_ID *pAddCmdID)
{
    if (pAddCmdID->AddCmdAnsID.ulCmdExist==1)//有附加控制指令
    {
#ifdef SYS_FUN_TEST
        GPIO_FunTestStart(DVM_CAN_AddCmdProc);
#endif
        //判断是否为等待的附加指令回应包
        if (pAddCmdID->AddCmdAnsID.ulDevComNO == m_pChData[uBusChIndex].sLastSetCmdData.uAddComNo &&
            pAddCmdID->AddCmdAnsID.ulCmdType == m_pChData[uBusChIndex].sLastSetCmdData.uAddCmdType)
        {
            if (pAddCmdID->AddCmdAnsID.ulExeResult != DVM_ERR_SUCCESS)
            {//附加指令中途出错
                m_pChData[uBusChIndex].sLastSetCmdData.uExeStep = CMD_EXE_STEP_END;
                m_pChData[uBusChIndex].sLastSetCmdData.ulExeRes = pAddCmdID->AddCmdAnsID.ulExeResult;
                return;
            }

            //附加指令回应包处理
            switch(pAddCmdID->AddCmdAnsID.ulCmdType)
            {
            case  CAN_ADD_CMD_FIL:                    //CCW转矩限制
            case CAN_ADD_CMD_RIL:                    //CW转矩限制
            case CAN_ADD_CMD_CLR_ALM:                //报警清除
            case CAN_ADD_CMD_ENABLE_SV:                //使能电机
            case CAN_ADD_CMD_SET_HOME:                //回零
            case CAN_ADD_CMD_SET_IO_PWM_MODE:        //设置IO进入PWM模式
                //一个字节附加指令处理
                m_pChData[uBusChIndex].sLastSetCmdData.uExeStep = CMD_EXE_STEP_END;
                m_pChData[uBusChIndex].sLastSetCmdData.ulExeRes = pAddCmdID->AddCmdAnsID.ulExeResult;
                break;
            case CAN_ADD_CMD_SET_IO_PWM_DUTY:        //设置IO进入PWM占空比，共两个字节指令
                if (pAddCmdID->AddCmdAnsID.ulDataIndex==1)//第二个字节回包
                {
                    m_pChData[uBusChIndex].sLastSetCmdData.uExeStep = CMD_EXE_STEP_END;
                    m_pChData[uBusChIndex].sLastSetCmdData.ulExeRes = pAddCmdID->AddCmdAnsID.ulExeResult;
                }
                break;
            case CAN_ADD_CMD_SET_IO_PWM_FREQ:        //设置IO进入PWM频率，共4个字节指令
                if (pAddCmdID->AddCmdAnsID.ulDataIndex==3)//第四个字节回包
                {
                    m_pChData[uBusChIndex].sLastSetCmdData.uExeStep = CMD_EXE_STEP_END;
                    m_pChData[uBusChIndex].sLastSetCmdData.ulExeRes = pAddCmdID->AddCmdAnsID.ulExeResult;
                }
                break;
            default:
                break;
            }
        }

#ifdef SYS_FUN_TEST
        GPIO_FunTestEnd(DVM_CAN_AddCmdProc);
#endif
    }
}

//刷新轴状态数据，接收到轴脉冲数据包时调用
void RefreshAxisStateAfterPulseRes(uBit32 ulDevNo)
{
    double64 dCurPos = 0;        //轴编码器位置换算成实际位置值
    DEVICE_STATE *pDevState;
    const AXIS_PARM *pAxisParm;
    COM_NO_MAP_DATA *pComNoMapData;

#ifdef SYS_FUN_TEST
    GPIO_FunTestStart(DVM_CAN_AxisStatusUpdate);
#endif

    pDevState = m_pCanDeviceState + ulDevNo;
    pAxisParm = m_pCanDevAxisParm + ulDevNo;
    pComNoMapData = m_pComNoMap + ulDevNo;

    //设置编码器速度
    pDevState->AxisStateData.dQeiSpeed *= m_pChData[pComNoMapData->uBusChIndex].pAxisScaleRatio[pComNoMapData->uComNo];//转化为内部脉冲/ms
    //轴编码器位置换算成内部脉冲的值
    dCurPos = pDevState->AxisStateData.lQeiPos*m_pChData[pComNoMapData->uBusChIndex].pAxisScaleRatio[pComNoMapData->uComNo];

    //设置轴精确停止状态、到位误差状态
    if (pDevState->AxisStateData.ulRunningStatus & AXIS_STATUS_ZERO_SPEED)
    {    
        //轴到位误差状态刷新
        if (pAxisParm->AxisSafeParm.ulInPosBand)
        {
            if (fabs(dCurPos - pDevState->AxisStateData.dCmdPos) > pAxisParm->AxisSafeParm.ulInPosBand)
            {
                pDevState->AxisStateData.ulRunningStatus |= AXIS_STATUS_LOCATION_ERR;//到位误差过大
                pDevState->AxisStateData.ulRunningStatus &= ~AXIS_STATUS_FSTOP;
            }
            else
            {
                pDevState->AxisStateData.ulRunningStatus &= ~AXIS_STATUS_LOCATION_ERR;
                pDevState->AxisStateData.ulRunningStatus |= AXIS_STATUS_FSTOP;//已精确停止
            }
        }else
        {
            pDevState->AxisStateData.ulRunningStatus &= ~AXIS_STATUS_LOCATION_ERR;//到位误差过大
            pDevState->AxisStateData.ulRunningStatus |= AXIS_STATUS_FSTOP;//已精确停止
        }
    }else
    {
        pDevState->AxisStateData.ulRunningStatus &= ~AXIS_STATUS_FSTOP;            
        pDevState->AxisStateData.ulRunningStatus &= ~AXIS_STATUS_LOCATION_ERR;
    }

    //轴跟踪误差状态刷新
    if (pAxisParm->AxisSafeParm.ulFollowErrorLim)
    {
        if (fabs(dCurPos - pDevState->AxisStateData.dCmdPos) > pAxisParm->AxisSafeParm.ulFollowErrorLim)
        {
            pDevState->AxisStateData.ulRunningStatus |= AXIS_STATUS_FOLLOW_ERR;//到位误差过大
        }
        else
        {
            pDevState->AxisStateData.ulRunningStatus &= ~AXIS_STATUS_FOLLOW_ERR;
        }
    }else
    {
        pDevState->AxisStateData.ulRunningStatus &= ~AXIS_STATUS_FOLLOW_ERR;//跟随误差过大
    }

    //正负软限位处理-------------------------------------
    if (pAxisParm->AxisSafeParm.lSoftPosLimN !=0)
    {
        //if (pDevState->AxisStateData.dCmdPos< pAxisParm->AxisSafeParm.lSoftPosLimN)
        if (dCurPos< pAxisParm->AxisSafeParm.lSoftPosLimN)
            pDevState->AxisStateData.ulRunningStatus |= AXIS_STATUS_LEFT_SOFT_LIMIT;
        else
            pDevState->AxisStateData.ulRunningStatus &= ~AXIS_STATUS_LEFT_SOFT_LIMIT;
    }else
    {
        pDevState->AxisStateData.ulRunningStatus &= ~AXIS_STATUS_LEFT_SOFT_LIMIT;
    }


    if (pAxisParm->AxisSafeParm.lSoftPosLimP !=0)
    {
        //if (pDevState->AxisStateData.dCmdPos > pAxisParm->AxisSafeParm.lSoftPosLimP)
        if (dCurPos > pAxisParm->AxisSafeParm.lSoftPosLimP)
            pDevState->AxisStateData.ulRunningStatus |= AXIS_STATUS_RIGHT_SOFT_LIMIT;
        else
            pDevState->AxisStateData.ulRunningStatus &= ~AXIS_STATUS_RIGHT_SOFT_LIMIT;
    }else
    {
        pDevState->AxisStateData.ulRunningStatus &= ~AXIS_STATUS_RIGHT_SOFT_LIMIT;
    }

#ifdef SYS_FUN_TEST
    GPIO_FunTestEnd(DVM_CAN_AxisStatusUpdate);
#endif
}

//刷新轴报警状态
void RefreashAxisAlarmStatus(AXIS_STATE_DATA *pAxisState)
{
    //刷新轴模块致命错误位状态
    if (pAxisState->ulRunningStatus & AXIS_ERROR_STATUS_BIT_MASK)
        pAxisState->ulRunningStatus |=AXIS_STATUS_FAIL;
    else
        pAxisState->ulRunningStatus &= ~AXIS_STATUS_FAIL;

    //刷新轴模块报警位状态
    if ((pAxisState->ulRunningStatus & AXIS_ALARM_STATUS_BIT_MASK))
        pAxisState->ulRunningStatus |=AXIS_STATUS_ALARM;
    else
        pAxisState->ulRunningStatus &= ~AXIS_STATUS_ALARM;
}

//轴进给脉冲/IO输出指令应答包处理
uBit32  PulseAnsDataProc(uBit8 uBusChIndex, CanFrame *pPackData)
{
    uBit32 ulRet = DVM_ERR_SUCCESS;
    uBit8 uCheckSum=0;
    DEVICE_INFO *pDevInfo;
    DEVICE_STATE *pDevState;
    DEV_PULSE_ANS_PACK_ID   sPulseAnsID;
    DEV_PULSE_ANS_PACK_DATA sPulseAnsData;
    const AXIS_PARM *pAxisParm;

    memcpy(&sPulseAnsID, &pPackData->ulID, sizeof(DEV_PULSE_ANS_PACK_ID));
    memcpy(&sPulseAnsData, pPackData->ucDatBuf, sizeof(DEV_PULSE_ANS_PACK_DATA));

    //处理脉冲指令中的附加指令
    DealWithAddCmd(uBusChIndex, &sPulseAnsID);

    pDevInfo = m_pChData[uBusChIndex].pDevInfo[sPulseAnsID.AxisPulseAnsID.ulDevComNO];

    //设备减少后重新扫描，可能出现设备信息不再可用
    if (pDevInfo->nDeviceNO==DEV_INVALID_NO)
        return DVM_ERR_DEVNO_DISMATCH;

    pDevState = (m_pCanDeviceState + pDevInfo->nDeviceNO);

    //测试 -------------------------------------
    //测试是否收到非指定设备的脉冲回应包
    /*
    uBit32 ulResComNo;
    uBit32 ulTemp;

    if (m_pChData[uBusChIndex].bRsAllowReqFlag==0)//非外部请求终止了发送
    {
        ulResComNo = m_pChData[uBusChIndex].uCurResponseComNo;

        if (ulResComNo==0)
            ulResComNo = m_pChData[uBusChIndex].uCodedDevCount-1;
        else
            ulResComNo -= 1;

        if (sPulseAnsID.AxisPulseAnsID.ulDevComNO != ulResComNo)
        {//收到非指定设备的脉冲回应包
                pDevState->AxisStateData.ulSvAlarm |= 0x02;
                pDevState->AxisStateData.ulSvStatus +=1;
        }

        if (sPulseAnsID.AxisPulseAnsID.ulDevComNO==m_pChData[uBusChIndex].uCurResponseComNo)
        {//提前收到回应包
            pDevState->AxisStateData.ulSvAlarm |= 0x04;
            pDevState->AxisStateData.ulSvStatus +=1;
        }
    }*/
    //--------------------------------------------

    switch(pDevInfo->nDeviceType)
    {
    case DEV_TYPE_AXIS:                    //轴卡
    case DEV_TYPE_SV:                        //伺服
        {
#ifdef SYS_FUN_TEST
            GPIO_FunTestStart(DVM_CAN_AxisPulseAns);
#endif
            //刷新轴编码器速度////
            pDevState->AxisStateData.dQeiSpeed = sPulseAnsID.AxisPulseAnsID.ulQeiSpeed;

            if (sPulseAnsID.AxisPulseAnsID.ulQeiSpeedDir==1)//负向
                pDevState->AxisStateData.dQeiSpeed = -pDevState->AxisStateData.dQeiSpeed;

            //轴回零完成检查
            if (pDevState->AxisStateData.ulRunningStatus&AXIS_STATUS_HOME_RUNNING)//正在回零
            {
                if ((sPulseAnsData.AxisPulseAnsData.ulStatus&AXIS_STATUS_HOME_RUNNING)==0)//回零停止
                {
                    if (sPulseAnsData.AxisPulseAnsData.ulStatus&AXIS_STATUS_HOME_SUCESS)//回零成功
                    {
                        pAxisParm = m_pCanDevAxisParm + pDevInfo->nDeviceNO;
                        pDevState->AxisStateData.dPulsePos = pAxisParm->AxisHomeParm.lHomePos;
                        pDevState->AxisStateData.dCmdPos = pDevState->AxisStateData.dPulsePos;
                        pDevState->AxisStateData.dCmdPos *= pAxisParm->AxisScaleParm.lGearRatioPosScale;
                        pDevState->AxisStateData.dCmdPos /= pAxisParm->AxisScaleParm.lGearRatioPlusScale;
                    }
                }
            }

            //刷新当前编码器实际位置
            pDevState->AxisStateData.lQeiPos   = sPulseAnsData.AxisPulseAnsData.lQeiPos;

            switch(sPulseAnsID.AxisPulseAnsID.ulSvDataFlag)
            {
            case AXIS_PULSE_AXIS_STATUS:
                {
                    //刷新轴运行状态数据
                    pDevState->AxisStateData.ulRunningStatus &= ~AXIS_STATUS_DEV_PROCESS_MASK;
                    pDevState->AxisStateData.ulRunningStatus |= sPulseAnsData.AxisPulseAnsData.ulStatus;
                    break;
                }
            case AXIS_PULSE_SV_STATUS:
                {
                    //刷新伺服状态数据
                    pDevState->AxisStateData.ulSvStatus = sPulseAnsData.AxisPulseAnsData.ulStatus;
                    break;
                }
            case AXIS_PULSE_SV_ALARM:
                {
                    //刷新伺服报警
                    pDevState->AxisStateData.ulSvAlarm = sPulseAnsData.AxisPulseAnsData.ulStatus;
                    break;
                }
            default:
                break;
            }

            //检查轴指令位置(不回零时检查)
            if ((pDevState->AxisStateData.ulRunningStatus & AXIS_STATUS_HOME_RUNNING)==0)
            {
                uCheckSum = m_pChData[uBusChIndex].pAxisSentPulse[sPulseAnsID.AxisPulseAnsID.ulDevComNO]&0xFF;
                uCheckSum += (m_pChData[uBusChIndex].pAxisSentPulse[sPulseAnsID.AxisPulseAnsID.ulDevComNO]>>8)&0xFF;
                uCheckSum += (m_pChData[uBusChIndex].pAxisSentPulse[sPulseAnsID.AxisPulseAnsID.ulDevComNO]>>16)&0xFF;
                uCheckSum += (m_pChData[uBusChIndex].pAxisSentPulse[sPulseAnsID.AxisPulseAnsID.ulDevComNO]>>24)&0xFF;

                if(uCheckSum != sPulseAnsID.AxisPulseAnsID.ulCheckSun)
                {
                    /*Bit32 lTmp;
                    lTmp = m_pChData[uBusChIndex].pAxisSentPulse[sPulseAnsID.AxisPulseAnsID.ulDevComNO];
                    lTmp += pDevState->AxisStateData.lPinc;
                    uCheckSum = lTmp&0xFF;
                    uCheckSum += (lTmp>>8)&0xFF;
                    uCheckSum += (lTmp>>16)&0xFF;
                    uCheckSum += (lTmp>>24)&0xFF;

                    if (uCheckSum != sPulseAnsID.AxisPulseAnsID.ulCheckSun)*/
                        pDevState->AxisStateData.ulRunningStatus |= AXIS_STATUS_CMDPOS_ERR;
                }
            }

            //刷新轴状态
            //pDevState->AxisStateData.ulRunningStatus &= ~AXIS_STATUS_OFFLINE; 
            RefreshAxisStateAfterPulseRes(pDevInfo->nDeviceNO);
            RefreashAxisAlarmStatus(&pDevState->AxisStateData);

#ifdef SYS_FUN_TEST
            GPIO_FunTestEnd(DVM_CAN_AxisPulseAns);
#endif
            break;
        }
    case DEV_TYPE_OUTPUT:                    //输出卡
    case DEV_TYPE_INPUT:                    //输入卡
    case DEV_TYPE_IO:                        //输入+输出卡
        {
            pDevState->IOStateData.cSysStatus = sPulseAnsID.IOPulseAnsID.ulSysStatus;
            pDevState->IOStateData.nInputStatus = sPulseAnsData.IOPulseAnsData.ulInputStatus;
            pDevState->IOStateData.nOutputStatus = sPulseAnsData.IOPulseAnsData.ulOutputStatus;
            pDevState->IOStateData.cSysStatus &= ~IO_STATUS_OFF_LINE; 
            break;
        }
    case DEV_TYPE_ADDA:
        {
            pDevState->ADDAStateData.cSysStatus = sPulseAnsID.ADDAPulseAnsID.ulSysStatus;
            pDevState->ADDAStateData.nADStatus[sPulseAnsID.ADDAPulseAnsID.ulADCh1] = \
                sPulseAnsData.ADDAPulseAnsData.nADCh1;
            pDevState->ADDAStateData.nADStatus[sPulseAnsID.ADDAPulseAnsID.ulADCh2] = \
                sPulseAnsData.ADDAPulseAnsData.nADCh2;
            pDevState->ADDAStateData.nDAStatus[sPulseAnsID.ADDAPulseAnsID.ulDACh1] = \
                sPulseAnsData.ADDAPulseAnsData.nDACh1;
            pDevState->ADDAStateData.nDAStatus[sPulseAnsID.ADDAPulseAnsID.ulDACh2] = \
                sPulseAnsData.ADDAPulseAnsData.nDACh2;
            pDevState->ADDAStateData.cSysStatus &= ~IO_STATUS_OFF_LINE; 
            break;
        }
    default:
        return DVM_ERR_INVALID_FRAME;
    }

    m_pChData[uBusChIndex].pPulseCtrlWords[sPulseAnsID.AxisPulseAnsID.ulDevComNO].nTick=0;
    return ulRet;
}


//设备控制指令应答包数据处理
uBit32  SetCmdAnsDataProc(uBit8 uBusChIndex, CanFrame *pPackData)
{
    uBit32 ulRet;
    DEVICE_INFO *pDevInfo;
    DEVICE_STATE *pDevState;
    DEV_CMD_ANS_PACK_ID   *pSetCmdAnsID;
    DEV_CMD_ANS_PACK_DATA *pSetCmdAnsData;

    pSetCmdAnsID = (DEV_CMD_ANS_PACK_ID*)&pPackData->ulID;
    pSetCmdAnsData = (DEV_CMD_ANS_PACK_DATA*)pPackData->ucDatBuf;
    pDevInfo = m_pChData[uBusChIndex].pDevInfo[pSetCmdAnsID->ulDevComNO];

    if (pDevInfo->nDeviceNO==DEV_INVALID_NO)
        return DVM_ERR_DEVNO_DISMATCH;

    pDevState = m_pCanDeviceState + pDevInfo->nDeviceNO;

    //如果有指令正在等待回应包
    if (m_pChData[uBusChIndex].sLastSetCmdData.uExeStep == CMD_EXE_STEP_WAIT_RES)
    {
        if (m_pChData[uBusChIndex].sLastSetCmdData.uCmdType == pSetCmdAnsID->ulCmdType &&
            m_pChData[uBusChIndex].sLastSetCmdData.uDevNo == m_pChData[uBusChIndex].pDevInfo[pSetCmdAnsID->ulDevComNO]->nDeviceNO)
        {//是上次设置指令正等的回包，则设置等待结果
            m_pChData[uBusChIndex].sLastSetCmdData.uExeStep = CMD_EXE_STEP_END;
            m_pChData[uBusChIndex].sLastSetCmdData.ulExeRes = pSetCmdAnsID->ulExeResult;
#ifdef SYS_FUN_TEST
            GPIO_FunTestEnd(DVM_CAN_DEV_SetCmd);
#endif
        }else
        {    //等待的回应包超时
            if (m_sCanDevExInterface.pf_SYS_GetSysTickCount() - m_pChData[uBusChIndex].sLastSetCmdData.dStartTick>=CAN_GETRES_MAX_TIME)
            {
                m_pChData[uBusChIndex].sLastSetCmdData.uExeStep = CMD_EXE_STEP_END;
                m_pChData[uBusChIndex].sLastSetCmdData.ulExeRes = DVM_ERR_RCV_TIMEOUT;
            }
        }
    }

    //不处理回应包的设备设置指令(获取指令)-------------------------------
    if (pSetCmdAnsID->ulCmdType==DEV_CMD_INQ_INFO || pSetCmdAnsID->ulCmdType==DEV_CMD_INQ_VER ||
        pSetCmdAnsID->ulCmdType==DEV_CMD_BLINK_LED || pSetCmdAnsID->ulCmdType==DEV_CMD_GET_SV_PARM)
    {
        return DVM_ERR_SUCCESS;
    }

    //清除设备原有的状态数据，因为设备类型都可能改变了, 设备重新编号后要做复位处理
    if (pSetCmdAnsID->ulCmdType==DEV_CMD_SET_NO && pSetCmdAnsID->ulExeResult==DVM_ERR_SUCCESS)
    {
        memset(pDevState, 0, sizeof(DEVICE_STATE));
    }

    ulRet = DVM_ERR_SUCCESS;

    switch(pSetCmdAnsID->ulDevType)
    {
    case DEV_TYPE_AXIS:    //轴设备，则刷新轴状态数据
    case DEV_TYPE_SV:        //伺服
        {
            //系统复位、报警清除
            if (pSetCmdAnsID->ulCmdType==DEV_CMD_CLR_ALM||
                pSetCmdAnsID->ulCmdType==DEV_CMD_RESET)
            {
                m_pChData[uBusChIndex].pAxisSentPulse[pSetCmdAnsID->ulDevComNO] = 0;    //校验字清0
                pDevState->AxisStateData.ulRunningStatus = 0;
            }            

            pDevState->AxisStateData.lQeiPos = pSetCmdAnsData->AxisCmdAnsData.lQeiPos;
            pDevState->AxisStateData.ulRunningStatus &= ~AXIS_STATUS_DEV_PROCESS_MASK;
            pDevState->AxisStateData.ulRunningStatus |= pSetCmdAnsData->AxisCmdAnsData.ulStatus;
            RefreashAxisAlarmStatus(&pDevState->AxisStateData);
            break;
        }
    case DEV_TYPE_OUTPUT:                //输出卡
    case DEV_TYPE_INPUT:                    //输入卡
    case DEV_TYPE_IO:                        //输入+输出卡
        {
            //IO卡刷新输入输出状态检查
            pDevState->IOStateData.nInputStatus = pSetCmdAnsData->IOCmdAnsData.ulInputStatus; 
            pDevState->IOStateData.nOutputStatus = pSetCmdAnsData->IOCmdAnsData.ulOutputStatus; 
            break;
        }
    case DEV_TYPE_ADDA:                        //ADDA
        {
            //ADDA卡刷新轴ADDA状态数据
            pDevState->ADDAStateData.cSysStatus = pSetCmdAnsData->ADDACmdAnsData.uSysStatus;
            pDevState->ADDAStateData.nDAStatus[pSetCmdAnsData->ADDACmdAnsData.uDAChNo] = pSetCmdAnsData->ADDACmdAnsData.uDAStatus;
            pDevState->ADDAStateData.nADStatus[pSetCmdAnsData->ADDACmdAnsData.uADChNo] = pSetCmdAnsData->ADDACmdAnsData.uADStatus;
            break;
        }
    default:
        {
            ulRet = DVM_ERR_UNKNOW_DEV;
            break;
        }
    }

#ifdef SYS_FUN_TEST
    GPIO_FunTestEnd(DVM_CAN_DEV_SetCmd);
#endif

/*
    switch(pSetCmdAnsID->ulCmdType)
    {
    //不处理回应包的设备设置指令(获取指令)-------------------------------
    case  DEV_CMD_INQ_INFO:             //查询设备信息(返回设备信息，不处理)
    case  DEV_CMD_INQ_VER:                //查询版本信息(返回版本信息)
    case  DEV_CMD_BLINK_LED:            //设置LED闪烁（设备测试）
    case    DEV_CMD_GET_SV_PARM:        //获取伺服参数
        {
            break;
        }
    //所有设备的通用设置指令--------------------------------------------
    case  DEV_CMD_SET_NO:                //设置设备编号
        {//清除设备原有的状态数据，因为设备类型都可能改变了, 设备重新编号后要做复位处理
            memset(pDevState, 0, sizeof(DEVICE_STATE));
            
            switch(pSetCmdAnsID->ulDevType)
            {
            case DEV_TYPE_AXIS:    //轴设备，则刷新轴状态数据
            case DEV_TYPE_SV:        //伺服
                {
                    pDevState->AxisStateData.lQeiPos = pSetCmdAnsData->AxisCmdAnsData.lQeiPos;
                    pDevState->AxisStateData.ulRunningStatus = (pSetCmdAnsData->AxisCmdAnsData.ulStatus&AXIS_STATUS_DEV_PROCESS_MASK);
                    RefreashAxisAlarmStatus(&pDevState->AxisStateData);
                    break;
                }
            case DEV_TYPE_OUTPUT:                //输出卡
            case DEV_TYPE_INPUT:                    //输入卡
            case DEV_TYPE_IO:                        //输入+输出卡
                {
                    //IO卡刷新输入输出状态检查
                    pDevState->IOStateData.nInputStatus = pSetCmdAnsData->IOCmdAnsData.ulInputStatus; 
                    pDevState->IOStateData.nOutputStatus = pSetCmdAnsData->IOCmdAnsData.ulOutputStatus; 
                    break;
                }
            case DEV_TYPE_ADDA:                        //ADDA
                {
                    //ADDA卡刷新轴ADDA状态数据
                    pDevState->ADDAStateData.cSysStatus = pSetCmdAnsData->ADDACmdAnsData.uSysStatus;
                    pDevState->ADDAStateData.nDAStatus[pSetCmdAnsData->ADDACmdAnsData.uDAChNo] = pSetCmdAnsData->ADDACmdAnsData.uDAStatus;
                    pDevState->ADDAStateData.nADStatus[pSetCmdAnsData->ADDACmdAnsData.uADChNo] = pSetCmdAnsData->ADDACmdAnsData.uADStatus;
                    break;
                }
            default:break;
            }

            break;
        }

    //轴设备设置指令--------------------------------------------
    case  DEV_CMD_CLR_ALM:                    //报警清除
    case  DEV_CMD_RESET:                    //系统复位
        {
            //清除由总线驱动层管理的轴状态
            m_pChData[uBusChIndex].pAxisSentPulse[pSetCmdAnsID->ulDevComNO] = 0;    //校验字清0
            pDevState->AxisStateData.ulRunningStatus &= AXIS_STATUS_DEV_PROCESS_MASK;
        }
    case  DEV_CMD_ENABLE_SV:                //使能电机
    case  DEV_CMD_SET_HOME:                //回零
    case  DEV_CMD_SET_LOCK:                //轴锁定(表示轴正在进行插补运动) （待完善）
    case  DEV_CMD_SET_CMD_POS:            //设置指令位置
    case  DEV_CMD_SET_QEI_POS:            //设置编码器位置
    case  DEV_CMD_SET_VERIFY_MODE:    //设置进入校验模式
    case  DEV_CMD_SET_FSTP:                //CCW驱动禁止                              
    case  DEV_CMD_SET_RSTP:                //CW驱动禁止                                
    case  DEV_CMD_SET_CLESC1ZERO:        //偏差计算清零/速度选择1/零速箝位      
    case  DEV_CMD_SET_INH_SC2:            //指令脉冲禁止/速度选择2              
    case  DEV_CMD_SET_FIL:                    //CCW转矩限制                            
    case  DEV_CMD_SET_RIL:                    //CW转矩限制 
    case  DEV_CMD_INQ_SV_ALM:            //查询伺服报警
    case  DEV_CMD_SET_IPO_CYCLE:        //设置插补周期
        {
            pDevState->AxisStateData.lQeiPos = pSetCmdAnsData->AxisCmdAnsData.lQeiPos;
            pDevState->AxisStateData.ulRunningStatus &= ~AXIS_STATUS_DEV_PROCESS_MASK;
            pDevState->AxisStateData.ulRunningStatus |= pSetCmdAnsData->AxisCmdAnsData.ulStatus;
            RefreashAxisAlarmStatus(&pDevState->AxisStateData);
            ulRet = DVM_ERR_SUCCESS;
            break;
        }
    case  DEV_CMD_SET_IO_PWM_MODE:        //设置IO进入PWM模式
    case  DEV_CMD_SET_IO_PWM_DUTY:        //设置IO进入PWM占空比
    case  DEV_CMD_SET_IO_PWM_FREQ:        //设置IO进入PWM频率
        {
            //IO卡刷新输入输出状态
            pDevState->IOStateData.nInputStatus = pSetCmdAnsData->IOCmdAnsData.ulInputStatus; 
            pDevState->IOStateData.nOutputStatus = pSetCmdAnsData->IOCmdAnsData.ulOutputStatus; 
            ulRet = DVM_ERR_SUCCESS;
            break;
        }
    default:ulRet = DVM_ERR_UNKNOW_CMD;break;
    }*/

    return ulRet;
}


//处理CAN总线上已编号设备的数据回应包
uBit32  DealWithRcvPack(uBit8 uBusChIndex,CanFrame *pPackData)
{
    uBit32 ulRet = DVM_ERR_INVALID_FRAME;
    DEVICE_FRAME_ID sAnsPackId;

    sAnsPackId.ulFrameID = pPackData->ulID;

    //未编号设备的脉冲回应包丢弃
    if (sAnsPackId.CmdAnsID.ulDevComNO >= m_pChData[uBusChIndex].uCodedDevCount)
        return DVM_ERR_INVALID_FRAME;

    //未编号的设备回应包丢弃(设备扫描回应的包在外部接口处直接处理)
    if (m_pChData[uBusChIndex].pDevInfo[sAnsPackId.CmdAnsID.ulDevComNO] == NULL)
        return DVM_ERR_INVALID_FRAME;

    //清除设备回应计数
    m_pChData[uBusChIndex].pPulseCtrlWords[sAnsPackId.CmdAnsID.ulDevComNO].nTick = 0;

    switch(sAnsPackId.CmdAnsID.ulMsgType)
    {
    case CAN_MSG_TYPE_PLUSE:                    //脉冲指令数据包
        ulRet = PulseAnsDataProc(uBusChIndex,  pPackData);
        break;
    case CAN_MSG_TYPE_CMD:                    //控制命令数据包，获取数据包回包不处理回包数据
        ulRet = SetCmdAnsDataProc(uBusChIndex, pPackData);
        break;                        
    case CAN_MSG_TYPE_PARM:                    //参数数据包，               不处理
        ulRet = DVM_ERR_SUCCESS;
        break;
    default:break;
    }
    
    /*外部非同步周期性函数去做超时判断和处理
    //如果有指令正在等待回应包
    if (m_pChData[uBusChIndex].sLastSetCmdData.uExeStep == CMD_EXE_STEP_WAIT_RES)
    {    
        if (m_sCanDevExInterface.pf_SYS_GetSysTickCount() - m_pChData[uBusChIndex].sLastSetCmdData.dStartTick >=CAN_GETRES_MAX_TIME)
        {//等待的回应包超时
            m_pChData[uBusChIndex].sLastSetCmdData.uExeStep = CMD_EXE_STEP_END;
            m_pChData[uBusChIndex].sLastSetCmdData.ulExeRes = DVM_ERR_RCV_TIMEOUT;
        }
    }*/

    return ulRet;
}


//接收数据包处理
uBit32  RcvProcess(uBit8 uBusChIndex, uBit8 uTimeLimit)
{

    double dTimeTick;
    CanFrame sPackData;
    dTimeTick = m_sCanDevExInterface.pf_SYS_GetSysTickCount();

    do 
    {
        m_sCanComInterface.pf_CAN_RcvHandle(uBusChIndex);    //底层接收

        if (m_sCanComInterface.pf_CAN_GetFrame(uBusChIndex, &sPackData) == CAN_ERR_SUCCESS)
            return DealWithRcvPack(uBusChIndex, &sPackData);
        
        if (m_sCanDevExInterface.pf_SYS_GetSysTickCount()-dTimeTick>=uTimeLimit)
            break;
    } while (1);

    return DVM_ERR_RCV_TIMEOUT;//接收回应包超时
}

//-------------------------------------------------------------------------------------------


//数据发送处理-------------------------------------------------------------------------------
//发送单包数据
uBit32 SendSinglePack(uBit8 uBusChIndex, MAINCTRL_COM_DATA_PACK *pSendPack)
{
    uBit32 ulRet;

    pSendPack->ID.PulseID.ulReceiveObj = CAN_DEVICE_RECEIVE;
    ulRet = m_sCanComInterface.pf_CAN_SendPack(uBusChIndex, 
                                                                    pSendPack->ID.ulFrameID,
                                                                    pSendPack->Data.cDataBuf,
                                                                    sizeof(MAINCTRL_FRAME_DATA));

    if (ulRet!=DVM_ERR_SUCCESS)
        return DVM_ERR_CMD_SEND;

    return DVM_ERR_SUCCESS;
}

//发送参数数据
uBit32 SendParmData(uBit8 uBusChIndex, MAINCTRL_FRAME_ID ulID, uBit8 *pData, uBit32 ulLen)
{
    uBit8 i;
    uBit8 *pBuf;
    uBit32 ulRet;
    uBit8 cCheckNum;
    uBit32 ulEndPackLen;    //最后一包的长度    
    uBit32 ulPackCount;        //数据包总数
    uBit32 ulSendCount;        //已发送的数据包
    double dTimeTick;        //定时时间
    CanFrame sRcvPack;        //回应包
    Bit32 iDelayCycle;

    ulSendCount = 0;
    cCheckNum = 0;

#ifdef SYS_FUN_TEST
    GPIO_FunTestStart(DVM_CAN_ParmPack);
#endif

    //计算包个数
    pBuf = pData;
    ulPackCount = ulLen / CAN_DATA_MAX_LEN;
    ulEndPackLen = ulLen % CAN_DATA_MAX_LEN;

    if (ulEndPackLen)
        ulPackCount++;
    else
        ulEndPackLen = CAN_DATA_MAX_LEN;

    ulID.ParmID.ulReceiveObj = CAN_DEVICE_RECEIVE;

    //发送单包数据---------------------------------------------------------
    if (ulPackCount == 1 || ulPackCount==0)//单独包或只有单独ID
    {
        ulID.ParmID.ulFrameStatus = CAN_TRANS_SINGLE_FRAME;

        if (m_sCanComInterface.pf_CAN_SendPack(uBusChIndex, ulID.ulFrameID, pBuf, ulLen) != DVM_ERR_SUCCESS)
            return DVM_ERR_CMD_SEND;

        dTimeTick = m_sCanDevExInterface.pf_SYS_GetSysTickCount();

        while(1)
        {
            if (m_sCanDevExInterface.pf_SYS_GetSysTickCount()-dTimeTick>=CAN_FLASH_TIME_OUT)
                return DVM_ERR_RCV_TIMEOUT;//接收回应包超时

            m_sCanComInterface.pf_CAN_RcvHandle(uBusChIndex);

            if (m_sCanComInterface.pf_CAN_GetFrame(uBusChIndex, &sRcvPack)!=CAN_ERR_SUCCESS)
                continue;

            if (IsRequiredRes(ulID.ulFrameID, sRcvPack.ulID))
            {
                ulRet = ((DEV_PARM_ANS_PACK_ID*)&sRcvPack.ulID)->ulExeResult;
                break;
            }

            //非参数设置回应包处理
            DealWithRcvPack(uBusChIndex, &sRcvPack);
        }

        return ulRet;
    }
    //--------------------------------------------------------------------------


    //发送起始包和中间包---------------------------------------------------------
    while(ulSendCount < (ulPackCount-1))
    {
        ulID.ParmID.ulFrameIndex = ulSendCount % CAN_SEND_PARA_FRAME_INDEX_MAX;

        if (ulSendCount == 0)//起始包
            ulID.ParmID.ulFrameStatus = CAN_TRANS_BEGIN_FRAME;
        else                //中间包
            ulID.ParmID.ulFrameStatus = CAN_TRANS_MIDDLE_FRAME;

        if (m_sCanComInterface.pf_CAN_SendPack(uBusChIndex, ulID.ulFrameID, pBuf, CAN_DATA_MAX_LEN)!=DVM_ERR_SUCCESS)
            return DVM_ERR_CMD_SEND;

        //需要加适当的延时，否则会出现发送失败,连续发送时硬件响应没那么快（约4us）
        for(iDelayCycle=0; iDelayCycle<CAN_CONTINUE_SEND_GAP; iDelayCycle++);
        //while(m_sCanComInterface.pf_CAN_IsLastFrameSent(uBusChIndex)==false);

    
        //发送中途接收回应包，如果是参数设置的错误回包，直接退出不再发送
        m_sCanComInterface.pf_CAN_RcvHandle(uBusChIndex);

        if (m_sCanComInterface.pf_CAN_GetFrame(uBusChIndex, &sRcvPack)==CAN_ERR_SUCCESS)
        {
            if (IsRequiredRes(ulID.ulFrameID, sRcvPack.ulID))//参数设置回应包
            {
                ulRet = ((DEV_PARM_ANS_PACK_ID*)&sRcvPack.ulID)->ulExeResult;

                if (ulRet)
                    return ulRet;
            }
            
            //非参数设置回应包处理
            DealWithRcvPack(uBusChIndex, &sRcvPack);
        }
        
        //计算校验值
        for (i = 0; i < CAN_DATA_MAX_LEN; i++)
            cCheckNum += *(pBuf++);

        ulSendCount++;
    }
    //--------------------------------------------------------------------------

    //发送最后一个结束包--------------------------------------------------------
    ulID.ParmID.ulFrameIndex = ulSendCount % CAN_SEND_PARA_FRAME_INDEX_MAX;
    ulID.ParmID.ulFrameStatus = CAN_TRANS_MIDDLE_FRAME;
      
    if (m_sCanComInterface.pf_CAN_SendPack(uBusChIndex, ulID.ulFrameID, pBuf, ulEndPackLen) != DVM_ERR_SUCCESS)
        return DVM_ERR_CMD_SEND;

    for (i=0; i < ulEndPackLen; i++)
        cCheckNum += *(pBuf++);

    ulSendCount++;
    //--------------------------------------------------------------------------

    //发送校验包----------------------------------------------------------------
    //延时约4us,否则硬件响应不过来造成数据包丢失
    for(iDelayCycle=0; iDelayCycle<CAN_CONTINUE_SEND_GAP; iDelayCycle++);
    
    ulID.ParmID.ulFrameIndex = ulSendCount % CAN_SEND_PARA_FRAME_INDEX_MAX;
    ulID.ParmID.ulFrameStatus = CAN_TRANS_VERIFY_FRAME;

    if (m_sCanComInterface.pf_CAN_SendPack(uBusChIndex, ulID.ulFrameID, &cCheckNum, 1) != DVM_ERR_SUCCESS)
        return DVM_ERR_CMD_SEND;
    //--------------------------------------------------------------------------

    //等待回应包----------------------------------------------------------------
    dTimeTick = m_sCanDevExInterface.pf_SYS_GetSysTickCount();

    while(1)
    {
        //参数回应包可能涉及到写FLASH，所以要求时间要长
        if (m_sCanDevExInterface.pf_SYS_GetSysTickCount()-dTimeTick>CAN_FLASH_TIME_OUT)
            return DVM_ERR_RCV_TIMEOUT;//接收回应包超时

        m_sCanComInterface.pf_CAN_RcvHandle(uBusChIndex);

        if (m_sCanComInterface.pf_CAN_GetFrame(uBusChIndex, &sRcvPack)!=CAN_ERR_SUCCESS)
            continue;

        //参数回应包处理
        if (IsRequiredRes(ulID.ulFrameID, sRcvPack.ulID))
        {
            ulRet = ((DEV_PARM_ANS_PACK_ID*)&sRcvPack.ulID)->ulExeResult;
            break;
        }

        //非参数设置回应包处理
        DealWithRcvPack(uBusChIndex, &sRcvPack);
    }
    //--------------------------------------------------------------------------


#ifdef SYS_FUN_TEST
    GPIO_FunTestEnd(DVM_CAN_ParmPack);
#endif
    return ulRet;
}

//发送轴控制参数
uBit32 SendAxisParm(COM_NO_MAP_DATA sComNoMapData, uBit8 *pParmBuf, uBit32 ulParmLen)
{
    uBit32 ulRet;
    AXIS_PARM *pAxisParm;
    MAINCTRL_FRAME_ID ulID;

    ulID.ParmID.ulDevComNO = sComNoMapData.uComNo;
    ulID.ParmID.ulMsgType = CAN_MSG_TYPE_PARM;
    ulID.ParmID.ulParaType = DEV_PARM_TYPE_AXIS_PARM;

    
    ulRet = SendParmData(sComNoMapData.uBusChIndex, ulID, pParmBuf, ulParmLen);
    if (ulRet)
        return ulRet;

    //计算比例系数
    pAxisParm = (AXIS_PARM*)pParmBuf;

    m_pChData[sComNoMapData.uBusChIndex].pAxisScaleRatio[ sComNoMapData.uComNo] = (double)pAxisParm->AxisScaleParm.lInternalPulseScale/pAxisParm->AxisScaleParm.lQEIPulseScale;    //内部指令脉冲/QEI脉冲
    m_pChData[sComNoMapData.uBusChIndex].pAxisScaleRatio[ sComNoMapData.uComNo] *= (double)pAxisParm->AxisScaleParm.lGearRatioPosScale/pAxisParm->AxisScaleParm.lGearRatioPlusScale;    //用户计数单位/QEI脉冲
    return DVM_ERR_SUCCESS;
}

//发送轴控制参数
uBit32 SendSysCtrlParm(COM_NO_MAP_DATA sComNoMapData, uBit8 *pParmBuf, uBit32 ulParmLen)
{
    MAINCTRL_FRAME_ID ulID;

    ulID.ParmID.ulDevComNO = sComNoMapData.uComNo;
    ulID.ParmID.ulMsgType = CAN_MSG_TYPE_PARM;
    ulID.ParmID.ulParaType = DEV_PARM_TYPE_SYS_CTRL_PARM;
    return SendParmData(sComNoMapData.uBusChIndex, ulID, pParmBuf, ulParmLen);
}

//发送伺服控制参数
uBit32 SendSevorParm(COM_NO_MAP_DATA sComNoMapData, uBit8 *pParmBuf, uBit32 ulParmLen)
{
    MAINCTRL_FRAME_ID ulID;

    ulID.ParmID.ulDevComNO = sComNoMapData.uComNo;
    ulID.ParmID.ulMsgType = CAN_MSG_TYPE_PARM;
    ulID.ParmID.ulParaType = DEV_PARM_TYPE_SV;
    return SendParmData(sComNoMapData.uBusChIndex, ulID, pParmBuf, ulParmLen);
}


//发送升级数据
uBit32 SendAppUpdateData(COM_NO_MAP_DATA sComNoMapData, uBit8 *pParmBuf, uBit32 ulParmLen)
{
    MAINCTRL_FRAME_ID ulID;
    ulID.ParmID.ulDevComNO = sComNoMapData.uComNo;
    ulID.ParmID.ulMsgType = CAN_MSG_TYPE_PARM;
    ulID.ParmID.ulParaType = DEV_PARM_TYPE_UPDATE_APP;
    return  SendParmData(sComNoMapData.uBusChIndex, ulID, pParmBuf, ulParmLen);
}

uBit32 SendBootUpdateData(COM_NO_MAP_DATA sComNoMapData, uBit8 *pParmBuf, uBit32 ulParmLen)
{
    MAINCTRL_FRAME_ID ulID;
    ulID.ParmID.ulDevComNO = sComNoMapData.uComNo;
    ulID.ParmID.ulMsgType = CAN_MSG_TYPE_PARM;
    ulID.ParmID.ulParaType = DEV_PARM_TYPE_UPDATE_BOOT;
    return SendParmData(sComNoMapData.uBusChIndex, ulID, pParmBuf, ulParmLen);
}
//-------------------------------------------------------------------------------------------

/*
函 数 名：HandleOutputCmd(uBit32 ulDevNO, Bit32 lCmdPara1, Bit32 lCmdPara2)
功    能：输出指令处理流程, IO输出和ADDA输出设置指令不涉及立即与设备端的通信
参    数：        
                ulDevNO        --设备编号
                lCmdPara1/lCmdPara2
                                --当为OUTPUT输出设备时，lCmdPara1：输出状态值；lCmdPara2：输出位掩码
                                --当为DA输出设备时，lCmdPara1：DA输出值；lCmdPara2：DA通道编号
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 HandleOutputCmd(uBit32 ulDevNO,Bit32 lCmdPara1, Bit32 lCmdPara2)
{
    uBit32 ulMaxValue;
    uBit32 ulOutputCmdStatus;    //临时保存指令输出状态
    DEVICE_INFO *pDevInfo;
    DEVICE_STATE *pDevState;
    uBit8 uBusChIndex;
    uBit8 uComNo;
    uBit8 uDAChNo;

    pDevState = m_pCanDeviceState + ulDevNO;
    pDevInfo = m_pChData[m_pComNoMap[ulDevNO].uBusChIndex].pDevInfo[m_pComNoMap[ulDevNO].uComNo];

    if (pDevInfo->nDeviceNO==DEV_INVALID_NO)
        return DVM_ERR_DEVNO_DISMATCH;

    switch(pDevInfo->nDeviceType)
    {
    case DEV_TYPE_OUTPUT:                        //输出卡
    case DEV_TYPE_IO:                                //输入+输出卡
        if ((pDevState->IOStateData.cSysStatus&IO_STATUS_OFF_LINE)==IO_STATUS_OFF_LINE)
            return DVM_ERR_DEV_OFFLINE;//设备掉线
        
#ifdef RS_MONITOR_ENALBE
    m_sCanBusRsMsData.sCanRsMsData.nIOSetTimes++;//记录IO发送的次数
#endif
        //临时变量保存指令输出状态，防止脉冲指令发出不确定的输出
        ulMaxValue = 1<<pDevInfo->IOResource.nOutputCount;
        ulMaxValue -=1;
        ulOutputCmdStatus = pDevState->IOStateData.nOutputCmdStatus;
        ulOutputCmdStatus &= ~lCmdPara2;                                        //输出状态位掩码清零
        ulOutputCmdStatus |=((lCmdPara1&lCmdPara2)&ulMaxValue);        //设置输出状态对应掩码位的值
        pDevState->IOStateData.nOutputCmdStatus = ulOutputCmdStatus;
#ifdef SYS_FUN_TEST
        GPIO_FunTestEnd(DVM_CAN_DEV_SetCmd);
#endif
        return DVM_ERR_SUCCESS;
    case DEV_TYPE_ADDA:                            //AD/DA卡
        {
            if (lCmdPara2>=pDevInfo->ADDAResource.nDACount)
                return DVM_ERR_WRONG_DATA;

            if ((pDevState->ADDAStateData.cSysStatus&IO_STATUS_OFF_LINE)==IO_STATUS_OFF_LINE)
                return DVM_ERR_DEV_OFFLINE;//设备掉线

            uBusChIndex = m_pComNoMap[ulDevNO].uBusChIndex;
            uComNo = m_pComNoMap[ulDevNO].uComNo;
            uDAChNo = GetPulseCtrlDataIndex(m_pChData[uBusChIndex].pPulseCtrlWords[uComNo]);

            //如果中断服务函数正在发送ADDA卡当前通道的输出值，则需要重新从低字节开始发送
            if (m_pChData[uBusChIndex].pPulseCtrlWords[uComNo].nDAHBit==0 && uDAChNo == lCmdPara2)
            {
                m_pChData[uBusChIndex].pPulseCtrlWords[uComNo].nDAHBit = 1;
                //return DVM_ERR_BUS_BUSY;//已经发送了DA当前通道的低字节，但高字节尚未发送
            }

            ulMaxValue = 1<<pDevInfo->ADDAResource.nDABitLen;
            ulMaxValue -=1;

            if (lCmdPara1>ulMaxValue)
                pDevState->ADDAStateData.nDACmdStatus[lCmdPara2] = ulMaxValue;
            else
                pDevState->ADDAStateData.nDACmdStatus[lCmdPara2] = lCmdPara1;
#ifdef SYS_FUN_TEST
            GPIO_FunTestEnd(DVM_CAN_DEV_SetCmd);
#endif
            return DVM_ERR_SUCCESS;
        }
        break;
    default:break;
    }

    return DVM_ERR_UNKNOW_CMD;
}

/*
函 数 名：HandleOutputCmd(uBit32 ulDevNO, Bit32 lCmdPara1, Bit32 lCmdPara2)
功    能：设置校验模式
参    数：        
                ulDevNO        --设备编号
                lCmdPara1    --0取消校验模式，1设置校验模式
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：校验模式只对轴设备有效，其他设备无效
*/
uBit32 HandleVerifyCmd(uBit32 ulDevNO,Bit32 lCmdPara1)
{
    uBit8 uComNo;
    uBit8 uBusChIndex;
    DEVICE_INFO *pDevInfo;
    DEVICE_STATE *pDevState;

    uBusChIndex = m_pComNoMap[ulDevNO].uBusChIndex;
    uComNo = m_pComNoMap[ulDevNO].uComNo;
    pDevState = m_pCanDeviceState + ulDevNO;
    pDevInfo = m_pChData[uBusChIndex].pDevInfo[uComNo];

    //非轴设备校验模式
    if (pDevInfo->nDeviceType!=DEV_TYPE_AXIS && pDevInfo->nDeviceType!=DEV_TYPE_SV)
    {
#ifdef SYS_FUN_TEST
        GPIO_FunTestEnd(DVM_CAN_DEV_SetCmd);
#endif
        return DVM_ERR_UNKNOW_CMD;
    }

    //检查通道上一条指令执行状态，如果未执行完则不能再发送设置指令
    if (m_pChData[uBusChIndex].sLastSetCmdData.uExeStep != CMD_EXE_STEP_END)
    {
#ifdef SYS_FUN_TEST
        GPIO_FunTestEnd(DVM_CAN_DEV_SetCmd);
#endif
        return DVM_ERR_BUS_BUSY;
    }

    if (lCmdPara1==0)//取消校验模式
    {
        if ((pDevState->AxisStateData.ulRunningStatus & AXIS_STATUS_VERIFY)==0)//已经是非校验模式了
        {
#ifdef SYS_FUN_TEST
            GPIO_FunTestEnd(DVM_CAN_DEV_SetCmd);
#endif
            return DVM_ERR_SUCCESS;
        }
        
        //恢复轴指令位置
        pDevState->AxisStateData.ulRunningStatus &= ~AXIS_STATUS_VERIFY;
        pDevState->AxisStateData.dCmdPos = m_pChData[uBusChIndex].pAxisVerifyCmdPos[uComNo];
    }else
    {
        if ((pDevState->AxisStateData.ulRunningStatus & AXIS_STATUS_VERIFY)==1)//已经是校验模式了
        {
#ifdef SYS_FUN_TEST
            GPIO_FunTestEnd(DVM_CAN_DEV_SetCmd);
#endif
            return DVM_ERR_SUCCESS;
        }

        //保存轴指令位置
        pDevState->AxisStateData.ulRunningStatus |= AXIS_STATUS_VERIFY;
        m_pChData[uBusChIndex].pAxisVerifyCmdPos[uComNo] = pDevState->AxisStateData.dCmdPos;
    }
#ifdef SYS_FUN_TEST
    GPIO_FunTestEnd(DVM_CAN_DEV_SetCmd);
#endif
    return DVM_ERR_SUCCESS;
}

/*
函 数 名：HandleBlinkLedCmd(uBit32 ulDevID, uBit32 ulBusChIndex)
功    能：测试设备指令处理
参    数：        
                ulDevID            --设备ID
                ulBusChIndex    --设备所在总线序号
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：测试LED跟获取设备信息的回应包数据相同
*/
uBit32 HandleBlinkLedCmd(uBit32 ulDevID, uBit32 ulBusChIndex)
{
    uBit32 ulRet;
    double64 dTimeTick;
    CanFrame sRcvPack;
    MAINCTRL_COM_DATA_PACK sSendPack={0};
    DEV_CMD_ANS_PACK_DATA *pSetCmdAnsData;
    DEV_CMD_ANS_PACK_ID *pDevCmdAnsID;

    //检查通道上一条指令执行状态，如果未执行完则不能再发送设置指令
    if (m_pChData[ulBusChIndex].sLastSetCmdData.uExeStep != CMD_EXE_STEP_END)
    {
#ifdef SYS_FUN_TEST
        GPIO_FunTestEnd(DVM_CAN_DEV_SetCmd);
#endif
        return DVM_ERR_BUS_BUSY;
    }

    //请求发送权限
    if (AskForRsPermit(ulBusChIndex) != DVM_ERR_SUCCESS)
    {
#ifdef SYS_FUN_TEST
        GPIO_FunTestEnd(DVM_CAN_DEV_SetCmd);
#endif
        return DVM_ERR_BUS_BUSY;
    }

    //配置通道设置命令数据区
    memset(&m_pChData[ulBusChIndex].sLastSetCmdData, 0, sizeof(CAN_SET_CMD_DATA));
    m_pChData[ulBusChIndex].sLastSetCmdData.uCmdType = DEV_CMD_BLINK_LED;
    m_pChData[ulBusChIndex].sLastSetCmdData.uDevNo = DEV_INVALID_NO;
    m_pChData[ulBusChIndex].sLastSetCmdData.uExeStep = CMD_EXE_STEP_END;
    m_pChData[ulBusChIndex].sLastSetCmdData.ulExeRes = DVM_ERR_BUS_BUSY;

    //配置发送ID
    sSendPack.ID.CmdID.ulMsgType = CAN_MSG_TYPE_CMD;    //
    sSendPack.ID.CmdID.ulCmdType = DEV_CMD_BLINK_LED;

    //配置发送数据
    memcpy(&sSendPack.Data.ulDataBuf[0], &ulDevID, sizeof(Bit32));

    //发送命令数据包
    ulRet = SendSinglePack(ulBusChIndex, &sSendPack);
    m_pChData[ulBusChIndex].sLastSetCmdData.ulExeRes = ulRet;
    m_pChData[ulBusChIndex].sLastSetCmdData.dStartTick = m_sCanDevExInterface.pf_SYS_GetSysTickCount();

    //指令发送成功,等待回应包
    if (ulRet == DVM_ERR_SUCCESS)
    {
        dTimeTick = m_sCanDevExInterface.pf_SYS_GetSysTickCount();

        while(1)
        {
            if (m_sCanDevExInterface.pf_SYS_GetSysTickCount()-dTimeTick>=CAN_GETRES_MAX_TIME)
            {//接收回应包超时
                ulRet = DVM_ERR_RCV_TIMEOUT;
                break;
            }

            m_sCanComInterface.pf_CAN_RcvHandle(ulBusChIndex);

            if (m_sCanComInterface.pf_CAN_GetFrame(ulBusChIndex, &sRcvPack)!=CAN_ERR_SUCCESS)
                continue;

            //检查是否是要获取的回应包数据
            pDevCmdAnsID = (DEV_CMD_ANS_PACK_ID*)(&sRcvPack.ulID);
            pSetCmdAnsData = (DEV_CMD_ANS_PACK_DATA*)sRcvPack.ucDatBuf;

            if (pDevCmdAnsID->ulMsgType == CAN_MSG_TYPE_CMD)
            {
                if (pDevCmdAnsID->ulCmdType == DEV_CMD_BLINK_LED)
                {
                    if (ulDevID==pSetCmdAnsData->InqInfoAxisAnsData.ulID)
                    {
                        m_pChData[ulBusChIndex].sLastSetCmdData.ulExeRes = DVM_ERR_SUCCESS;
                        ulRet = DVM_ERR_SUCCESS;
                        break;
                    }else
                    {
                        m_pChData[ulBusChIndex].sLastSetCmdData.ulExeRes = DVM_ERR_UNKNOW_DEV;
                        ulRet = DVM_ERR_UNKNOW_DEV;
                    }
                }
            }

            DealWithRcvPack(ulBusChIndex, &sRcvPack);
        }
    }

    //释放发送权限
    ReleaseRsPermit(ulBusChIndex);
#ifdef SYS_FUN_TEST
    GPIO_FunTestEnd(DVM_CAN_DEV_SetCmd);
#endif
    return ulRet;
}


/*
函 数 名：HandleAddCmd(uBit32 ulDevNO, uBit32 ulCmdType, Bit32 lCmdPara1, Bit32 lCmdPara2, Bit32 iWaitExeResult)
功    能：总线锁定但轴未锁定时，附加指令处理
参    数：        
                ulDevID            --设备ID
                ulBusChIndex    --设备所在总线序号
返 回 值：成功返回0，否则返回错误码
调用位置：
注意事项：
*/
uBit32 HandleAddCmd(uBit32 ulDevNO, uBit32 ulCmdType, Bit32 lCmdPara1, Bit32 lCmdPara2, Bit32 iWaitExeResult)
{
    uBit32 ulRet;
    uBit32 ulBusChIndex;

    ulBusChIndex = m_pComNoMap[ulDevNO].uBusChIndex;

    //配置通道设置命令数据区
    memset(&m_pChData[ulBusChIndex].sLastSetCmdData, 0, sizeof(CAN_SET_CMD_DATA));
    m_pChData[ulBusChIndex].sLastSetCmdData.uCmdType = ulCmdType;
    m_pChData[ulBusChIndex].sLastSetCmdData.uDevNo = ulDevNO;
    m_pChData[ulBusChIndex].sLastSetCmdData.uExeStep = CMD_EXE_STEP_END;
    m_pChData[ulBusChIndex].sLastSetCmdData.ulExeRes = DVM_ERR_BUS_BUSY;

    if (iWaitExeResult)//要求立即返回
    {
#ifdef SYS_FUN_TEST
        GPIO_FunTestEnd(DVM_CAN_DEV_SetCmd);
#endif
        return DVM_ERR_BUS_BUSY;
    }

    switch(ulCmdType)
    {
    case  DEV_CMD_CLR_ALM:                //报警清除，无数据区
        m_pChData[ulBusChIndex].sLastSetCmdData.uAddCmdType = CAN_ADD_CMD_CLR_ALM;
        break;
    case  DEV_CMD_ENABLE_SV:            //使能电机，1个字节数据区
        m_pChData[ulBusChIndex].sLastSetCmdData.uAddCmdType = CAN_ADD_CMD_ENABLE_SV;
        m_pChData[ulBusChIndex].sLastSetCmdData.pAddCmdData[0] = lCmdPara1;
        break;
    case  DEV_CMD_SET_HOME:                //回零，1个字节数据区
        m_pChData[ulBusChIndex].sLastSetCmdData.uAddCmdType = CAN_ADD_CMD_SET_HOME;
        m_pChData[ulBusChIndex].sLastSetCmdData.pAddCmdData[0] = lCmdPara1;
        break;
    case  DEV_CMD_SET_IO_PWM_MODE:        //设置IO进入PWM模式，1个字节数据区，高5位保存编号，低3位保存开启或关闭
        m_pChData[ulBusChIndex].sLastSetCmdData.uAddCmdType = CAN_ADD_CMD_SET_IO_PWM_MODE;
        m_pChData[ulBusChIndex].sLastSetCmdData.pAddCmdData[0] = lCmdPara1;
        m_pChData[ulBusChIndex].sLastSetCmdData.pAddCmdData[0] <<= 3;    //高5位保存IO端口号
        m_pChData[ulBusChIndex].sLastSetCmdData.pAddCmdData[0] += lCmdPara2==0?0:1; //低3位保存开启或关闭PWM模式
        break;
    case  DEV_CMD_SET_IO_PWM_DUTY:        //设置IO进入PWM占空比, 2字节数据区，最高字节高保存IO端口编号，低字节保存占空比
        m_pChData[ulBusChIndex].sLastSetCmdData.uAddCmdType = CAN_ADD_CMD_SET_IO_PWM_DUTY;
        m_pChData[ulBusChIndex].sLastSetCmdData.uAddMaxIndex = 1;
        m_pChData[ulBusChIndex].sLastSetCmdData.pAddCmdData[1] = lCmdPara1;
        m_pChData[ulBusChIndex].sLastSetCmdData.pAddCmdData[0] = lCmdPara2;
        break;
    case  DEV_CMD_SET_IO_PWM_FREQ:        //设置IO进入PWM频率,4字节数据区，最高5位保存IO端口编号，剩余字节保存频率 
        m_pChData[ulBusChIndex].sLastSetCmdData.uAddCmdType = CAN_ADD_CMD_SET_IO_PWM_FREQ;
        m_pChData[ulBusChIndex].sLastSetCmdData.uAddMaxIndex = 3;
        ulRet = (lCmdPara1<<27);        
        ulRet += (lCmdPara2&0x07FFFFFF);
        memcpy(m_pChData[ulBusChIndex].sLastSetCmdData.pAddCmdData, &ulRet, sizeof(uBit32));
        break;
    case  DEV_CMD_SET_FIL:                //CCW转矩限制, 1字节数据    
        m_pChData[ulBusChIndex].sLastSetCmdData.uAddCmdType = CAN_ADD_CMD_FIL;
        m_pChData[ulBusChIndex].sLastSetCmdData.pAddCmdData[0] = lCmdPara1;
        break;
    case  DEV_CMD_SET_RIL:                //CW转矩限制，1字节数据 
        m_pChData[ulBusChIndex].sLastSetCmdData.uAddCmdType = CAN_ADD_CMD_RIL;
        m_pChData[ulBusChIndex].sLastSetCmdData.pAddCmdData[0] = lCmdPara1;
        break;
    default:
        return DVM_ERR_BUS_BUSY;
    }

    m_pChData[ulBusChIndex].sLastSetCmdData.dStartTick = m_sCanDevExInterface.pf_SYS_GetSysTickCount();
    m_pChData[ulBusChIndex].sLastSetCmdData.uExeStep = CMD_EXE_STEP_NOT_START;
    m_pChData[ulBusChIndex].sLastSetCmdData.uAddComNo = m_pComNoMap[ulDevNO].uComNo;
#ifdef SYS_FUN_TEST
    GPIO_FunTestEnd(DVM_CAN_DEV_SetCmd);
#endif
    return DVM_ERR_SUCCESS;
}


//接收数据处理接口--------------------------------------------------------------------------
//设置脉冲数据包ID的ulData9
void SetPulseData9(MAINCTRL_PULSE_PACK_ID *pID,  uBit8 uBitIndex, uBit8 uValue)
{
    if (uValue)
        pID->ulData09 |= (0x01<<uBitIndex);
    else
        pID->ulData09 &= ~(0x01<<uBitIndex);
}

//设置脉冲数据包ID的ulData10
void SetPulseData10(MAINCTRL_PULSE_PACK_ID *pID, uBit8 uBitIndex, uBit8 uValue)
{
    if (uValue)
        pID->ulData10 |= (0x01<<uBitIndex);
    else
        pID->ulData10 &= ~(0x01<<uBitIndex);
}

//获取脉冲ID中uData9和uData10代表的序号
uBit8 GetPulseIDDataIndex(MAINCTRL_PULSE_PACK_ID *pID, uBit8 uBitIndex)
{
    return 2*((pID->ulData10>>uBitIndex)&0x01) + (pID->ulData09>>uBitIndex)&0x01;
}

//获取脉冲控制字中uData9和uData10代表的序号
uBit8 GetPulseCtrlDataIndex(PL_CTRL_WORDS sPlCtrlWords)
{
    return sPlCtrlWords.nData9 + 2*sPlCtrlWords.nData10;
}

//中断服务函数中接收数据包处理(接收上次脉冲数据回应包)
void SynCycleRcv(uBit32 uBusChIndex)
{
    uBit32 i;
    uBit32 ulRet;
    uBit8 uRetryTimes=0;

    CanFrame sRcvPack;                //接收数据包

    if (m_pChData[uBusChIndex].uCodedDevCount==0)
        return;

#ifdef SYS_FUN_TEST
    GPIO_FunTestStart(DVM_CAN_SynCycleRcv);
#endif
    while(1) 
    {
#ifdef SYS_FUN_TEST
        GPIO_FunTestStart(DVM_CAN_DEV_SynHardRcv);
#endif

          m_sCanComInterface.pf_CAN_RcvHandle(uBusChIndex);
#ifdef SYS_FUN_TEST
        GPIO_FunTestEnd(DVM_CAN_DEV_SynHardRcv);
#endif
        ulRet = m_sCanComInterface.pf_CAN_GetFrame(uBusChIndex, &sRcvPack);

        if (ulRet==CAN_ERR_SUCCESS)
        {
            DealWithRcvPack(uBusChIndex, &sRcvPack);           
#ifdef SYS_FUN_TEST
            GPIO_FunTestEnd(DVM_CAN_SynCycleRcv);
#endif
            return;
        }      


#ifdef RS_MONITOR_ENALBE
        m_sCanBusRsMsData.sCanRsMsData.nRcvPulseFailTimes++;//记录接收数据失败次数
#endif


        uRetryTimes++;
        if (uRetryTimes>=3)
            break;

        for(i=0; i<10000; i++);
    };

#ifdef SYS_FUN_TEST
    GPIO_FunTestEnd(DVM_CAN_SynCycleRcv);
#endif
}

//中断服务函数中刷新设备状态
void SynCycleUpdateState(uBit32 uBusChIndex)
{
    uBit8 uDevComNo=0;
    DEVICE_INFO *pDevInfo;
    DEVICE_STATE *pDevState;

#ifdef SYS_FUN_TEST
    GPIO_FunTestStart(DVM_CAN_SynStatusUpdate);
#endif

    for (uDevComNo=0; uDevComNo<m_pChData[uBusChIndex].uCodedDevCount; uDevComNo++)
    {
        pDevInfo = m_pChData[uBusChIndex].pDevInfo[uDevComNo];
        
        //重新扫描后设备被移除了但尚未编号
        if (pDevInfo->nDeviceNO==DEV_INVALID_NO)
            continue;

        if (pDevInfo->nDeviceType==DEV_TYPE_AXIS || pDevInfo->nDeviceType==DEV_TYPE_SV )
        {
            pDevState = m_pCanDeviceState + pDevInfo->nDeviceNO;

            if (pDevState->AxisStateData.lPinc!=0)
            {
                m_pChData[uBusChIndex].pAxisSentPulse[uDevComNo] += pDevState->AxisStateData.lPinc;
                pDevState->AxisStateData.lPinc = 0;
            }

            //清除轴加速度，指令速度
            if(m_pChData[uBusChIndex].pPulseCtrlWords[uDevComNo].nFeedFlag==0)
            {
                pDevState->AxisStateData.dAcc = 0;
                pDevState->AxisStateData.dCmdSpeed = 0;
            }else
            {
                m_pChData[uBusChIndex].pPulseCtrlWords[uDevComNo].nFeedFlag -= 1;
            }
        }
    }

    //主要刷新设备脱机状态
    if (m_pChData[uBusChIndex].pPulseCtrlWords[m_pChData[uBusChIndex].uCurResponseComNo].nTick>=CAN_DEV_UNRES_MAX_TIME)
    {
        pDevInfo = m_pChData[uBusChIndex].pDevInfo[m_pChData[uBusChIndex].uCurResponseComNo];

        if (pDevInfo->nDeviceNO==DEV_INVALID_NO)
            return;

        pDevState = m_pCanDeviceState + pDevInfo->nDeviceNO;
        m_pChData[uBusChIndex].pPulseCtrlWords[m_pChData[uBusChIndex].uCurResponseComNo].nTick = CAN_DEV_UNRES_MAX_TIME;

        switch(pDevInfo->nDeviceType)
        {
        case DEV_TYPE_AXIS:    //轴设备
        case DEV_TYPE_SV:        //伺服
            pDevState->AxisStateData.ulRunningStatus |= AXIS_STATUS_OFFLINE;
            break;
        case DEV_TYPE_OUTPUT:            //输出卡
        case DEV_TYPE_INPUT:            //输入卡
        case DEV_TYPE_IO:                //输入+输出卡
            pDevState->IOStateData.cSysStatus |= IO_STATUS_OFF_LINE;
            break;
        case DEV_TYPE_ADDA:                //AD/DA卡
            pDevState->ADDAStateData.cSysStatus |= IO_STATUS_OFF_LINE;
            break;
        default:break;
        }
    }

#ifdef SYS_FUN_TEST
    GPIO_FunTestEnd(DVM_CAN_SynStatusUpdate);
#endif
}

#ifdef ENABLE_EX_PULSE_BIT
/*
函数名称：    SetMotorPulseData(uBit32 ulBusChNo, uBit32 uPulseData, uBit8 uComNo, uBit32 *pBuf)

功    能：    设置CAN数据包8字节数据区轴脉冲数据

参    数：    ulBusChNo    --总线编号
            ulPulse        --脉冲数据
            uComNo        --通信编号
            pBuf        --CAN数据包数据区

返 回 值：    设置的脉冲数据

注意事项：    轴的脉冲数据存储：ID存储符号及最高位，数据区存放剩余低位数据,平均分配，分配多余位舍弃不用，CAN数据区分两个32位数据处理
            轴脉冲数据从高位开始存放，其他设备数据从低位开始存放(例如4轴+2IO卡时 数据区轴位宽为8+(2*8)/4)=12位
                ID10符号位：MMMMOOXX    
                ID09最高位：MMMMOOXX
                8字节数据区第一个32位数据：MMMMMMMMMMMMM|MMMMMMMMMMM|MMMMMMM
                8字节数据区第二个32位数据：MMMM|MMMMMMMMMMMM|OOOOOOOO|OOOOOOOO
调用位置：
*/
void SetMotorPulseData(uBit32 ulBusChNo, uBit32 uPulseData, uBit8 uComNo, uBit32 *pBuf)
{
    uBit8 uStartIndex;    //数据存放起始位置    
    uBit8 uEndIndex;    //数据存放起始位置

    //算法一分两个4字节数据处理
    uStartIndex = uComNo*m_pChData[ulBusChNo].uPulseBitLen;
    uEndIndex = uStartIndex+m_pChData[ulBusChNo].uPulseBitLen;
    uPulseData &= m_pChData[ulBusChNo].ulPulseBitMask;

    if (uStartIndex<32)
    {
        if (uEndIndex<=32)//全落在第一字节区
        {
            pBuf[0] &=~(m_pChData[ulBusChNo].ulPulseBitMask<<(32-uEndIndex));    //清除数据区
            pBuf[0] |= uPulseData<<(32-uEndIndex);
        }else//分开成两个字节存储
        {
            pBuf[0] &= ~((1<<(32-uStartIndex))-1);
            pBuf[0] |= uPulseData>>(uEndIndex-32);
            pBuf[1] &= ((1<<(64-uEndIndex))-1);
            pBuf[1] |= (uPulseData<<(64-uEndIndex));
        }
    }else//全落在最后一个字节区
    {
        pBuf[1] &=~(m_pChData[ulBusChNo].ulPulseBitMask<<(64-uEndIndex));    //清除数据区
        pBuf[1] |= uPulseData<<(64-uEndIndex);
    }
}

/*
函数名称：    SetOtherDevPulseData(uBit32 ulBusChNo, uBit8 uPulseData, uBit8 uComNoEx, uBit32 *pBuf)

功    能：    设置CAN数据包8字节数据区轴脉冲数据

参    数：    ulBusChNo    --总线编号
            ulPulse        --脉冲数据
            uComNoEx    --扩展通信编号，表示非轴设备脉冲数据应该存放的位置 = 总线允许挂接设备总数+轴设备总数-原通信编号-1
                          如总线挂6个设备，其中包含4个轴设备，通信编号5的IO设备，其脉冲数据应该存储的位置是 8+4-5-1=6
            pBuf        --CAN数据包数据区

返 回 值：    获取的脉冲数据

注意事项：    轴的脉冲数据存储：ID存储符号及最高位，数据区存放剩余低位数据,平均分配，分配多余位舍弃不用，CAN数据区分两个32位数据处理
            轴脉冲数据从高位开始存放，其他设备数据从低位开始存放(例如4轴+2IO卡时 数据区轴位宽为8+(2*8)/4)=12位
                ID10符号位：MMMMOOXX    
                ID09最高位：MMMMOOXX
                8字节数据区第一个32位数据：MMMMMMMMMMMMM|MMMMMMMMMMM|MMMMMMM
                8字节数据区第二个32位数据：MMMM|MMMMMMMMMMMM|OOOOOOOO|OOOOOOOO
调用位置：
*/
void SetOtherDevPulseData(uBit32 ulBusChNo, uBit8 uPulseData, uBit8 uComNoEx,uBit32 *pBuf)
{
    //算法一分两个4字节数据处理
    if (uComNoEx<4)
    {
        pBuf[0] |= uPulseData<<((4-uComNoEx-1)*8);
    }else
    {
        pBuf[1] |= uPulseData<<((8-uComNoEx-1)*8);
    }
}
#endif

//周期性服务函数中的发送处理
uBit32 SysCycleSend(uBit32 uBusChIndex)
{
    uBit32 ulRet;
    uBit8 uIndex;
    uBit8 uDaChNo;
    uBit8 uDevComNo;
    uBit8 uIoGroupCount;
    uBit8 uIoGroupIndex;
    uBit32 ulIoState;
    BooLean bResComSetFlag = false;    //回应设备已经设置标志
    DEVICE_INFO *pDevInfo;
    DEVICE_STATE *pDevState;
    uBit8 pPulseData[8];                //脉冲数据包数据区
    Bit32 lPulseTemp;
    MAINCTRL_PULSE_PACK_ID ulPulseID;    //脉冲数据包ID
#ifdef ENABLE_EX_PULSE_BIT
    uBit8 uDevComNoEx;
#endif

    if (m_pChData[uBusChIndex].uCodedDevCount==0)
        return DVM_ERR_SUCCESS;

#ifdef SYS_FUN_TEST
    GPIO_FunTestStart(DVM_CAN_SynCycleSend);
#endif

    //配置发送数据-----------------------------------------------
    memset(pPulseData, 0, 8);
    memset(&ulPulseID, 0, sizeof(MAINCTRL_CMD_PACK_ID));
    ulPulseID.ulMsgType = CAN_MSG_TYPE_PLUSE;
    ulPulseID.ulReceiveObj = CAN_DEVICE_RECEIVE;

    //有附加指令，优先发送附加指令数据
    if (m_pChData[uBusChIndex].sLastSetCmdData.uExeStep == CMD_EXE_STEP_NOT_START ||
        m_pChData[uBusChIndex].sLastSetCmdData.uExeStep == CMD_EXE_STEP_BEING)//设置指令未执行或执行中
    {
        uDevComNo = m_pChData[uBusChIndex].sLastSetCmdData.uAddComNo;
        //如果发送附加指令连续无应答次数超过规定次数，则不再指定该设备为应答设备，防止其他设备被饿死
        if (m_pChData[uBusChIndex].pPulseCtrlWords[uDevComNo].nTick>CAN_DEV_UNRES_MAX_TIME)
        {
            m_pChData[uBusChIndex].sLastSetCmdData.uExeStep = CMD_EXE_STEP_END;
            m_pChData[uBusChIndex].sLastSetCmdData.ulExeRes = DVM_ERR_RCV_TIMEOUT;
            ulPulseID.ulCmdExist = 0;
        }else
        {
            ulPulseID.ulDevComNO = m_pChData[uBusChIndex].sLastSetCmdData.uAddComNo;
            ulPulseID.ulCmdType = m_pChData[uBusChIndex].sLastSetCmdData.uAddCmdType;
            ulPulseID.ulCmdExist = 1;
            bResComSetFlag = true;    //标示通信编号已经设置

#ifdef ENABLE_EX_PULSE_BIT
            if (m_pChData[uBusChIndex].sLastSetCmdData.uExeStep == CMD_EXE_STEP_NOT_START)
            {//尚未开始，则发送附加指令数据第一个字节

                if (ulPulseID.ulDevComNO<m_pChData[uBusChIndex].uMotorDevCount)//轴设备
                {
                    SetMotorPulseData(uBusChIndex,m_pChData[uBusChIndex].sLastSetCmdData.pAddCmdData[0],ulPulseID.ulDevComNO,(uBit32*)pPulseData);
                }else//非轴设备
                {
                    uDevComNoEx = CAN_DEV_MAX_COUNT-(uDevComNo-m_pChData[uBusChIndex].uMotorDevCount)-1;
                    SetOtherDevPulseData(uBusChIndex,m_pChData[uBusChIndex].sLastSetCmdData.pAddCmdData[0],uDevComNoEx,(uBit32*)pPulseData);
                }

                m_pChData[uBusChIndex].sLastSetCmdData.uExeStep = CMD_EXE_STEP_BEING;
            }
            else//已经开始，则继续发送附加指令数据的相应数据
            {
                if (ulPulseID.ulDevComNO<m_pChData[uBusChIndex].uMotorDevCount)//轴设备
                {
                    SetMotorPulseData(uBusChIndex,
                        m_pChData[uBusChIndex].sLastSetCmdData.pAddCmdData[m_pChData[uBusChIndex].sLastSetCmdData.uAddPreIndex],
                        ulPulseID.ulDevComNO,
                        (uBit32*)pPulseData);
                }else//非轴设备
                {
                    uDevComNoEx = CAN_DEV_MAX_COUNT-(uDevComNo-m_pChData[uBusChIndex].uMotorDevCount)-1;
                    SetOtherDevPulseData(uBusChIndex,
                        m_pChData[uBusChIndex].sLastSetCmdData.pAddCmdData[m_pChData[uBusChIndex].sLastSetCmdData.uAddPreIndex],
                        uDevComNoEx,
                        (uBit32*)pPulseData);
                }
            }
#else
            if (m_pChData[uBusChIndex].sLastSetCmdData.uExeStep == CMD_EXE_STEP_NOT_START)
            {//尚未开始，则发送附加指令数据第一个字节
                pPulseData[ulPulseID.ulDevComNO] = m_pChData[uBusChIndex].sLastSetCmdData.pAddCmdData[0];
                m_pChData[uBusChIndex].sLastSetCmdData.uExeStep = CMD_EXE_STEP_BEING;
            }
            else//已经开始，则继续发送附加指令数据的相应数据
            {
                pPulseData[ulPulseID.ulDevComNO] = \
                    m_pChData[uBusChIndex].sLastSetCmdData.pAddCmdData[m_pChData[uBusChIndex].sLastSetCmdData.uAddPreIndex];
            }
#endif

            //设置发送ID中的ulData9和ulData10;
            SetPulseData9(&ulPulseID, ulPulseID.ulDevComNO, m_pChData[uBusChIndex].sLastSetCmdData.uAddPreIndex&0x01);
            SetPulseData10(&ulPulseID, ulPulseID.ulDevComNO, m_pChData[uBusChIndex].sLastSetCmdData.uAddPreIndex&0x02);

            //修改下次附加指令发送的数据序号
            if (m_pChData[uBusChIndex].sLastSetCmdData.uAddPreIndex == \
                m_pChData[uBusChIndex].sLastSetCmdData.uAddMaxIndex)
            {//指令发送完成
                m_pChData[uBusChIndex].sLastSetCmdData.uExeStep = CMD_EXE_STEP_WAIT_RES;
            }else
            {
                m_pChData[uBusChIndex].sLastSetCmdData.uAddPreIndex++;
            }

            m_pChData[uBusChIndex].uCurResponseComNo = m_pChData[uBusChIndex].uNextResponseComNo;
            m_pChData[uBusChIndex].pPulseCtrlWords[ulPulseID.ulDevComNO].nTick++;
            m_pChData[uBusChIndex].uNextResponseComNo = ulPulseID.ulDevComNO;
        }
    }


    //设置指令数据
    for (uDevComNo=0; uDevComNo<m_pChData[uBusChIndex].uCodedDevCount; uDevComNo++)
    {
        pDevInfo = m_pChData[uBusChIndex].pDevInfo[uDevComNo];

        if (pDevInfo->nDeviceNO==DEV_INVALID_NO)
            continue;

        pDevState = m_pCanDeviceState + pDevInfo->nDeviceNO;
    
        //发送附加指令数据
        if (bResComSetFlag && uDevComNo==ulPulseID.ulDevComNO)
            continue;

        switch(pDevInfo->nDeviceType)
        {
        case DEV_TYPE_AXIS://轴设备
        case DEV_TYPE_SV://伺服设备
            {
                if (pDevState->AxisStateData.lPinc!=0)//有进给时才计算
                {
                    lPulseTemp = pDevState->AxisStateData.lPinc;
                    //设置脉冲方向位
                    if (pDevState->AxisStateData.lPinc>0)//正向
                    {
                        SetPulseData10(&ulPulseID, uDevComNo, 0);
                    }
                    else
                    {
                        lPulseTemp  *= -1;
                        SetPulseData10(&ulPulseID, uDevComNo,1);
                    }
#ifdef ENABLE_EX_PULSE_BIT
                    //设置最高位
                    SetPulseData9(&ulPulseID, uDevComNo, lPulseTemp>>m_pChData[uBusChIndex].uPulseBitLen);
                    SetMotorPulseData(uBusChIndex,lPulseTemp,uDevComNo,(uBit32*)pPulseData);
#else
                    //设置8位数据
                    pPulseData[uDevComNo] = lPulseTemp;//&0XFF
                    //设置第9位数据
                    SetPulseData9(&ulPulseID, uDevComNo, lPulseTemp>>8);
#endif
                }
            }
            break;
        case DEV_TYPE_OUTPUT://输出设备
        case DEV_TYPE_IO:     //输入输出设备
            {
#ifdef ENABLE_EX_PULSE_BIT
                uDevComNoEx = CAN_DEV_MAX_COUNT-(uDevComNo-m_pChData[uBusChIndex].uMotorDevCount)-1;
#endif
                //计算有效输出组数
                uIoGroupCount = pDevInfo->IOResource.nOutputCount/8;
                if (pDevInfo->IOResource.nOutputCount%8)
                    uIoGroupCount++;

                //计算本次应该输出的组序号
                uIoGroupIndex = m_pChData[uBusChIndex].pPulseCtrlWords[uDevComNo].nData9 \
                    + 2*m_pChData[uBusChIndex].pPulseCtrlWords[uDevComNo].nData10;

                uIoGroupIndex++;
                uIoGroupIndex %= uIoGroupCount;
                //if (uIoGroupIndex >= uIoGroupCount)
                //    uIoGroupIndex =0;

                //如果输出和指令输出一致，则按顺序输出
                if (pDevState->IOStateData.nOutputStatus == pDevState->IOStateData.nOutputCmdStatus)
                {
                    //保存本次发送的输出组序号
                    m_pChData[uBusChIndex].pPulseCtrlWords[uDevComNo].nData9 = (uIoGroupIndex&0x01)==0?0:1;
                    m_pChData[uBusChIndex].pPulseCtrlWords[uDevComNo].nData10 = (uIoGroupIndex&0x02)==0?0:1;

                    //设置本次发送的输出组脉冲数据
                    SetPulseData9(&ulPulseID, uDevComNo, uIoGroupIndex&0x01);
                    SetPulseData10(&ulPulseID,uDevComNo, uIoGroupIndex&0x02);
#ifdef ENABLE_EX_PULSE_BIT
                    SetOtherDevPulseData(uBusChIndex,
                        (uBit8)(pDevState->IOStateData.nOutputCmdStatus>>(8*uIoGroupIndex))&0xFF,
                        uDevComNoEx,
                        (uBit32*)pPulseData);
#else
                    pPulseData[uDevComNo] = (uBit8)(pDevState->IOStateData.nOutputCmdStatus>>(8*uIoGroupIndex))&0xFF;
#endif
                }else//如果当前输出跟指令输出不一致，则优先输出更改的数据组
                {
                    ulIoState = pDevState->IOStateData.nOutputCmdStatus^pDevState->IOStateData.nOutputStatus;

                    //从本组开始查找更改的输出组序号
                    for (uIndex=uIoGroupIndex; uIndex<uIoGroupCount; uIndex++)
                    {
                        if (ulIoState&(0xFF<<8*uIndex))//输出组当前输出和指令输出不一致
                            break;
    
                        //if ((ulIoState>>(8*uIndex)) & 0xFF)//输出组当前输出和指令输出不一致
                        //    break;
                    }

                    //未找到则回到第0组往后找
                    if (uIndex==uIoGroupCount)
                    {
                        for (uIndex=0; uIndex<uIoGroupIndex; uIndex++)
                        {
                            if (ulIoState&(0xFF<<8*uIndex))//输出组当前输出和指令输出不一致
                                break;
                            //if ((ulIoState>>(8*uIndex)) & 0xFF)//输出组当前输出和指令输出不一致
                            //    break;
                        }
                    }

#ifdef ENABLE_EX_PULSE_BIT
                    SetOtherDevPulseData(uBusChIndex,
                        (uBit8)((pDevState->IOStateData.nOutputCmdStatus>>(8*uIndex))&0xFF),
                        uDevComNoEx,
                        (uBit32*)pPulseData);

#else
                    pPulseData[uDevComNo] = (uBit8)((pDevState->IOStateData.nOutputCmdStatus>>(8*uIndex))&0xFF);
#endif
                    SetPulseData9(&ulPulseID, uDevComNo, uIndex&0x01);
                    SetPulseData10(&ulPulseID,uDevComNo, uIndex&0x02);
                }
            }
            break;
        case DEV_TYPE_ADDA://ADDA设备
            {
#ifdef ENABLE_EX_PULSE_BIT
                uDevComNoEx = CAN_DEV_MAX_COUNT-(uDevComNo-m_pChData[uBusChIndex].uMotorDevCount)-1;
#endif
                //先检查上次通道ID数据是否发送完成
                if (m_pChData[uBusChIndex].pPulseCtrlWords[uDevComNo].nDAHBit==0)//上次发送只发了DA通道的低7位
                {
                    m_pChData[uBusChIndex].pPulseCtrlWords[uDevComNo].nDAHBit = 1;
                    SetPulseData9(&ulPulseID, uDevComNo, m_pChData[uBusChIndex].pPulseCtrlWords[uDevComNo].nData9);
                    SetPulseData10(&ulPulseID, uDevComNo, m_pChData[uBusChIndex].pPulseCtrlWords[uDevComNo].nData10);

                    uDaChNo = m_pChData[uBusChIndex].pPulseCtrlWords[uDevComNo].nData9 \
                        + 2*m_pChData[uBusChIndex].pPulseCtrlWords[uDevComNo].nData10;//保存先前的輸出通道
                    
#ifdef ENABLE_EX_PULSE_BIT
                    SetOtherDevPulseData(uBusChIndex,
                        (uBit8)((pDevState->ADDAStateData.nDACmdStatus[uDaChNo]>>7)&0x7F)|0x80,
                        uDevComNoEx,
                        (uBit32*)pPulseData);
#else
                    pPulseData[uDevComNo] = 0x80;
                    pPulseData[uDevComNo] |= (uBit8)((pDevState->ADDAStateData.nDACmdStatus[uDaChNo]>>7)&0x7F);
#endif
                }else//上次DA通道数据发送完成
                {
                    m_pChData[uBusChIndex].pPulseCtrlWords[uDevComNo].nDAHBit = 0;

                    //计算本次应该输出的通道
                    uDaChNo = m_pChData[uBusChIndex].pPulseCtrlWords[uDevComNo].nData9 \
                        + 2*m_pChData[uBusChIndex].pPulseCtrlWords[uDevComNo].nData10;//保存先前的输出通道

                    uDaChNo++;
                    uDaChNo %= pDevInfo->ADDAResource.nDACount;

                    //DA通道输出更改，则按顺序输出
                    if (memcmp(pDevState->ADDAStateData.nDACmdStatus, pDevState->ADDAStateData.nDAStatus, sizeof(uBit16)*DA_CHANNEL_COUNT)!=0)
                    {
                        //优先选择数据更改的DA通道发送
                        for (uIndex=uDaChNo; uIndex < pDevInfo->ADDAResource.nDACount;  uIndex++)
                        {
                            if (pDevState->ADDAStateData.nDACmdStatus[uIndex] != pDevState->ADDAStateData.nDAStatus[uIndex])
                                break;
                        }

                        //没有找到更改DA通道则从0通道开始往后继续找
                        if (uIndex==pDevInfo->ADDAResource.nDACount)
                        {
                            for (uIndex=0; uIndex < uDaChNo;  uIndex++)
                            {
                                if (pDevState->ADDAStateData.nDACmdStatus[uIndex] != pDevState->ADDAStateData.nDAStatus[uIndex])
                                    break;
                            }
                        }

                        uDaChNo = uIndex;
                    }

                    if (uDaChNo&0x01)
                        m_pChData[uBusChIndex].pPulseCtrlWords[uDevComNo].nData9=1;
                    else
                        m_pChData[uBusChIndex].pPulseCtrlWords[uDevComNo].nData9=0;

                    if (uDaChNo&0x02)
                        m_pChData[uBusChIndex].pPulseCtrlWords[uDevComNo].nData10=1;
                    else
                        m_pChData[uBusChIndex].pPulseCtrlWords[uDevComNo].nData10=0;

                    SetPulseData9(&ulPulseID, uDevComNo, m_pChData[uBusChIndex].pPulseCtrlWords[uDevComNo].nData9);
                    SetPulseData10(&ulPulseID, uDevComNo, m_pChData[uBusChIndex].pPulseCtrlWords[uDevComNo].nData10);
#ifdef ENABLE_EX_PULSE_BIT
                    SetOtherDevPulseData(uBusChIndex,
                        (uBit8)(0x7F&pDevState->ADDAStateData.nDACmdStatus[uDaChNo]),
                        uDevComNoEx,
                        (uBit32*)pPulseData);
#else
                    pPulseData[uDevComNo] = (uBit8)(0x7F&pDevState->ADDAStateData.nDACmdStatus[uDaChNo]);
#endif
                }
                break;
            }
        case DEV_TYPE_INPUT: //输入设备
        default:break;
        }
    }

    //如果未指定回应设备(无附件指令发送)，则按顺序指定回应设备
    if(bResComSetFlag==false)
    {
        m_pChData[uBusChIndex].uCurResponseComNo = m_pChData[uBusChIndex].uNextResponseComNo;
        ulPulseID.ulDevComNO = m_pChData[uBusChIndex].uNextResponseComNo;
        m_pChData[uBusChIndex].pPulseCtrlWords[ulPulseID.ulDevComNO].nTick++;
        m_pChData[uBusChIndex].uNextResponseComNo++;

        if (m_pChData[uBusChIndex].uNextResponseComNo>=m_pChData[uBusChIndex].uCodedDevCount)
            m_pChData[uBusChIndex].uNextResponseComNo=0;
    }

    //发送脉冲指令数据包
    uIndex = 3;//重发次数

    while(uIndex)
    {
        ulRet = m_sCanComInterface.pf_CAN_SendPack(uBusChIndex, *((uBit32*)&ulPulseID), pPulseData, 8);

        if (ulRet == DVM_ERR_SUCCESS)
            break;

#ifdef RS_MONITOR_ENALBE
        m_sCanBusRsMsData.sCanRsMsData.nSendPulseFailTimes++;
#endif

        if (uIndex==0)//重发次数到还未发成功，则错误返回
        {

            for (uDevComNo=0; uDevComNo<m_pChData[uBusChIndex].uCodedDevCount; uDevComNo++)
            {
                pDevInfo = m_pChData[uBusChIndex].pDevInfo[uDevComNo];

                if (pDevInfo->nDeviceNO==DEV_INVALID_NO)
                    continue;

                pDevState = m_pCanDeviceState + pDevInfo->nDeviceNO;
                switch(pDevInfo->nDeviceType)
                {
                case DEV_TYPE_AXIS://轴设备
                case DEV_TYPE_SV://伺服设备
                    pDevState->AxisStateData.ulRunningStatus |= AXIS_STATUS_SEND_CYCLE_DATA_FAIL;
                    break;
                case DEV_TYPE_OUTPUT:    //输出设备
                case DEV_TYPE_IO:            //输入输出设备
                case DEV_TYPE_INPUT:        //输入设备
                    pDevState->IOStateData.cSysStatus |= IO_STATUS_SEND_CYCLE_DATA_FAIL;
                    break;
                case DEV_TYPE_ADDA:        //ADDA设备
                    pDevState->ADDAStateData.cSysStatus |= IO_STATUS_SEND_CYCLE_DATA_FAIL;
                    break;
                default:break;
                }
            }

#ifdef RS_MONITOR_ENALBE
            m_sCanBusRsMsData.sCanRsMsData.nSendRetryFailTimes++;
#endif
            return DVM_ERR_CMD_SEND;
        }

        uIndex--;

        //延时0.4us
        for (lPulseTemp=0; lPulseTemp<10000; lPulseTemp++);
    }

#ifdef SYS_FUN_TEST
    GPIO_FunTestEnd(DVM_CAN_SynCycleSend);
#endif
    return DVM_ERR_SUCCESS;
}













































































































