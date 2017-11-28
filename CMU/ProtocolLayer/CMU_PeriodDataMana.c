
#include "string.h"
#include "CMU_PeriodDataMana.h"
#include "CMU_SendDataProcess.h"
#include "CMU_CmdProcess.h"

#include "..\\..\\CNC\\DataStructDef\\SYS_DataStructDef.h"
#include "..\\..\\CNC\\DataStructDef\\CRD_DataStructDef.h"
#include "..\\..\\CNC\\DataStructDef\\IO_DataStructDef.h"

#include "..\\CMU_ErrorCodeDef.h"


#include "..\\UIN_GlobalDataDef.h"
//#include "..\\GPIO\\GPIOCtrl.h"



#define MAX_INQ_RECORD_COUNT            (32)    //最大允许周期性查询的个数
#define MIN_CONTINUE_CYCLE_GAP            (10)    //两周期性查询间最小循环间隔


typedef struct _INQ_RECORD_DATA
{
    uBit32 ulInqId;                    //查询ID
    uBit16 nCycle;                    //查询周期，单位ms
    double64 dPreTickCount;    //上次发送时周期数
}INQ_RECORD_DATA;

static BooLean m_bTickCountSwitch;
static uBit8 m_ucRecordCount;
static uBit8 m_ucCurRecordIndex; //当前周期准备处理的记录序号（每个循环周期加1）
static INQ_RECORD_DATA m_sInqRecordBuf[MAX_INQ_RECORD_COUNT];
static uBit8 m_ucCycleGap = 0;        //发送周期检查开关，周期性查询，连续查询循环周期间隔数

void CMU_PeriodDataManaInit()
{
    m_bTickCountSwitch = true;
    m_ucRecordCount=0;
    m_ucCurRecordIndex=0;                        //当前周期准备处理的记录序号
    memset(m_sInqRecordBuf,0, sizeof(INQ_RECORD_DATA)*MAX_INQ_RECORD_COUNT);
}

void CMU_PeriodSendSwitch(BooLean bOpen)
{
    m_bTickCountSwitch = bOpen;
}

//添加或修改查询记录
uBit32 CMU_AddInqRecord(COM_DATA_ID ulID, uBit32 ulCycle)
{
    uBit8 i;
    uBit8 ucRestCount;

    //查找同类型的记录，并修改
    for (i=0; i<m_ucRecordCount; i++)
    {
        if ((m_sInqRecordBuf[i].ulInqId&SAME_TYPE_PACK_MASK) == 
            (ulID.ulFrameID&SAME_TYPE_PACK_MASK))
        {
            if (ulCycle==0)//删除查询记录
            {
                ucRestCount = m_ucRecordCount-i-1;
                m_ucRecordCount -= 1;

                if (ucRestCount!=0)//非最后一个记录,后面的记录前移
                    memcpy(&m_sInqRecordBuf[i], &m_sInqRecordBuf[i+1], ucRestCount*sizeof(INQ_RECORD_DATA));
                else
                    memset(&m_sInqRecordBuf[i],0,sizeof(INQ_RECORD_DATA)); 
            }
            else //修改查询记录
                {
                    m_sInqRecordBuf[i].nCycle = ulCycle;
                    
                    if(m_sInqRecordBuf[i].nCycle<MIN_PERIOD_MS)
                      m_sInqRecordBuf[i].nCycle=MIN_PERIOD_MS;
                    
                    m_sInqRecordBuf[i].dPreTickCount = m_sExternalFunTable.pf_SYS_GetSysTickCount();
                }

            return CMU_ERR_SUCCESS;
        }
    }

    if(ulCycle==0)//取消查询时，但未找到匹配的查询记录，正确返回
       return CMU_ERR_SUCCESS;
        
    if (m_ucRecordCount==MAX_INQ_RECORD_COUNT)//无法再添加查询记录
        return CMU_ERR_INQ_OUTRANG;

    m_sInqRecordBuf[m_ucRecordCount].ulInqId = ulID.ulFrameID;
    m_sInqRecordBuf[m_ucRecordCount].nCycle = ulCycle;
    
    if(m_sInqRecordBuf[m_ucRecordCount].nCycle<MIN_PERIOD_MS)
      m_sInqRecordBuf[m_ucRecordCount].nCycle=MIN_PERIOD_MS;
    
    m_sInqRecordBuf[m_ucRecordCount].dPreTickCount = m_sExternalFunTable.pf_SYS_GetSysTickCount();
    m_ucRecordCount++;
    return CMU_ERR_SUCCESS;
}

 /*
函数名称：uBit32 INQ_SendSysPeriodDataProc(COM_DATA_ID ulID)

功    能：获取系统周期性数据并配置发送数据

参    数：ulID--通信ID

返 回 值：0--成功 非0--错误编号

注意事项：可有周期性数据发送处理函数 和 周期性数据获取指令处理函数中调用
*/  
uBit32 INQ_SendSysPeriodDataProc(COM_DATA_ID ulID)
{
    uBit32 ulRet;
    const SYS_STATE_DATA *pSysStateData;

    //获取系统状态数据时都使用本地发送缓冲区
    CMU_ResetSendCtrlData(ulID.ulFrameID, NULL, 0);

    pSysStateData = m_sExternalFunTable.pf_SYS_GetStateReadAddr();

    switch(ulID.ulComDataID.ulCmdIndex)
    {
        case SYS_CMD_INQ_ALL_STATE://系统全部状态
                ulRet = CMU_AddToSendCtrlData((uBit8*)pSysStateData, sizeof(SYS_STATE_DATA));
                break;
        case SYS_CMD_INQ_CTRL_STATUS://系统控制字
                ulRet = CMU_AddToSendCtrlData((uBit8*)&pSysStateData->ulSysCtrlStatus,sizeof(pSysStateData->ulSysCtrlStatus));
                break;
        case SYS_CMD_INQ_RUNNING_STATUS://系统运行状态字
                ulRet = CMU_AddToSendCtrlData((uBit8*)&pSysStateData->ulSysRunningStatus,sizeof(pSysStateData->ulSysRunningStatus));
                break;
        case SYS_CMD_INQ_EXT_ALARM://系统外部报警
                ulRet = CMU_AddToSendCtrlData((uBit8*)&pSysStateData->ulSysExtAlarm,sizeof(pSysStateData->ulSysExtAlarm));
                break;
        default: ulRet = CMU_ERR_INVALID_CMD;
                break;
    }

    return ulRet;
}

 /*
函数名称：uBit32 INQ_SendCrdPeriodDataProc(COM_DATA_ID ulID)

功    能：获取坐标系周期性数据并配置发送数据

参    数：ulID--通信ID

返 回 值：0--成功 非0--错误编号

注意事项：可有周期性数据发送处理函数 和 周期性数据获取指令处理函数中调用
*/  
uBit32 INQ_SendCrdPeriodDataProc(COM_DATA_ID ulID)
{
    uBit32 ulRet;
    uBit32 ulCrdMask;
    const CRDSYS_STATE_DATA *pCrdStateData;
    const DATA_BUF_MAN      *pCrdBufStateData;

    //获取坐标系周期性数据时使用本地发送缓冲区
    CMU_ResetSendCtrlData(ulID.ulFrameID, NULL, 0);
    ulCrdMask =ulID.ulComDataID.ulCrdNo;

    for(uBit8 ulCrdIndex=0; ulCrdIndex<CRDSYS_MAX_COUNT; ulCrdIndex++)
    {
        if ((ulCrdMask&(1<<(ulCrdIndex)))==0)
            continue;

        switch(ulID.ulComDataID.ulCmdIndex)
        {
            case CRD_CMD_INQ_ALL_STATE:
                pCrdStateData = m_sExternalFunTable.pf_CSM_GetCrdSysStateReadAddr(ulCrdIndex);
                ulRet = CMU_AddToSendCtrlData((uBit8*)pCrdStateData,sizeof(CRDSYS_STATE_DATA));
                break;
            case CRD_CMD_INQ_ACT_SPEED:                    //坐标系实际速度查询
                pCrdStateData = m_sExternalFunTable.pf_CSM_GetCrdSysStateReadAddr(ulCrdIndex);
                ulRet = CMU_AddToSendCtrlData((uBit8*)&pCrdStateData->dFeedRate,sizeof(pCrdStateData->dFeedRate));
                break;
            case CRD_CMD_INQ_CMD_SPEED:                    //坐标系设定速度查询
                pCrdStateData = m_sExternalFunTable.pf_CSM_GetCrdSysStateReadAddr(ulCrdIndex);
                ulRet = CMU_AddToSendCtrlData((uBit8*)&pCrdStateData->dProgFeedRate,sizeof(pCrdStateData->dProgFeedRate));
                break;
            case CRD_CMD_INQ_ACT_ACC:                    //坐标系实际加速度查询
                pCrdStateData = m_sExternalFunTable.pf_CSM_GetCrdSysStateReadAddr(ulCrdIndex);
                ulRet = CMU_AddToSendCtrlData((uBit8*)&pCrdStateData->dAcc,sizeof(pCrdStateData->dAcc));
                break;
            case CRD_CMD_INQ_RAPID_OVERRIDE:            //坐标系快移修调查询
                pCrdStateData = m_sExternalFunTable.pf_CSM_GetCrdSysStateReadAddr(ulCrdIndex);
                ulRet = CMU_AddToSendCtrlData((uBit8*)&pCrdStateData->dRapidOverride,sizeof(pCrdStateData->dRapidOverride));
                break;
            case CRD_CMD_INQ_FEED_OVERRIDE:                //坐标系进给修调查询
                pCrdStateData = m_sExternalFunTable.pf_CSM_GetCrdSysStateReadAddr(ulCrdIndex);
                ulRet = CMU_AddToSendCtrlData((uBit8*)&pCrdStateData->dFeedOverride,sizeof(pCrdStateData->dFeedOverride));
                break;
            case CRD_CMD_INQ_SPINDLE_SPEED:                //坐标系主轴速度查询
                pCrdStateData = m_sExternalFunTable.pf_CSM_GetCrdSysStateReadAddr(ulCrdIndex);
                ulRet = CMU_AddToSendCtrlData((uBit8*)&pCrdStateData->ulSpindleSpeed,sizeof(pCrdStateData->ulSpindleSpeed));
                break;
            case CRD_CMD_INQ_SPINDLE_OVERRIDE:            //坐标系主轴修调查询
                pCrdStateData = m_sExternalFunTable.pf_CSM_GetCrdSysStateReadAddr(ulCrdIndex);
                ulRet = CMU_AddToSendCtrlData((uBit8*)&pCrdStateData->dSpindleOverride,sizeof(pCrdStateData->dSpindleOverride));
                break;
            case CRD_CMD_INQ_RUNNING_STATUS:            //坐标系运行状态查询
                pCrdStateData = m_sExternalFunTable.pf_CSM_GetCrdSysStateReadAddr(ulCrdIndex);
                ulRet = CMU_AddToSendCtrlData((uBit8*)&pCrdStateData->ulStatus,sizeof(pCrdStateData->ulStatus));
                break;
            case CRD_CMD_INQ_CTRL_STATUS:               //坐标系控制状态查询
                pCrdStateData = m_sExternalFunTable.pf_CSM_GetCrdSysStateReadAddr(ulCrdIndex);
                ulRet = CMU_AddToSendCtrlData((uBit8*)&pCrdStateData->ulCtrl,sizeof(pCrdStateData->ulCtrl));
                break;
            case CRD_CMD_INQ_ALARM:                        //坐标系报警状态查询
                pCrdStateData = m_sExternalFunTable.pf_CSM_GetCrdSysStateReadAddr(ulCrdIndex);
                ulRet = CMU_AddToSendCtrlData((uBit8*)&pCrdStateData->ulAlarm,sizeof(pCrdStateData->ulAlarm));
                break;
           // case CRD_CMD_INQ_MST_STATUS:                //坐标系MST运行状态字
           //        ulRet = CMU_AddToSendCtrlData((uBit8*)&pCrdStateData->ulMSTStatus,sizeof(pCrdStateData->ulMSTStatus));
           //        break;
            case CRD_CMD_INQ_M_CODE:                    //坐标系当前运行的M码
                pCrdStateData = m_sExternalFunTable.pf_CSM_GetCrdSysStateReadAddr(ulCrdIndex);
                ulRet = CMU_AddToSendCtrlData((uBit8*)&pCrdStateData->nMCode,sizeof(pCrdStateData->nMCode));
                break;
            case CRD_CMD_INQ_S_CODE:                    //坐标系当前运行的S码
                pCrdStateData = m_sExternalFunTable.pf_CSM_GetCrdSysStateReadAddr(ulCrdIndex);
                ulRet = CMU_AddToSendCtrlData((uBit8*)&pCrdStateData->ulSCode,sizeof(pCrdStateData->ulSCode));
                break;
            case CRD_CMD_INQ_T_CODE:                    //坐标系当前运行的T码
                pCrdStateData = m_sExternalFunTable.pf_CSM_GetCrdSysStateReadAddr(ulCrdIndex);
                ulRet = CMU_AddToSendCtrlData((uBit8*)&pCrdStateData->cTCode,sizeof(pCrdStateData->cTCode));
                break;
            case CRD_CMD_INQ_B_CODE:                    //坐标系当前运行的B码
                pCrdStateData = m_sExternalFunTable.pf_CSM_GetCrdSysStateReadAddr(ulCrdIndex);
                ulRet = CMU_AddToSendCtrlData((uBit8*)&pCrdStateData->cBCode,sizeof(pCrdStateData->cBCode));
                break;
            case CRD_CMD_INQ_PROG_LINENO:
                pCrdStateData = m_sExternalFunTable.pf_CSM_GetCrdSysStateReadAddr(ulCrdIndex);
                ulRet = CMU_AddToSendCtrlData((uBit8*)&pCrdStateData->ulCurProgLineNO,sizeof(pCrdStateData->ulCurProgLineNO));
                break;
            case CRD_CMD_INQ_USER_LINENO:                //坐标系程序用户自定义行号
                pCrdStateData = m_sExternalFunTable.pf_CSM_GetCrdSysStateReadAddr(ulCrdIndex);
                ulRet = CMU_AddToSendCtrlData((uBit8*)&pCrdStateData->ulUserLineNO,sizeof(pCrdStateData->ulUserLineNO));
                break;
            case CRD_CMD_INQ_PROG_BUF_STATE:
                pCrdBufStateData = m_sExternalFunTable.pf_CSM_GetProgBufStateReadAddr(ulCrdIndex);
                ulRet = CMU_AddToSendCtrlData((uBit8*)pCrdBufStateData, sizeof(DATA_BUF_MAN));
                break;
            case CRD_CMD_INQ_PROG_BUF_SIZE:                //坐标系程序缓冲区总大小查询
                pCrdBufStateData = m_sExternalFunTable.pf_CSM_GetProgBufStateReadAddr(ulCrdIndex);
                ulRet = CMU_AddToSendCtrlData((uBit8*)&pCrdBufStateData->ulSize, sizeof(pCrdBufStateData->ulSize));
                break;
            case CRD_CMD_INQ_PROG_BUF_STATUS:            //坐标系程序缓冲区状态查询
                pCrdBufStateData = m_sExternalFunTable.pf_CSM_GetProgBufStateReadAddr(ulCrdIndex);
                ulRet = CMU_AddToSendCtrlData((uBit8*)&pCrdBufStateData->ulStatus, sizeof(pCrdBufStateData->ulStatus));
                break;
            case CRD_CMD_INQ_USER_BUF_STATE:
                pCrdBufStateData = m_sExternalFunTable.pf_CSM_GetUserBufStateReadAddr(ulCrdIndex);
                ulRet = CMU_AddToSendCtrlData((uBit8*)pCrdBufStateData, sizeof(DATA_BUF_MAN));
                break;
            case CRD_CMD_INQ_USER_BUF_SIZE:                //坐标系用戶缓冲区总大小查询
                pCrdBufStateData = m_sExternalFunTable.pf_CSM_GetUserBufStateReadAddr(ulCrdIndex);
                ulRet = CMU_AddToSendCtrlData((uBit8*)&pCrdBufStateData->ulSize, sizeof(pCrdBufStateData->ulSize));
                break;
            case CRD_CMD_INQ_USER_BUF_STATUS:            //坐标系用戶缓冲区状态查询
                pCrdBufStateData = m_sExternalFunTable.pf_CSM_GetUserBufStateReadAddr(ulCrdIndex);
                ulRet = CMU_AddToSendCtrlData((uBit8*)&pCrdBufStateData->ulStatus, sizeof(pCrdBufStateData->ulStatus));
                break;
            default:
                    return CMU_ERR_INVALID_CMD;
        }
    }
    
    return ulRet;
}

 /*
函数名称：uBit32 INQ_AddMotorPeriodData(COM_DATA_ID ulID)

功    能：获取电机周期性数据并配置发送数据

参    数：ulID--通信ID

返 回 值：0--成功 非0--错误编号

注意事项：可由周期性数据发送处理函数 和 周期性数据获取指令处理函数中调用
*/  
uBit32 INQ_AddMotorPeriodData(COM_DATA_ID ulID, AXIS_STATE_DATA const *pAxisState)
{
    uBit32 ulRet = CMU_ERR_INVALID_CMD;

    switch(ulID.ulComDataID.ulCmdIndex)
    {
        case MOTOR_CMD_INQ_ALL_STATE:                //电机所有状态
            {  
                ulRet = CMU_AddToSendCtrlData((uBit8*)pAxisState, sizeof(AXIS_STATE_DATA));
                break;
            }
        case MOTOR_CMD_INQ_CTRL_STATUS:                //电机控制状态字
            {
                ulRet = CMU_AddToSendCtrlData((uBit8*)&pAxisState->ulCtrlStatus, sizeof(uBit32));
                break;
            }
        case MOTOR_CMD_INQ_RUNNING_STATUS:            //电机运行状态字
            {
                ulRet = CMU_AddToSendCtrlData((uBit8*)&pAxisState->ulRunningStatus, sizeof(uBit32));
                break;
            }
        case MOTOR_CMD_INQ_QEI_POS:                    //电机编码器位置查询
            {
                ulRet = CMU_AddToSendCtrlData((uBit8*)&pAxisState->lQeiPos, sizeof(Bit32));
                break;
            }
        case MOTOR_CMD_INQ_QEI_SPEED:                //电机编码器速度查询
            {
                ulRet = CMU_AddToSendCtrlData((uBit8*)&pAxisState->dQeiSpeed, sizeof(float32));
                break;
            }
        case MOTOR_CMD_INQ_CMD_POS:                    //电机指令位置查询
            {
                ulRet = CMU_AddToSendCtrlData((uBit8*)&pAxisState->dCmdPos, sizeof(double64));
                break;
            }
        case MOTOR_CMD_INQ_CMD_SPEED:                //电机指令速度查询
            {
                ulRet = CMU_AddToSendCtrlData((uBit8*)&pAxisState->dCmdSpeed, sizeof(float32));
                break;
            }
        case MOTOR_CMD_INQ_CMD_ACC:                    //电机加速度查询
            {
                ulRet = CMU_AddToSendCtrlData((uBit8*)&pAxisState->dAcc, sizeof(float32));
                break;
            }
        case MOTOR_CMD_INQ_SV_ALRM:                    //电机伺服报警查询
            {
                ulRet = CMU_AddToSendCtrlData((uBit8*)&pAxisState->ulSvAlarm, sizeof(uBit32));
                break;
            }
        case MOTOR_CMD_INQ_SV_STATUS:                    //电机伺服状态查询
            {
                ulRet = CMU_AddToSendCtrlData((uBit8*)&pAxisState->ulSvStatus, sizeof(uBit32));
                break;
            }
        default: 
            ulRet = CMU_ERR_INVALID_CMD;
    }

    return ulRet;
}


 /*
函数名称：uBit32 INQ_SendAxisPeriodDataProc(COM_DATA_ID ulID)

功    能：获取轴周期性数据并配置发送数据

参    数：ulID--通信ID

返 回 值：0--成功 非0--错误编号

注意事项：可由周期性数据发送处理函数 和 周期性数据获取指令处理函数中调用
*/  
uBit32 INQ_SendAxisPeriodDataProc(COM_DATA_ID ulID)
{
    uBit32 ulRet = CMU_ERR_INVALID_CMD;
    const DEVICE_STATE *pDeviceState;

    //获取坐标系周期性数据时使用本地发送缓冲区
    CMU_ResetSendCtrlData(ulID.ulFrameID, NULL, 0);

    if (ulID.ulComDataID.ulCrdNo>=CRDSYS_MAX_COUNT)//ulDevNo域存放的是电机设备编号
    {
        pDeviceState = m_sExternalFunTable.pf_DEV_GetStateReadAddr(ulID.ulComDataID.ulDevNo);
        ulRet = INQ_AddMotorPeriodData(ulID, &pDeviceState->AxisStateData);
    }
    else//ulDevNo域存放的是轴屏蔽字
        {
            uBit32 ulAxisMask;
            uBit32 ulMotorNo;
            const CRDSYS_AXIS_MAP_TABLE* pAxisMap=NULL;

            pAxisMap = m_sExternalFunTable.pf_SPM_GetCrdSysAxisMapTableAddr();
            pAxisMap += ulID.ulComDataID.ulCrdNo;
            ulAxisMask = ulID.ulComDataID.ulDevNo;

            for (uBit32 ulAxisIndex=0; ulAxisIndex<pAxisMap->nCrdAxisCount; ulAxisIndex++)
            {
                if((ulAxisMask&(1<<ulAxisIndex)) == 0)
                    continue;

                ulMotorNo = pAxisMap->CrdSysAxisMap[ulAxisIndex].cMotorNO[0];

               // if (ulMotorNo >= AXIS_MAX_COUNT)
               //     return CMU_ERR_DATA_WRONG;

                pDeviceState = m_sExternalFunTable.pf_DEV_GetStateReadAddr(ulMotorNo);

                ulRet = INQ_AddMotorPeriodData(ulID, &pDeviceState->AxisStateData);

                if (ulRet!=CMU_ERR_SUCCESS)
                    break;
            }
        }

    return ulRet;
}


 /*
函数名称：uBit32 INQ_SendADDAPeriodDataProc(COM_DATA_ID ulID)

功    能：获取ADDA周期性数据并配置发送数据

参    数：ulID--通信ID

返 回 值：0--成功 非0--错误编号

注意事项：可由周期性数据发送处理函数 和 周期性数据获取指令处理函数中调用
*/  
uBit32 INQ_SendADDAPeriodDataProc(COM_DATA_ID ulID)
{
    uBit32 ulRet = CMU_ERR_INVALID_CMD;
    const DEVICE_STATE *pDeviceState;

    //获取坐标系周期性数据时使用本地发送缓冲区
    CMU_ResetSendCtrlData(ulID.ulFrameID, NULL, 0);

    pDeviceState = m_sExternalFunTable.pf_DEV_GetStateReadAddr(ulID.ulADDADataID.ulADDANo);

    switch(ulID.ulADDADataID.ulCmdIndex)
    {
        case ADDA_CMD_INQ_ALL_STATE://ADDA板所有状态
            {
                ulRet = CMU_AddToSendCtrlData((uBit8*)&pDeviceState->ADDAStateData, sizeof(ADDA_STATE_DATA));
                break;
            }
        case ADDA_CMD_INQ_SYS_STATUS://ADDA板系统状态
            {
                ulRet = CMU_AddToSendCtrlData((uBit8*)&pDeviceState->ADDAStateData.cSysStatus, sizeof(uBit8));
                break;
            }
        case ADDA_CMD_INQ_AD_STATUS://ADDA板AD通道输入状态
            {
                for (uBit32 ulChIndex=0; ulChIndex<AD_CHANNEL_COUNT; ulChIndex++)
                {
                    if ((ulID.ulADDADataID.ulChMask&(1<<ulChIndex))==0)
                        continue;

                    ulRet = CMU_AddToSendCtrlData((uBit8*)&pDeviceState->ADDAStateData.nADStatus[ulChIndex], sizeof(uBit16));
                    
                    if (ulRet!=CMU_ERR_SUCCESS)
                        break;
                }
                break;
             }
        case ADDA_CMD_INQ_DA_STATUS://ADDA板DA通道输出状态
            {
                for (uBit32 ulChIndex=0; ulChIndex<DA_CHANNEL_COUNT; ulChIndex++)
                {
                    if ((ulID.ulADDADataID.ulChMask&(1<<ulChIndex))==0)
                        continue;

                    ulRet = CMU_AddToSendCtrlData((uBit8*)&pDeviceState->ADDAStateData.nDAStatus[ulChIndex], sizeof(uBit16));

                    if (ulRet!=CMU_ERR_SUCCESS)
                        break;
                }
                break;
            }
        case ADDA_CMD_INQ_DA_CMD:    //ADDA板DA通道指令输出状态
            {
                for (uBit32 ulChIndex=0; ulChIndex<DA_CHANNEL_COUNT; ulChIndex++)
                {
                    if ((ulID.ulADDADataID.ulChMask&(1<<ulChIndex))==0)
                        continue;

                    ulRet = CMU_AddToSendCtrlData((uBit8*)&pDeviceState->ADDAStateData.nDACmdStatus[ulChIndex], sizeof(uBit16));

                    if (ulRet!=CMU_ERR_SUCCESS)
                        break;
                }
                break;
            }   
        default: 
            break;
    }

    return ulRet;
}


 /*
函数名称：uBit32 INQ_SendIoPeriodDataProc(COM_DATA_ID ulID)

功    能：获取IO周期性数据并配置发送数据

参    数：ulID--通信ID

返 回 值：0--成功 非0--错误编号

注意事项：可由周期性数据发送处理函数 和 周期性数据获取指令处理函数中调用
*/  
uBit32 INQ_SendIoPeriodDataProc(COM_DATA_ID ulID)
{
    uBit32 ulRet = CMU_ERR_INVALID_CMD;
    const DEVICE_STATE *pDeviceState;

    //获取坐标系周期性数据时使用本地发送缓冲区
    CMU_ResetSendCtrlData(ulID.ulFrameID, NULL, 0);

    pDeviceState = m_sExternalFunTable.pf_DEV_GetStateReadAddr(ulID.ulComDataID.ulDevNo);

    switch(ulID.ulComDataID.ulCmdIndex)
    {
        case IO_CMD_INQ_ALL_STATE:
            {
                ulRet = CMU_AddToSendCtrlData((uBit8*)&pDeviceState->IOStateData, sizeof(IO_STATE_DATA));
                break;
            }
        case IO_CMD_INQ_SYS_STATUS:                    //获取IO板系统状态
            {
                ulRet = CMU_AddToSendCtrlData((uBit8*)&pDeviceState->IOStateData.cSysStatus, sizeof(uBit8));
                break;
            }
        case IO_CMD_INQ_INPUT_STATUS:                //获取IO板端口状态
            {
                ulRet = CMU_AddToSendCtrlData((uBit8*)&pDeviceState->IOStateData.nInputStatus, sizeof(uBit32));
                break;
            }
        case IO_CMD_INQ_OUTPUT_STATUS:            //获取IO板端口状态
            {
                ulRet = CMU_AddToSendCtrlData((uBit8*)&pDeviceState->IOStateData.nOutputStatus, sizeof(uBit32));
                break;
            }
        case IO_CMD_INQ_OUTPUT_CMD:            //IO板系统状态
            {
                ulRet = CMU_AddToSendCtrlData((uBit8*)&pDeviceState->IOStateData.nOutputCmdStatus, sizeof(uBit32));
                break;
            }
        default:
            break;
    }


    return ulRet;
}

 /*
函数名称：uBit32 CMU_SendPeriodDataProc(COM_DATA_ID ulID)

功    能：

参    数：ulID--通信ID

返 回 值：0--成功 非0--错误编号

注意事项：可由周期性数据发送处理函数 和 周期性数据获取指令处理函数中调用
*/  
uBit32 CMU_SendPeriodDataProc(COM_DATA_ID ulID)
{
    uBit32 ulRet=CMU_ERR_INVALID_CMD;

    switch(ulID.ulComDataID.ulDataType)
    {
        case DATA_TYPE_SYS:                                //系统数据
                ulRet = INQ_SendSysPeriodDataProc(ulID);
                break;
        case DATA_TYPE_CRD:                                //坐标系数据
                ulRet = INQ_SendCrdPeriodDataProc(ulID);    
                break;
        case DATA_TYPE_AXIS:                            //轴数据
                ulRet = INQ_SendAxisPeriodDataProc(ulID);    
                break;
        case DATA_TYPE_IO:                                //IO数据
                ulRet = INQ_SendIoPeriodDataProc(ulID);    
                break;
        case DATA_TYPE_ADDA:                            //ADDA数据
                ulRet = INQ_SendADDAPeriodDataProc(ulID);    
                break;
        default:
                break;
    }

    return ulRet;
}

/*
函数名称：uBit32 CMU_IsInqTimeUp(void)

功    能：检查当前查询记录的查询时间到

参    数：无

返 回 值：1--时间到 0--时间未到

注意事项：
*/  
uBit32 CMU_IsInqTimeUp(void)
{
    uBit32 ulMsTick;
    uBit32 ulTickGap;
    double dCurSysTickCount=0;
    const SYS_CTRL_PARM *pSysCtrlParm;

    pSysCtrlParm = m_sExternalFunTable.pf_SPM_GetSysCtrlParmAddr();
    dCurSysTickCount = m_sExternalFunTable.pf_SYS_GetSysTickCount();


    //添加获取当前系统时钟
    ulTickGap = (uBit32)(dCurSysTickCount - m_sInqRecordBuf[m_ucCurRecordIndex].dPreTickCount);

    if (ulTickGap)
    {
        ulMsTick = ulTickGap*pSysCtrlParm->nTimeCycle;
        ulMsTick /= 1000;

        if (ulMsTick>=m_sInqRecordBuf[m_ucCurRecordIndex].nCycle)
            return 1;
    }

    return 0;
}


//周期性数据查询处理主线程
uBit32 CMU_MainPeriodInqProc()
{
    uBit32 ulRet=CMU_ERR_SUCCESS;
    COM_DATA_ID ulId;

    if(m_bTickCountSwitch==false)
        return CMU_ERR_SUCCESS;

    if (m_ucRecordCount==0)
        return CMU_ERR_SUCCESS;

    m_ucCycleGap++;
    
    if (m_ucCycleGap<MIN_CONTINUE_CYCLE_GAP)
        return CMU_ERR_SUCCESS;

    if (m_ucCurRecordIndex==m_ucRecordCount)
        m_ucCurRecordIndex = 0;

    //当前记录查询时间到
#ifdef SYS_FUN_TEST
    GPIO_FunTestStart(CMU_CMU_MainPeriodInqProc);
#endif
    if (CMU_IsInqTimeUp()==1)
    {
        ulId.ulFrameID = m_sInqRecordBuf[m_ucCurRecordIndex].ulInqId;
        m_sInqRecordBuf[m_ucCurRecordIndex].dPreTickCount = m_sExternalFunTable.pf_SYS_GetSysTickCount();
        ulId.ulFrameID &= SAME_TYPE_PACK_MASK;
        ulId.ulComDataID.ulPeriodFlag = 1;
        ulId.ulComDataID.ulOperType = DATA_OPRE_TYPE_GET;
        ulId.ulComDataID.ulCmdType = GETCMD_TYPE_INQ;
        m_ucCurRecordIndex++;  
        m_ucCycleGap=0;
        ulRet = CMU_SendPeriodDataProc(ulId);
    }

#ifdef SYS_FUN_TEST
    GPIO_FunTestEnd(CMU_CMU_MainPeriodInqProc);
#endif
    return ulRet;
}

//主动向上位机发送错误处理
void CMU_ReportErrorProc()
{
    uBit32 ulError;
    if (m_sExternalFunTable.pf_ECM_GetErrorCode(&ulError)==0)//成功获取错误
    {
        COM_DATA_ID ulID;
        memset(&ulID, 0, sizeof(COM_DATA_ID));
        ulID.ulComDataID.ulOperType = DATA_OPRE_TYPE_GET;    //获取数据
        ulID.ulComDataID.ulDataType = DATA_TYPE_SYS;        //系统数据
        ulID.ulComDataID.ulCmdType = GETCMD_TYPE_INQ;        //周期新数据获取指令
        ulID.ulComDataID.ulCmdIndex = SYS_CMD_INQ_ERROR;    //下位机主动发送的错误
        ulID.ulComDataID.ulPeriodFlag = PERIOD_DATA_FLAGS;    //下位机主动发送的错误
        ulID.ulComDataID.ulTransmitFrame = TRANSMIT_SELF_FRAME;
        ulID.ulComDataID.ulReceiveObj = SCR_RECEIVE;
        CMU_SendResponsePack(ulID.ulFrameID, ulError);
    }
}
