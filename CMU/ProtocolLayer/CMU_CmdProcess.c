
#include "string.h"
#include "CMU_CmdProcess.h"
#include "CMU_SendDataProcess.h"
#include "CMU_PeriodDataMana.h"

#include "CNC/DVM/DEV_COM_CmdDef.h"

#include "CNC/DataStructDef/SYS_DataStructDef.h"
#include "CNC/DataStructDef/CRD_DataStructDef.h"
#include "CNC/DataStructDef/DEV_DataStructDef.h"
#include "CNC/DataStructDef/IO_DataStructDef.h"

#include "../CMU_ErrorCodeDef.h"


#include "../UIN_GlobalDataDef.h"
#include "../AbstractionLayer/AbstractionLayer.h"

//#define DISABLE_IPR_INTERFACE //解释器接口暂时注释掉Alan???

#ifdef RS_MONITOR_ENALBE        
CMU_RS_MS_DATA g_sCmuRsMonitorData;
#endif



//------------升级文件数据管理---------------
uBit32 Sys_SetNomalCmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData);
uBit32 Sys_GetNomalCmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData);
uBit32 Sys_CmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData);

uBit32 Crd_NomalSetCmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData);
uBit32 Crd_NomalGetCmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData);
uBit32 Crd_CmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData);

uBit32 Motor_NomalSetCmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData);
uBit32 Motor_NomalGetCmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData);
uBit32 Motor_CmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData);

uBit32 Io_NomalSetCmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData);
uBit32 Io_NomalGetCmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData);
uBit32 Io_CmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData);

uBit32 Adda_NomalSetCmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData);
uBit32 Adda_NomalGetCmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData);
uBit32 Adda_CmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData);

uBit32 Reg_CmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData);
uBit32 Servo_CmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData);

 /*
函数名称:uBit32 Sys_SetNomalCmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData)

功    能:系统常规设置指令处理线程,处理的指令包括
            #define SYS_SETCMD_COM_STYLE                    (1) //通信类型
            #define SYS_SETCMD_BLINK_DEV_LED                (2) //测试板卡
            #define SYS_SETCMD_DEVICE_CONF                  (3) //设置硬件配置
            #define SYS_SETCMD_CTRL_PARM                    (4) //设置系统控制参数
            #define SYS_SETCMD_UPDATE_SLC                   (7) //SLC升级数据
            #define SYS_SETCMD_UPDATE_IPO                   (8) //IPO升级数据
            #define SYS_SETCMD_UPDATE_DEV                   (9) //设备升级数据
            #define SYS_SETCMD_UPDATE_SLCIPO_BOOT           (10)//SLCIPObootloader升级数据
            #define SYS_SETCMD_UPDATE_DEV_BOOT              (11)//设备bootloader升级数据
参    数:pRcvCtrlData--接收控制数据区

返 回 值:0--成功
          非0--错误码
注意事项:系统指令处理线程调用
*/ 
uBit32 Sys_SetNomalCmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData)
{
    uBit32 ulComType = 0;
    uBit32 ulRet = CMU_ERR_INVALID_CMD;
    
    (void)ulComType;
    
    switch(pRcvCtrlData->ulRevID.ulComDataID.ulCmdIndex)
    {
    case SYS_SETCMD_UPDATE_SLC:                         //SLC升级数据
        ulRet = m_sExternalFunTable.pf_SYS_UpdateSLC((uBit8*)pRcvCtrlData->pRevBuf, 
            pRcvCtrlData->ulRevLen);
        break;
    case SYS_SETCMD_UPDATE_IPO:                         //IPO升级数据
        ulRet = m_sExternalFunTable.pf_SYS_UpdateIPO((uBit8*)pRcvCtrlData->pRevBuf, 
            pRcvCtrlData->ulRevLen);
        break;
    case SYS_SETCMD_COM_STYLE:                          //通信类型
        {
            break;
        }
    case SYS_SETCMD_BLINK_DEV_LED:                    //测试板卡
        ulRet = m_sExternalFunTable.pf_DEV_BlinkLED(*(uBit32*)pRcvCtrlData->pRevBuf);
        break;
    case SYS_SETCMD_DEVICE_CONF:                    //设置硬件配置
        ulRet = m_sExternalFunTable.pf_DEV_SetConfig((DEVICE_CONFIG*)pRcvCtrlData->pRevBuf,\
            pRcvCtrlData->ulRevID.ulComDataID.ulDevNo);
        break;
    case SYS_SETCMD_CTRL_PARM:                        //设置系统控制参数
        ulRet = m_sExternalFunTable.pf_SPM_SetSysCtrlParm((SYS_CTRL_PARM*)pRcvCtrlData->pRevBuf);
        break;
    case SYS_SETCMD_UPDATE_DEV:                        //设备升级数据
        {     
            ulRet = m_sExternalFunTable.pf_SYS_PreUpdateDevProc((Bit8*)pRcvCtrlData->pRevBuf,pRcvCtrlData->ulRevLen);

            if (ulRet != CMU_ERR_SUCCESS)
                break;;

            ulRet = m_sExternalFunTable.pf_DEV_SendUpdateAppData(pRcvCtrlData->ulRevID.ulComDataID.ulDevNo, 
                pRcvCtrlData->pRevBuf, 
                pRcvCtrlData->ulRevLen);

            if (ulRet == CMU_ERR_SUCCESS)
                m_sExternalFunTable.pf_SYS_PostUpdateDevProc((Bit8*)pRcvCtrlData->pRevBuf,pRcvCtrlData->ulRevLen);
        }
        break;
    case SYS_SETCMD_UPDATE_SLCIPO_BOOT:                //SLCIPObootloader升级数据
        ulRet = m_sExternalFunTable.pf_SYS_UpdateBootloader((Bit8*)pRcvCtrlData->pRevBuf, 
                                                    pRcvCtrlData->ulRevLen);
        break;
    case SYS_SETCMD_UPDATE_DEV_BOOT:                //设备bootloader升级数据
        {
            ulRet = m_sExternalFunTable.pf_SYS_PreUpdateDevProc((Bit8*)pRcvCtrlData->pRevBuf,pRcvCtrlData->ulRevLen);

            if (ulRet != CMU_ERR_SUCCESS)
                break;;

            ulRet = m_sExternalFunTable.pf_DEV_SendUpdateBootData(pRcvCtrlData->ulRevID.ulComDataID.ulDevNo,
                pRcvCtrlData->pRevBuf, 
                pRcvCtrlData->ulRevLen);

            if (ulRet == CMU_ERR_SUCCESS)
                m_sExternalFunTable.pf_SYS_PostUpdateDevProc((Bit8*)pRcvCtrlData->pRevBuf,pRcvCtrlData->ulRevLen);
        }
        break;
    case SYS_SETCMD_IPR_ENABLE://使能或禁止解释器
        {//解释器接口暂时注释掉Alan???
            //if (m_sExternalFunTable.pf_SYS_IPREnable(*(uBit32*)pRcvCtrlData->pRevBuf)==0)//使能成功
            //    ulRet = CMU_ERR_SUCCESS;
            //else
            //    ulRet = CMU_ERR_IPR_ERROR;
        }
        break;
    case SYS_SETCMD_DEV_SCAN://系统扫描
        ulRet = m_sExternalFunTable.pf_DEV_Scan();
        break;
    case SYS_SETCMD_SAVE_PARM://控制器保存控制参数
        ulRet = m_sExternalFunTable.pf_SPM_SaveParm();
        break;
    case SYS_SETCMD_SAVE_CONF://控制器保存配置信息
        ulRet = m_sExternalFunTable.pf_SPM_SaveConfig();
        break;
    case SYS_SETCMD_SYS_RESET://系统复位
        m_sExternalFunTable.pf_SYS_Reset();
        break;
        
    case SYS_SETCMD_APP_FLAG_CLEAR:         //APP存在标志清除 2017.12.19 Duhanfeng]
        ulRet = m_sExternalFunTable.pf_SYS_ClearAppFlag();
        break;
        
    case SYS_SETCMD_SUB_APP_FLAG_CLEAR:     //副APP存在标志清除 2017.12.19 Duhanfeng]
        ulRet = m_sExternalFunTable.pf_SYS_ClearSubAppFlag();
        break;
        
    default:break;
    }

    return ulRet;
}


 /*
函数名称:uBit32 Sys_GetNomalCmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData)

功    能:系统常规获取指令处理线程,如果获取成功则配置回包,指令包括
        #define SYS_GETCMD_SLC_VER                            (1)//获取SLC版本
        #define SYS_GETCMD_IPO_VER                            (2)//获取IPO版本
        #define SYS_GETCMD_DEVICE_VER                        (3)//获取AXIS\IO\ADDA版本
        #define SYS_GETCMD_BUS_DRV_VER                    (4)//获取AXIS\IO\ADDA版本
        #define SYS_GETCMD_DEVICE_COUNT                        (5)//获取设备总数
        #define SYS_GETCMD_DEVICE_INFO                        (6)//获取硬件配置
        #define SYS_GETCMD_CTRL_PARM                        (7)//获取系统控制数据
参    数:pRcvCtrlData--接收控制数据区

返 回 值:0--成功
          非0--错误码
注意事项:系统指令处理线程调用
*/ 
uBit32 Sys_GetNomalCmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData)
{
    uBit32 ulNo;
    uBit32 ulLen;
    uBit32 ulCount;
    uBit8* pSendBuf;
    const SYS_CTRL_PARM *pSysCtrlParm;
    uBit32 ulRet = CMU_ERR_INVALID_CMD;
    
#ifdef RS_MONITOR_ENALBE   
    CTRL_RS_MS_DATA *pRsMsData; 
#endif

    CMU_ResetSendCtrlData(pRcvCtrlData->ulRevID.ulFrameID, NULL, 0);
    pSendBuf = CMU_GetSendBufAddr();

    switch(pRcvCtrlData->ulRevID.ulComDataID.ulCmdIndex)
    {
    case SYS_GETCMD_SLC_VER:                        //获取SLC版本
        {
            ulRet = m_sExternalFunTable.pf_SYS_GetSLCVersion((SOFTWARE_VERSION*)pSendBuf);

            if (ulRet==CMU_ERR_SUCCESS)
                CMU_AddToSendCtrlData(NULL,sizeof(SOFTWARE_VERSION));

            break;
        }
    case SYS_GETCMD_IPO_VER:                        //获取IPO版本
        {
            ulRet = m_sExternalFunTable.pf_SYS_GetIPOVersion((SOFTWARE_VERSION*)pSendBuf);

            if (ulRet==CMU_ERR_SUCCESS)
                CMU_AddToSendCtrlData(NULL,sizeof(SOFTWARE_VERSION));

            break;
        }
    case SYS_GETCMD_DEVICE_VER:                        //获取AXIS\IO\ADDA版本
        {
            ulRet = m_sExternalFunTable.pf_DEV_GetVersion(pRcvCtrlData->ulRevID.ulComDataID.ulDevNo, 
                (SOFTWARE_VERSION*)pSendBuf);

            if (ulRet==CMU_ERR_SUCCESS)
                CMU_AddToSendCtrlData(NULL,sizeof(SOFTWARE_VERSION));

            break;
        }
    case SYS_GETCMD_BUS_DRV_VER:                    //获取总线驱动版本
        {
            ulRet = m_sExternalFunTable.pf_DEV_GetBusDriverVersion(pRcvCtrlData->ulRevID.ulComDataID.ulDevNo,
                (SOFTWARE_VERSION*)pSendBuf);

            if (ulRet==CMU_ERR_SUCCESS)
                CMU_AddToSendCtrlData(NULL,sizeof(SOFTWARE_VERSION));

            break;
        }
    case SYS_GETCMD_DEVICE_COUNT:                    //获取设备总数
        {
            *((uBit32*)pSendBuf) = m_sExternalFunTable.pf_DEV_GetCount();
            ulRet = CMU_ERR_SUCCESS;
            CMU_AddToSendCtrlData(NULL,sizeof(uBit32));
            break;
        }
    case SYS_GETCMD_DEVICE_INFO:                    //获取硬件信息
        {
            ulRet = CMU_ERR_SUCCESS;
            ulCount = pRcvCtrlData->ulRevID.ulComDataID.ulDevNo;//获取要获取设备的总数
            ulLen = ulCount*sizeof(DEVICE_INFO);

            if (CMU_CheckSendBufFreeSize(ulLen)==false)
            {
                ulRet = CMU_ERR_SEND_OVERFLOW;
                break;
            }

            for (ulNo=0; ulNo<ulCount; ulNo++)
            {
                ulRet = m_sExternalFunTable.pf_DEV_GetInfo(ulNo, (DEVICE_INFO*)pSendBuf);

                if (ulRet==CMU_ERR_SUCCESS)
                    CMU_AddToSendCtrlData(NULL,sizeof(DEVICE_INFO));
                else
                    break;
                
                pSendBuf = CMU_GetSendBufAddr();
            }

            break;
        }
    case SYS_GETCMD_CTRL_PARM:                        //获取系统控制数据
        {
            ulRet = CMU_ERR_SUCCESS;
            pSysCtrlParm = m_sExternalFunTable.pf_SPM_GetSysCtrlParmAddr();
            CMU_ResetSendCtrlData(pRcvCtrlData->ulRevID.ulFrameID, (uBit8*)pSysCtrlParm, sizeof(SYS_CTRL_PARM));            
            break;
        }
#ifdef RS_MONITOR_ENALBE        
    case SYS_GETCMD_CTRL_RS_MS_DATA:                        //获取CMU模块通信监测数据
        {

            ulRet = CMU_ERR_SUCCESS;
            pRsMsData = (CTRL_RS_MS_DATA*)pSendBuf;

            pRsMsData->sCmuRsMsData = g_sCmuRsMonitorData;
            m_sExternalFunTable.pf_DEV_GetBusRsMsData(DEV_BUS_TYPE_CAN, &pRsMsData->sCanRsMsData);
            CMU_AddToSendCtrlData(NULL,sizeof(CTRL_RS_MS_DATA));
            break;
        }
#endif
    case SYS_GETCMD_CSM_VER:                        //获取CSM版本
        {
            ulRet = m_sExternalFunTable.pf_CSM_GetVersion((SOFTWARE_VERSION*)pSendBuf);

            if (ulRet==CMU_ERR_SUCCESS)
                CMU_AddToSendCtrlData(NULL,sizeof(SOFTWARE_VERSION));

            break;
        }
    case SYS_GETCMD_HQEIPOS:                        //获取高速编码器值
        {
            *(Bit32*)pSendBuf = m_sExternalFunTable.pf_QEI_GetHSpdQeiPos();
            CMU_AddToSendCtrlData(NULL,sizeof(Bit32));
            ulRet = CMU_ERR_SUCCESS;
            break;
        }
    case SYS_GETCMD_HQEISPD:                        //获取高速编码器值
        {
            *(Bit32*)pSendBuf = m_sExternalFunTable.pf_QEI_GetHSpdQeiSpeed();
            CMU_AddToSendCtrlData(NULL,sizeof(Bit32));
            ulRet = CMU_ERR_SUCCESS;
            break;
        }
    case SYS_GETCMD_CNC_ALARM:                      //获取CNC系统报警 2017.12.18 Duanfeng
        {
            
            *(Bit32*)pSendBuf = m_sExternalFunTable.pf_SYS_GetCncAlarmStatus();
            CMU_AddToSendCtrlData(NULL,sizeof(Bit32));
            ulRet = CMU_ERR_SUCCESS;
            break;
        }
        
    default: break;
    }

    return ulRet;
}


 /*
函数名称:uBit32 Sys_CmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData)

功    能:系统常规获取指令处理线程,如果设置指令执行完成并成功则回包,指令类型包括
            #define SETCMD_TYPE_NOMAL    (0)                        //常规设置指令
            #define SETCMD_TYPE_INQ        (1)                        //周期查询设置指令
            #define GETCMD_TYPE_NOMAL    (0)                        //常规获取指令
            #define GETCMD_TYPE_INQ        (1)                        //周期新数据获取指令

参    数:pRcvCtrlData--接收控制数据区

返 回 值:0--成功
          非0--错误码
注意事项:数据块接收完成后调用
*/ 
uBit32 Sys_CmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData)
{
    uBit32 ulRet = CMU_ERR_INVALID_CMD;

    if (pRcvCtrlData->ulRevID.ulComDataID.ulOperType==DATA_OPRE_TYPE_SET)//设置数据
    {
        switch(pRcvCtrlData->ulRevID.ulComDataID.ulCmdType)
        {
        case SETCMD_TYPE_NOMAL:    //常规设置指令
            ulRet = Sys_SetNomalCmdProcess(pRcvCtrlData);
            break;
        case SETCMD_TYPE_INQ:    //周期性设置指令
            ulRet = CMU_AddInqRecord(pRcvCtrlData->ulRevID, *(uBit32*)pRcvCtrlData->pRevBuf);
            break;
        default:break;
        }

        if (ulRet==CMU_ERR_SUCCESS)//
        {
            if(pRcvCtrlData->ulWaitResultCode==0) //无需等待处理结果
                  CMU_SendResponsePack(pRcvCtrlData->ulRevID.ulFrameID, CMU_ERR_SUCCESS);
        }
    }else//获取数据
    {
        switch(pRcvCtrlData->ulRevID.ulComDataID.ulCmdType)
        {
        case GETCMD_TYPE_NOMAL:    //获取常规数据
            ulRet = Sys_GetNomalCmdProcess(pRcvCtrlData);
            break;
        case GETCMD_TYPE_INQ:            //获取周期性状态数据
            ulRet = CMU_SendPeriodDataProc(pRcvCtrlData->ulRevID);
            break;
        default:break;
        }
    }

    return ulRet;
}
//---------------------------------------------------------------------------

//坐标系指令处理接口----------------------------------------------------------
uBit32 RapidCmdTranslate(COM_RCV_CTRL_DATA *pRcvCtrlData)
{
    //uBit32 ulMask;                            //运动段屏蔽字bit0-绝对编程标志,bit1-顺时针标志
    BooLean bAbsFlag;

    //memcpy(&ulMask, pRcvCtrlData->pRevBuf, sizeof(uBit32));    //保存运动段屏蔽位
    bAbsFlag = ((*(uBit32*)(pRcvCtrlData->pRevBuf))&PROG_SEG_DATA_MASK_ABS_BIT)==0?false:true;


    return m_sExternalFunTable.pf_CSM_ProgRapid(pRcvCtrlData->ulRevID.ulComDataID.ulCrdNo,
        pRcvCtrlData->ulRevID.ulComDataID.ulDevNo,            //轴屏蔽字
        (Bit32*)(pRcvCtrlData->pRevBuf+sizeof(uBit32)),        //轴位置
        bAbsFlag);
}

uBit32 LineCmdTranslate(COM_RCV_CTRL_DATA *pRcvCtrlData)
{
    //uBit32 ulMask;                            //运动段屏蔽字bit0-绝对编程标志,bit1-顺时针标志
    BooLean bAbsFlag;

    //memcpy(&ulMask, pRcvCtrlData->pRevBuf, sizeof(uBit32));    //保存运动段屏蔽位
    bAbsFlag = ((*(uBit32*)(pRcvCtrlData->pRevBuf))&PROG_SEG_DATA_MASK_ABS_BIT)==0?false:true;

    return m_sExternalFunTable.pf_CSM_ProgLine(pRcvCtrlData->ulRevID.ulComDataID.ulCrdNo,
         pRcvCtrlData->ulRevID.ulComDataID.ulDevNo,        //轴屏蔽字
        (Bit32*)(pRcvCtrlData->pRevBuf+sizeof(uBit32)),        //轴位置
        *(float32*)(pRcvCtrlData->pRevBuf + pRcvCtrlData->ulRevLen-sizeof(float32)),    //插补速度
        bAbsFlag);
}


uBit32 ArcCCmdTranslate(COM_RCV_CTRL_DATA *pRcvCtrlData)
{
    BooLean bAbsFlag;
    BooLean bClockWise;

    bAbsFlag = ((*(uBit32*)(pRcvCtrlData->pRevBuf))&PROG_SEG_DATA_MASK_ABS_BIT)==0?false:true;
    bClockWise = ((*(uBit32*)(pRcvCtrlData->pRevBuf))&PROG_SEG_DATA_MASK_CW_BIT)==0?false:true;

    return m_sExternalFunTable.pf_CSM_ProgArcC(pRcvCtrlData->ulRevID.ulComDataID.ulCrdNo,
        (uBit32*)(pRcvCtrlData->pRevBuf+sizeof(uBit32)),            //圆弧插补轴序号列表(2轴)
        (Bit32*)(pRcvCtrlData->pRevBuf+3*sizeof(uBit32)),        //圆弧插补终点位置
        (Bit32*)(pRcvCtrlData->pRevBuf+5*sizeof(uBit32)),        //圆弧插补圆心位置(相对于起点增量)
        bClockWise,
        *(float32*)(pRcvCtrlData->pRevBuf + pRcvCtrlData->ulRevLen-sizeof(float32)),    //插补速度
        bAbsFlag);
}


uBit32 ArcRCmdTranslate(COM_RCV_CTRL_DATA *pRcvCtrlData)
{
    BooLean bAbsFlag;
    BooLean bClockWise;

    bAbsFlag = ((*(uBit32*)(pRcvCtrlData->pRevBuf))&PROG_SEG_DATA_MASK_ABS_BIT)==0?false:true;
    bClockWise = ((*(uBit32*)(pRcvCtrlData->pRevBuf))&PROG_SEG_DATA_MASK_CW_BIT)==0?false:true;

    return m_sExternalFunTable.pf_CSM_ProgArcR(pRcvCtrlData->ulRevID.ulComDataID.ulCrdNo,
        (uBit32*)(pRcvCtrlData->pRevBuf+sizeof(uBit32)),        //圆弧插补轴序号列表(2轴)
        (Bit32*)(pRcvCtrlData->pRevBuf+3*sizeof(uBit32)),        //圆弧插补终点位置
        *(Bit32*)(pRcvCtrlData->pRevBuf+5*sizeof(uBit32)),        //圆弧半径
        bClockWise,
        *(float32*)(pRcvCtrlData->pRevBuf + pRcvCtrlData->ulRevLen-sizeof(float32)),    //插补速度
        bAbsFlag);
}


uBit32 ArcPTCmdTranslate(COM_RCV_CTRL_DATA *pRcvCtrlData)
{
    BooLean bAbsFlag;
    bAbsFlag = ((*(uBit32*)(pRcvCtrlData->pRevBuf))&PROG_SEG_DATA_MASK_ABS_BIT)==0?false:true;

    return m_sExternalFunTable.pf_CSM_ProgArcPT(pRcvCtrlData->ulRevID.ulComDataID.ulCrdNo,
        (uBit32*)(pRcvCtrlData->pRevBuf+sizeof(uBit32)),        //圆弧插补轴序号列表(2轴)
        (Bit32*)(pRcvCtrlData->pRevBuf+3*sizeof(uBit32)),        //圆弧插补终点位置
        (Bit32*)(pRcvCtrlData->pRevBuf+5*sizeof(uBit32)),        //圆弧插补中间点位置
        *(float32*)(pRcvCtrlData->pRevBuf + pRcvCtrlData->ulRevLen-sizeof(float32)),    //插补速度
        bAbsFlag);
}

uBit32 Crd_SetCrdCtrlParmEx(uBit32 ulCrdNo, COM_RCV_CTRL_DATA *pRcvCtrlData)
{
    uBit32 ulParmType;
    uBit32 ulRet=CMU_ERR_SUCCESS;
    const CRDSYS_PARM *pCrdCtrlParmAddr;
    CRDSYS_PARM sCrdCtrlParm;

    pCrdCtrlParmAddr = m_sExternalFunTable.pf_SPM_GetCrdSysParmAddr();
    sCrdCtrlParm = pCrdCtrlParmAddr[ulCrdNo];
    ulParmType =  *(uBit32*)pRcvCtrlData->pRevBuf;

    //控制参数检查
    switch(ulParmType)
    {
    case CRD_PARM_LOOKAHEADLEN                ://(uBit16)前瞻段数,取值范围:大于等于0
        {
            memcpy(&sCrdCtrlParm.nLookaheadLen, &pRcvCtrlData->pRevBuf[4], sizeof(uBit16));
            break;
        }
    case CRD_PARM_MINCONSTANTVELTIME    ://(uBit32)最小匀速段时间,单位:豪秒,取值范围:大于等于0
        {
             memcpy(&sCrdCtrlParm.ulMinConstantVelTime, &pRcvCtrlData->pRevBuf[4], sizeof(uBit32));
            break;           
        }
    case CRD_PARM_LINEAXISCORNERPRECI    ://(float32)直线轴拐角误差,单位:计数,取值范围:大于等于0
        {
               memcpy(&sCrdCtrlParm.dLineAxisCornerPreci, &pRcvCtrlData->pRevBuf[4], sizeof(float32));
            break;
        }
    case CRD_PARM_ROTAXISCORNERPRECI    ://(float32)旋转轴拐角误差,单位:0.001度,取值范围:大于等于0
        {
               memcpy(&sCrdCtrlParm.dRotAxisCornerPreci, &pRcvCtrlData->pRevBuf[4], sizeof(float32));
            break;
        }
    case CRD_PARM_CIRCLEERRORLIM            ://(float32)圆弧误差限制,单位:0.001计数,取值范围:大于等于
        {
            memcpy(&sCrdCtrlParm.dCircleErrorLim, &pRcvCtrlData->pRevBuf[4], sizeof(float32));
            break;
        }
    case CRD_PARM_PROGMAXFEEDSPD            ://(float32)程序最大进给速度,单位:0.001计数/毫秒,取值范围:大于0
        {
            memcpy(&sCrdCtrlParm.dProgMaxFeedSpeed, &pRcvCtrlData->pRevBuf[4], sizeof(float32));
            break;
        }
    case CRD_PARM_PROGMAXACC                ://(float32)程序最大加速度,单位:0.001计算/平方毫秒,取值范围:大于0
        {
            memcpy(&sCrdCtrlParm.dProgMaxAcc, &pRcvCtrlData->pRevBuf[4], sizeof(float32));
            break;
        }
    default:
        return CMU_ERR_DATA_WRONG;
        break;
    }

    ulRet = m_sExternalFunTable.pf_SPM_SetCrdParm(ulCrdNo, &sCrdCtrlParm);
    return ulRet;
}

 /*
函数名称:uBit32 Crd_NomalSetCmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData)

功    能:坐标系常规设置指令处理线程,指令包括
                #define CRD_SETCMD_AXISMAP                      (1)//设置轴映射表 
                #define CRD_SETCMD_CTRL_PARM                    (2)//设置坐标系控制参数
                #define CRD_SETCMD_RAPID_OVERRIDE               (3)//设置坐标系快移修调
                #define CRD_SETCMD_FEED_OVERRIDE                (4)//设置坐标系进给修调
                #define CRD_SETCMD_SPINDLE_OVERRIDE             (5)//设置坐标系主轴修调
                #define CRD_SETCMD_VERIFY_ENABLE                (6)//校验运行使能控制
                #define CRD_SETCMD_CTRL_MODE                    (7)//设置坐标系工作模式
                #define CRD_SETCMD_ESTOP                        (8)//设置指定的坐标系急停
                #define CRD_SETCMD_RESET                        (9)//坐标系复位
                #define CRD_SETCMD_SPINDLE_RUN                  (10)//启动主轴
                #define CRD_SETCMD_SPINDLE_STOP                 (11)//停止主轴
                #define CRD_SETCMD_PROG_MODE                    (12)//设置程序缓冲区工作模式
                #define CRD_SETCMD_CLEAR_PROG_BUF               (13)//清除坐标系程序缓冲区
                #define CRD_SETCMD_SINGLE_SEG_MODE              (14)//设置系统单段方式
                #define CRD_SETCMD_LOOKAHEAD                    (15)//前瞻控制控制
                #define CRD_SETCMD_PROG_RUN                     (16)//运行坐标系程序
                #define CRD_SETCMD_PROG_HOLD                    (17)//暂停坐标系程序
                #define CRD_SETCMD_PROG_STOP                    (18)//暂停坐标系程序
                #define CRD_SETCMD_PROG_CANCEL                  (19)
                #define CRD_SETCMD_SEGEDN_STOP                  (20)//段结束停止坐标系程序
                #define CRD_SETCMD_USERDATABUF_MODE             (21)//设置用户缓冲区工作模式
                #define CRD_SETCMD_CLEAR_USERDATABUF            (22)//清除用户自定义数据区
                #define CRD_SETCMD_LOAD_USERDATA                (23)//下载用户数据
                #define CRD_SETCMD_LOAD_GCODE                   (24)//下载G代码数据
                #define CRD_SETCMD_RELOAD_GCODE                 (25)//重新装载G代码
                #define CRD_SETCMD_TOOL_DATA                    (26)//设置刀库数据
                #define CRD_SETCMD_IPR_SELECT                   (27)//必须下载完G代码后才能调用
                #define CRD_SETCMD_IPR_SET_OFFSET               (28)//设置程序偏移
                #define CRD_SETCMD_IPR_RDN_START                (29)//任意行启动
                #define CRD_SETCMD_IPR_SET_PROG_POS             (30) //设置当前编程位置（工件坐标系值）
                #define CRD_SETCMD_IPR_SET_COORD                (31) //重新设置坐标零点
                #define CRD_SETCMD_IPR_SET_VAR                  (32) //设置变量值
                #define CRD_SETCMD_CTRL_PARM_EX                 (33)//设置坐标系控制参数单项数据
                #define CRD_PROG_CMD_BASE                       (50)                 //运动指令起始
                #define CRD_PROG_CMD_COUNT                      (11)                 //运动指令总数
                #define CRD_SETCMD_PROG_DWELL                   (CRD_PROG_CMD_BASE)  //设置坐标系程序延时
                #define CRD_SETCMD_M_CODE                       (CRD_PROG_CMD_BASE+1)//设置坐标系M代码
                #define CRD_SETCMD_S_CODE                       (CRD_PROG_CMD_BASE+2)//设置坐标系S代码
                #define CRD_SETCMD_T_CODE                       (CRD_PROG_CMD_BASE+3)//设置坐标系T代码
                #define CRD_SETCMD_B_CODE                       (CRD_PROG_CMD_BASE+4)//设置坐标系B代码
                #define CRD_SETCMD_IO_CODE                      (CRD_PROG_CMD_BASE+5)//
                #define CRD_SETCMD_RAPID                        (CRD_PROG_CMD_BASE+6)//单个轴或者多个轴同时执行快速移动
                #define CRD_SETCMD_LINE                         (CRD_PROG_CMD_BASE+7)//多轴直线插补运动
                #define CRD_SETCMD_ARC_C                        (CRD_PROG_CMD_BASE+8)//圆弧插补圆心,终点模式
                #define CRD_SETCMD_ARC_R                        (CRD_PROG_CMD_BASE+9)//圆弧插补半径,终点模式
                #define CRD_SETCMD_ARC_PT                       (CRD_PROG_CMD_BASE+10)//圆弧插补中间点、终点模式
参    数:pRcvCtrlData--接收控制数据区

返 回 值:0--成功
          非0--错误码
注意事项:坐标系指令处理线程调用
*/ 

uBit32 Crd_NomalSetCmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData)
{
    Bit32 iDir;
    uBit8 uCrdNo;
    uBit32 ulSpeed;
    Bit32 iCrdIndex;
    uBit16 nCrdMask;
    uBit16 nBlockNo;
    //uBit32 ulAxisMask;
    const SYS_CTRL_PARM *pSysCtrlParm;
    const CRDSYS_STATE_DATA *pCrdStateData; 
    uBit32 ulRet=CMU_ERR_INVALID_CMD;

    if (pRcvCtrlData->ulRevID.ulComDataID.ulCmdIndex>=CRD_PROG_CMD_BASE && 
        pRcvCtrlData->ulRevID.ulComDataID.ulCmdIndex<CRD_PROG_CMD_BASE+CRD_PROG_CMD_COUNT)
    {
        if (m_sExternalFunTable.pf_IPR_GetPpiEnable())  //解释器使能
        {
            ulRet = m_sExternalFunTable.pf_IPR_SetPpiEnable(0);

            if (ulRet)
                return ulRet;
        }

        switch(pRcvCtrlData->ulRevID.ulComDataID.ulCmdIndex)
        {
        case CRD_SETCMD_PROG_DWELL:                     //设置坐标系程序延时
            {
                ulRet = m_sExternalFunTable.pf_CSM_ProgDwell(pRcvCtrlData->ulRevID.ulComDataID.ulCrdNo,
                    *(uBit32*)pRcvCtrlData->pRevBuf);
            }
            break;
        case CRD_SETCMD_M_CODE:                         //设置坐标系M代码
            {
                ulRet = m_sExternalFunTable.pf_CSM_ProgMCode(pRcvCtrlData->ulRevID.ulComDataID.ulCrdNo,
                    *(uBit16*)pRcvCtrlData->pRevBuf);
            }
            break;
        case CRD_SETCMD_S_CODE:                         //设置坐标系S代码
            {
                ulRet = m_sExternalFunTable.pf_CSM_ProgSCode(pRcvCtrlData->ulRevID.ulComDataID.ulCrdNo,
                    *(uBit32*)pRcvCtrlData->pRevBuf);
            }
            break;
        case CRD_SETCMD_T_CODE:                         //设置坐标系T代码
            {
                ulRet = m_sExternalFunTable.pf_CSM_ProgSCode(pRcvCtrlData->ulRevID.ulComDataID.ulCrdNo,
                    *pRcvCtrlData->pRevBuf);
            }
            break;
        case CRD_SETCMD_B_CODE:                         //设置坐标系B代码
            {
                ulRet = m_sExternalFunTable.pf_CSM_ProgBCode(pRcvCtrlData->ulRevID.ulComDataID.ulCrdNo,
                    *pRcvCtrlData->pRevBuf);
            }
            break;
        case CRD_SETCMD_IO_CODE:
            {
                ulRet = m_sExternalFunTable.pf_CSM_ProgIOCode(pRcvCtrlData->ulRevID.ulComDataID.ulCrdNo,
                    pRcvCtrlData->ulRevID.ulComDataID.ulDevNo,
                    *(uBit32*)pRcvCtrlData->pRevBuf,                //IO状态掩码    
                    *(uBit32*)pRcvCtrlData->pRevBuf+sizeof(uBit32), //IO状态位状态值
                    *(uBit32*)pRcvCtrlData->pRevBuf+2*sizeof(uBit32)//表示延时时间,即执行IO状态设置后,延迟指定时间,再反转IO状态
                    );
            }
            break;
        case CRD_SETCMD_RAPID:                          //单个轴或者多个轴同时执行快速移动
            {
                ulRet = RapidCmdTranslate(pRcvCtrlData);
            }
            break;
        case CRD_SETCMD_LINE:                           //多轴直线插补运动
            {
                ulRet = LineCmdTranslate(pRcvCtrlData);
            }
            break;
        case CRD_SETCMD_ARC_C:                          //圆弧插补中心点,结点模式
            {
                ulRet = ArcCCmdTranslate(pRcvCtrlData);
            }
            break;
        case CRD_SETCMD_ARC_R:                          //圆弧插补半径,终点模式
            {
                ulRet = ArcRCmdTranslate(pRcvCtrlData);
            }
            break;
        case CRD_SETCMD_ARC_PT:                         //圆弧插补中间点、终点模式
            {
                ulRet = ArcPTCmdTranslate(pRcvCtrlData);
            }
            break;
        default:
            return CMU_ERR_INVALID_CMD;
            break;
        }
    }else
    {
         iCrdIndex = pRcvCtrlData->ulRevID.ulComDataID.ulCrdNo;
         nCrdMask = pRcvCtrlData->ulRevID.ulComDataID.ulCrdNo;   //

        switch(pRcvCtrlData->ulRevID.ulComDataID.ulCmdIndex)
        {
        case  CRD_SETCMD_AXISMAP:                       //设置轴映射表
            {
                ulRet = m_sExternalFunTable.pf_SPM_SetCrdSysAxisMapTable((CRDSYS_AXIS_MAP_TABLE*)pRcvCtrlData->pRevBuf,nCrdMask);
#ifndef DISABLE_IPR_INTERFACE
                if (ulRet==0)
                    m_sExternalFunTable.pf_IPR_UpdateCrdData();
#endif

                break;
            }
        case CRD_SETCMD_CTRL_PARM:                      //设置坐标系控制参数
            {
                ulRet = m_sExternalFunTable.pf_SPM_SetCrdParm(iCrdIndex, (CRDSYS_PARM*)pRcvCtrlData->pRevBuf);
                break;
            }
        case CRD_SETCMD_CTRL_PARM_EX:                   //设置坐标系控制参数单项数据
            {
                ulRet =Crd_SetCrdCtrlParmEx(iCrdIndex, pRcvCtrlData);
                break;
            }
        case CRD_SETCMD_RAPID_OVERRIDE:                 //设置坐标系快移修调
            {
                ulRet = m_sExternalFunTable.pf_CSN_SetRapidOverride(iCrdIndex, *((float32*)pRcvCtrlData->pRevBuf));
                break;
            }
        case CRD_SETCMD_FEED_OVERRIDE:                  //设置坐标系进给修调
            {
                ulRet = m_sExternalFunTable.pf_CSM_SetFeedOverride(iCrdIndex, *((float32*)pRcvCtrlData->pRevBuf));
                break;
            }
        case CRD_SETCMD_SPINDLE_OVERRIDE:               //设置坐标系主轴修调
            {
                ulRet = m_sExternalFunTable.pf_CSM_SetSpindleOverride(iCrdIndex, *((float32*)pRcvCtrlData->pRevBuf));
                break;
            }
        case CRD_SETCMD_VERIFY_ENABLE:                  //校验运行使能控制
            {
                ulRet = m_sExternalFunTable.pf_CSM_EnableVerify(iCrdIndex, *((Bit32*)pRcvCtrlData->pRevBuf));
                break;
            }
        case CRD_SETCMD_CTRL_MODE:                      //设置坐标系工作模式
            {
                ulRet = m_sExternalFunTable.pf_CSM_SetCtrlMode(iCrdIndex, *((uBit32*)pRcvCtrlData->pRevBuf));
                break;
            }
        case CRD_SETCMD_ESTOP:                          //设置指定的坐标系急停
            {
                ulRet = m_sExternalFunTable.pf_CSM_ProgEStop(nCrdMask);
                break;
            }
        case CRD_SETCMD_RESET:                          //坐标系复位
            {
                ulRet = m_sExternalFunTable.pf_CSM_Reset(iCrdIndex);
                break;
            }
        case CRD_SETCMD_SPINDLE_RUN:                    //启动主轴
            {
                memcpy(&iDir, pRcvCtrlData->pRevBuf, sizeof(Bit32));
                memcpy(&ulSpeed, pRcvCtrlData->pRevBuf+sizeof(uBit32), sizeof(uBit32));
                ulRet = m_sExternalFunTable.pf_CSM_SpindleRun(iCrdIndex, iDir, ulSpeed);
                break;
            }
        case CRD_SETCMD_SPINDLE_STOP:                   //停止主轴
            {
                ulRet = m_sExternalFunTable.pf_CSM_SpindleStop(iCrdIndex);
                break;
            }
        case CRD_SETCMD_PROG_MODE:                      //设置程序缓冲区工作模式
            {
                ulRet = m_sExternalFunTable.pf_CSM_SetProgBufMode(iCrdIndex, *((Bit32*)pRcvCtrlData->pRevBuf));
                break;
            }
        case CRD_SETCMD_CLEAR_PROG_BUF:                 //清除坐标系程序缓冲区
            {
                ulRet = m_sExternalFunTable.pf_CSM_ClearProgData(iCrdIndex, 2);//清除所有运动段数据
                break;
            }
        case CRD_SETCMD_SINGLE_SEG_MODE:                //设置系统单段方式
            {
                ulRet = m_sExternalFunTable.pf_CSM_EnableSingleSegmentMode(iCrdIndex, *((Bit32*)pRcvCtrlData->pRevBuf));
                break;
            }
        case CRD_SETCMD_LOOKAHEAD:                      //前瞻控制控制
            {
                ulRet = m_sExternalFunTable.pf_SPM_SetLookaheadEnable(iCrdIndex, *((Bit32*)pRcvCtrlData->pRevBuf));
                break;
            }
        case CRD_SETCMD_PROG_RUN:                       //运行坐标系程序
            {
#ifndef DISABLE_IPR_INTERFACE
                if (m_sExternalFunTable.pf_IPR_GetPpiEnable())//解释器使能
                {
                    //获取坐标系数量
                    ulRet=CMU_ERR_SUCCESS;
                    pSysCtrlParm = m_sExternalFunTable.pf_SPM_GetSysCtrlParmAddr();

                    for (uCrdNo=0; uCrdNo<pSysCtrlParm->nCrdSysCount; uCrdNo++)
                    {
                        if ((nCrdMask & (1<<uCrdNo)) == 0)
                            continue;

                        //程序下载且处于暂停或正在解释状态时
                        if (m_sExternalFunTable.pf_IPR_GetState(uCrdNo)>=0)
                        {
                            ulRet = m_sExternalFunTable.pf_IPR_Cycle(uCrdNo);
                            
                            if (ulRet!=CMU_ERR_SUCCESS)
                                break;
                        }
                    }
                }else
                {
                    ulRet = m_sExternalFunTable.pf_CSM_ProgRun(nCrdMask);
                }
#else
                ulRet = m_sExternalFunTable.pf_CSM_ProgRun(nCrdMask);
#endif
                break;
            }
        case CRD_SETCMD_PROG_HOLD:                    //暂停坐标系程序
            {
#ifndef DISABLE_IPR_INTERFACE
                if (m_sExternalFunTable.pf_IPR_GetPpiEnable())//解释器使能
                {
                    //获取坐标系数量
                    ulRet=CMU_ERR_SUCCESS;
                    pSysCtrlParm = m_sExternalFunTable.pf_SPM_GetSysCtrlParmAddr();

                    for (uCrdNo=0; uCrdNo<pSysCtrlParm->nCrdSysCount; uCrdNo++)
                    {
                        if ((nCrdMask & (1<<uCrdNo)) == 0)
                            continue;

                        //程序下载且处与暂停状态
                        if (m_sExternalFunTable.pf_IPR_GetState(uCrdNo)>0)
                        {
                            ulRet = m_sExternalFunTable.pf_IPR_Pause(uCrdNo);
                            
                            if (ulRet!=CMU_ERR_SUCCESS)
                                break;                            
                        }
                    }
                }else
                {
                    ulRet = m_sExternalFunTable.pf_CSM_ProgHold(nCrdMask);
                }
#else
                ulRet = m_sExternalFunTable.pf_CSM_ProgHold(nCrdMask);
#endif
                break;
            }
        case CRD_SETCMD_PROG_STOP:                    //停止坐标系程序
            {
#ifndef DISABLE_IPR_INTERFACE
                if (m_sExternalFunTable.pf_IPR_GetPpiEnable())//解释器使能
                {
                    ulRet=CMU_ERR_SUCCESS;

                    //获取坐标系数量
                    pSysCtrlParm = m_sExternalFunTable.pf_SPM_GetSysCtrlParmAddr();

                    for (uCrdNo=0; uCrdNo<pSysCtrlParm->nCrdSysCount; uCrdNo++)
                    {
                        if ((nCrdMask & (1<<uCrdNo)) == 0)
                            continue;

                        ulRet = m_sExternalFunTable.pf_IPR_ProgReset(uCrdNo);

                        if (ulRet!=CMU_ERR_SUCCESS)
                            break;
                    }
                }else
                {
                    ulRet = m_sExternalFunTable.pf_CSM_ProgStop(nCrdMask);
                }
#else
                ulRet = m_sExternalFunTable.pf_CSM_ProgStop(1<<uCrdNo);
#endif
                break;
            }
            case CRD_SETCMD_PROG_CANCEL:            //取消坐标系程序
            {
#ifndef DISABLE_IPR_INTERFACE
                if (m_sExternalFunTable.pf_IPR_GetPpiEnable())//解释器使能
                {
                    ulRet=CMU_ERR_SUCCESS;

                    //获取坐标系数量
                    pSysCtrlParm = m_sExternalFunTable.pf_SPM_GetSysCtrlParmAddr();

                    for (uCrdNo=0; uCrdNo<pSysCtrlParm->nCrdSysCount; uCrdNo++)
                    {
                        if ((nCrdMask & (1<<uCrdNo)) == 0)
                            continue;

                        //程序正在运行或暂停
                        if (m_sExternalFunTable.pf_IPR_GetState(uCrdNo)>=0)
                        {
                            ulRet = m_sExternalFunTable.pf_IPR_Cancel(uCrdNo);
                            
                            if (ulRet!=CMU_ERR_SUCCESS)
                                break;         
                        }

                        //等待解释器停止
                        while (m_sExternalFunTable.pf_IPR_GetState(uCrdNo)>0);
                    }
                }else
                {
                    ulRet = m_sExternalFunTable.pf_CSM_ProgCancel(nCrdMask);
                }
#else
                ulRet = m_sExternalFunTable.pf_CSM_ProgCancel(nCrdMask);
#endif
                break;
            }                
        case CRD_SETCMD_SEGEDN_STOP:                //段结束停止坐标系程序
            {
                ulRet = m_sExternalFunTable.pf_CSM_ProgSegmentEndStop(nCrdMask);
                break;
            }
        case CRD_SETCMD_USERDATABUF_MODE:            //设置用户缓冲区工作模式
            {
                ulRet = m_sExternalFunTable.pf_CSM_SetUserBufMode(iCrdIndex, *((Bit32*)pRcvCtrlData->pRevBuf));
                break;
            }
        case CRD_SETCMD_CLEAR_USERDATABUF:            //清除用户自定义数据区
            {
                ulRet = m_sExternalFunTable.pf_CSM_ClearUserData(nCrdMask);
                break;
            }
        case CRD_SETCMD_LOAD_USERDATA:                //下载用户数据
            {
                ulRet = m_sExternalFunTable.pf_CSM_AddUserDataToBuf(iCrdIndex, (Bit8*)pRcvCtrlData->pRevBuf, pRcvCtrlData->ulRevLen);
                break;
            }
        case CRD_SETCMD_LOAD_GCODE:                    //下载G代码数据
            {
#ifndef DISABLE_IPR_INTERFACE
                //获取块序号
                memcpy(&nBlockNo, pRcvCtrlData->pRevBuf, sizeof(nBlockNo));

                if (nBlockNo==0)//开始下载G代码程序,先检查IPR是否启动
                {
                    if (m_sExternalFunTable.pf_IPR_GetPpiEnable()==0)
                    {
                        ulRet = m_sExternalFunTable.pf_IPR_SetPpiEnable(1);
                        
                        if (ulRet)
                            break;                        
                    }

                    //检查程序是否正在运行
                    pCrdStateData = m_sExternalFunTable.pf_CSM_GetCrdSysStateReadAddr(pRcvCtrlData->ulRevID.ulComDataID.ulCrdNo);
                    
                    if((pCrdStateData->ulStatus & CRDSYS_STATUS_FREE)==0 ||
                        (pCrdStateData->ulStatus & CRDSYS_STATUS_PROG_STOP)==0)
                    {
                        ulRet = CMU_ERR_SYS_BUSY;
                        break;
                    }
                    //设置程序缓冲区工作模式
                    //m_sExternalFunTalbe.pf_CSM_SetProgBufMode();
                }

                ulRet = m_sExternalFunTable.pf_IPR_PutGCodeToIprBuf(pRcvCtrlData->ulRevID.ulComDataID.ulCrdNo,
                    nBlockNo, 
                    pRcvCtrlData->pRevBuf+2, 
                    pRcvCtrlData->ulRevLen-2,
                    pRcvCtrlData->ulRevID.ulComDataID.ulDevNo);
#endif
                break;
            }
        case CRD_SETCMD_LOAD_CANNDED:                    //下载固定循环数据
            {
    #ifndef DISABLE_IPR_INTERFACE
                ulRet = 0;
#if 1
                if (m_sExternalFunTable.pf_IPR_GetPpiEnable())//解释器使能
                {
                    ulRet = CMU_ERR_IPR_ENABLE;
                    break;
                }

                //获取块序号
                memcpy(&nBlockNo, pRcvCtrlData->pRevBuf, sizeof(nBlockNo));

                ulRet = m_sExternalFunTable.pf_SYS_UpdateCCFileData(nBlockNo, 
                    pRcvCtrlData->pRevBuf+2, 
                    pRcvCtrlData->ulRevLen-2,
                    pRcvCtrlData->ulRevID.ulComDataID.ulDevNo);
#endif         
    #endif
                break;
            }
        case CRD_SETCMD_LOAD_BP_DATA:                    //下载断点数据
            {
#ifndef DISABLE_IPR_INTERFACE
                //获取起始地址写位置
                memcpy(&nBlockNo, pRcvCtrlData->pRevBuf, sizeof(nBlockNo));
                ulRet = m_sExternalFunTable.pf_IPR_SetBpData(pRcvCtrlData->ulRevID.ulComDataID.ulCrdNo,
                nBlockNo, 
                pRcvCtrlData->pRevBuf+2, 
                pRcvCtrlData->ulRevLen-2,
                pRcvCtrlData->ulRevID.ulComDataID.ulDevNo);
#endif
                break;
            }

        case CRD_SETCMD_TOOL_DATA:                //设置刀库数据
            {
#ifndef DISABLE_IPR_INTERFACE
                ulRet = m_sExternalFunTable.pf_IPR_SetToolData(pRcvCtrlData->ulRevID.ulComDataID.ulCrdNo,
                    pRcvCtrlData->ulRevID.ulComDataID.ulDevNo, (CUTTER_T*)pRcvCtrlData->pRevBuf);
#endif
                break;
            }
        case CRD_SETCMD_CUR_TOOL_NO:                //设置刀库数据
            {
#ifndef DISABLE_IPR_INTERFACE
                m_sExternalFunTable.pf_IPR_SetTool(pRcvCtrlData->ulRevID.ulComDataID.ulCrdNo,
                    pRcvCtrlData->ulRevID.ulComDataID.ulDevNo);

                ulRet = CMU_ERR_SUCCESS;
#endif
                break;
            }
        case CRD_SETCMD_BASE_TOOL_NO:                //设置基准刀具
            {
#ifndef DISABLE_IPR_INTERFACE
                m_sExternalFunTable.pf_IPR_SetBaseTool(pRcvCtrlData->ulRevID.ulComDataID.ulCrdNo,
                    pRcvCtrlData->ulRevID.ulComDataID.ulDevNo);

                ulRet = CMU_ERR_SUCCESS;
#endif
                break;
            }

        case CRD_SETCMD_IPR_RDN_START:                    //任意行启动
            {
#ifndef DISABLE_IPR_INTERFACE

                //获取行号
                m_sExternalFunTable.pf_IPR_RndStart(pRcvCtrlData->ulRevID.ulComDataID.ulCrdNo,
                    *(Bit32*)pRcvCtrlData->pRevBuf);

                ulRet = CMU_ERR_SUCCESS;
#endif
                break;
            }
        case CRD_SETCMD_IPR_SET_COORD:             //重新设置坐标零点
            {
#ifndef DISABLE_IPR_INTERFACE
                
                m_sExternalFunTable.pf_IPR_SetCrdZero(pRcvCtrlData->ulRevID.ulComDataID.ulCrdNo,
                    pRcvCtrlData->ulRevID.ulComDataID.ulDevNo,(Bit32*)pRcvCtrlData->pRevBuf);

                ulRet = CMU_ERR_SUCCESS;
#endif
                break;
            }
        default:ulRet = CMU_ERR_INVALID_CMD;
        }
    }

    return ulRet;
}


 /*
函数名称:uBit32 Crd_NomalGetCmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData)

功    能:坐标系常规获取指令处理线程,如果获取成功则配置回包,指令包括
        #define CRD_GETCMD_AXIS_MAP                (1)//获取坐标系映射表
        #define CRD_GETCMD_CTRL_PARM            (2)//获取坐标系控制数据
        #define CRD_GETCMD_TOOL_DATA            (3)//获取刀具数据
        #define CRD_GETCMD_UPLOAD_PROG_DATA        (4)//获取程序数据
        #define CRD_GETCMD_UPLOAD_USER_DATA        (5)//获取用户数据
        #define CRD_GETCMD_UPLOAD_GCODE_DATA    (6)//获取G代码数据
        #define CRD_GETCMD_BP_DATA_LEN            (7)//获取断点数据长度
        #define CRD_GETCMD_BP_DATA                (8)//获取断点数据
        #define CRD_GETCMD_IPR_CUR_LINENO        (9)//获取当前解释行号
        #define CRD_GETCMD_CRD_ZERO                (10)//获取坐标系零点
        #define CRD_GETCMD_BASE_TOOL            (11)//获取基本刀具号
参    数:pRcvCtrlData--接收控制数据区

返 回 值:0--成功
          非0--错误码
注意事项:坐标系指令处理线程调用
*/ 
uBit32 Crd_NomalGetCmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData)
{
    uBit32 ulRet = CMU_ERR_INVALID_CMD;
    uBit32 ulLen;
    Bit32 iCrdIndex;
    uBit8 *pSendBuf;
    uBit32 ulStartPos;
    const CRDSYS_PARM *pCrdCtrlParm;
    const CRDSYS_AXIS_MAP_TABLE *pCrdAxisMapAddr;

    CMU_ResetSendCtrlData(pRcvCtrlData->ulRevID.ulFrameID,NULL,0);
    pSendBuf = CMU_GetSendBufAddr();
    iCrdIndex = pRcvCtrlData->ulRevID.ulComDataID.ulCrdNo;

    switch(pRcvCtrlData->ulRevID.ulComDataID.ulCmdIndex)
    {
    case CRD_GETCMD_AXIS_MAP:                        //获取坐标系映射表
        {
            pCrdAxisMapAddr = m_sExternalFunTable.pf_SPM_GetCrdSysAxisMapTableAddr();
            pCrdAxisMapAddr +=  iCrdIndex;
            CMU_ResetSendCtrlData(pRcvCtrlData->ulRevID.ulFrameID, (uBit8*)pCrdAxisMapAddr, sizeof(CRDSYS_AXIS_MAP_TABLE));
            ulRet = CMU_ERR_SUCCESS;
            break;
        }
    case CRD_GETCMD_CTRL_PARM:                        //获取坐标系控制数据
        {
            pCrdCtrlParm = m_sExternalFunTable.pf_SPM_GetCrdSysParmAddr();
            pCrdCtrlParm +=  iCrdIndex;
            CMU_ResetSendCtrlData(pRcvCtrlData->ulRevID.ulFrameID, (uBit8*)pCrdCtrlParm, sizeof(CRDSYS_PARM));
            ulRet = CMU_ERR_SUCCESS;
            break;
        }
    case CRD_GETCMD_TOOL_DATA:                    //获取刀库数据
        {
#ifndef DISABLE_IPR_INTERFACE

            ulRet = m_sExternalFunTable.pf_IPR_GetToolData(pRcvCtrlData->ulRevID.ulComDataID.ulCrdNo,
                pRcvCtrlData->ulRevID.ulComDataID.ulDevNo, (CUTTER_T*)pSendBuf);

            if (ulRet==CMU_ERR_SUCCESS)
                CMU_AddToSendCtrlData(NULL, sizeof(CUTTER_T));
#endif
            break;
        }
        break;
    case CRD_GETCMD_UPLOAD_PROG_DATA:            //获取程序数据
        {
            memcpy(&ulStartPos, pRcvCtrlData->pRevBuf, sizeof(uBit32));
            memcpy(&ulLen, pRcvCtrlData->pRevBuf+sizeof(uBit32), sizeof(uBit32));
            
            if (CMU_CheckSendBufFreeSize(ulLen)==false)
            {
                ulRet = CMU_ERR_SEND_OVERFLOW;
                break;
            }

            ulRet = m_sExternalFunTable.pf_CSM_UploadProgData(iCrdIndex, ulStartPos, pSendBuf, ulLen);

            if (ulRet==CMU_ERR_SUCCESS)
                CMU_AddToSendCtrlData(NULL,ulLen);
    
            break;
        }
    case CRD_GETCMD_UPLOAD_USER_DATA:            //获取用户数据
    case CRD_GETCMD_UPLOAD_GCODE_DATA:            //获取G代码数据
        {
            ulRet = CMU_ERR_SUCCESS;
            memcpy(&ulStartPos, pRcvCtrlData->pRevBuf, sizeof(uBit32));
            memcpy(&ulLen, pRcvCtrlData->pRevBuf+sizeof(uBit32), sizeof(uBit32));

            if (CMU_CheckSendBufFreeSize(ulLen)==false)
            {
                ulRet = CMU_ERR_SEND_OVERFLOW;
                break;
            }

            ulRet = m_sExternalFunTable.pf_CSM_UploadUserData(iCrdIndex, ulStartPos, pSendBuf, ulLen);

            if (ulRet==CMU_ERR_SUCCESS)
                CMU_AddToSendCtrlData(NULL,ulLen);

            break;
        }
    case CRD_GETCMD_BP_DATA_LEN:        //获取断点数据长度
        {
            //获取长度
            ulLen = m_sExternalFunTable.pf_IPR_GetBpDataLen(iCrdIndex);
            memcpy(pSendBuf,&ulLen,sizeof(ulLen));
            CMU_AddToSendCtrlData(NULL,sizeof(ulLen));
            ulRet = CMU_ERR_SUCCESS;
            break;
        }
    case CRD_GETCMD_BP_DATA:            //获取断点数据
        {
            memcpy(&ulStartPos, pRcvCtrlData->pRevBuf, sizeof(uBit32));
            memcpy(&ulLen, pRcvCtrlData->pRevBuf+sizeof(uBit32), sizeof(uBit32));

            ulRet = m_sExternalFunTable.pf_IPR_GetBpData(iCrdIndex,ulStartPos, pSendBuf, ulLen);

            if (ulRet==CMU_ERR_SUCCESS)
                CMU_AddToSendCtrlData(NULL,ulLen);

            break;
        }
    case CRD_GETCMD_IPR_CUR_LINENO:        //获取当前解释行号
        {
            ulRet = m_sExternalFunTable.pf_IPR_GetCurLineNo(iCrdIndex, (Bit32*)pSendBuf);

            if (ulRet==CMU_ERR_SUCCESS)
                CMU_AddToSendCtrlData(NULL,sizeof(Bit32));

            break;
        }
    case CRD_GETCMD_BASE_TOOL:            //获取基本刀具号
        {
            ulRet = m_sExternalFunTable.pf_IPR_GetBaseToolNo(iCrdIndex, (Bit32*)pSendBuf);

            if (ulRet==CMU_ERR_SUCCESS)
                CMU_AddToSendCtrlData(NULL,sizeof(Bit32));

            break;
        }  
    case CRD_GETCMD_CRD_ZERO:            //获取坐标系零点位置
        {
            pCrdAxisMapAddr = m_sExternalFunTable.pf_SPM_GetCrdSysAxisMapTableAddr();
            pCrdAxisMapAddr +=  iCrdIndex;
            
            ulRet = m_sExternalFunTable.pf_IPR_GetCrdZero(iCrdIndex, pRcvCtrlData->ulRevID.ulComDataID.ulDevNo, (Bit32*)pSendBuf);

            if (ulRet==CMU_ERR_SUCCESS)
                CMU_AddToSendCtrlData(NULL,sizeof(Bit32)*pCrdAxisMapAddr->nCrdAxisCount);

            break;
        }  
    default:
        return CMU_ERR_INVALID_CMD;
        break;
    }

    return ulRet;
}


 /*
函数名称:uBit32 Crd_CmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData)

功    能:坐标系指令处理线程,如果设置指令执行完成并成功则回包,指令类型包括
            #define SETCMD_TYPE_NOMAL    (0)                        //常规设置指令
            #define SETCMD_TYPE_INQ        (1)                        //周期查询设置指令
            #define GETCMD_TYPE_NOMAL    (0)                        //常规获取指令
            #define GETCMD_TYPE_INQ        (1)                        //周期新数据获取指令

参    数:pRcvCtrlData--接收控制数据区

返 回 值:0--成功
          非0--错误码
注意事项:数据块接收完成后调用
*/ 
uBit32 Crd_CmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData)
{
    uBit32 ulRet = CMU_ERR_INVALID_CMD;

    if (pRcvCtrlData->ulRevID.ulComDataID.ulOperType==DATA_OPRE_TYPE_SET)//设置数据
    {
        switch(pRcvCtrlData->ulRevID.ulComDataID.ulCmdType)
        {
            case SETCMD_TYPE_NOMAL://常规设置指令
                    ulRet = Crd_NomalSetCmdProcess(pRcvCtrlData);
                    break;
            case SETCMD_TYPE_INQ://周期性设置指令
                    ulRet = CMU_AddInqRecord(pRcvCtrlData->ulRevID, *(uBit32*)pRcvCtrlData->pRevBuf);
                    break;
            default:
                    break;
        }

        if (ulRet==CMU_ERR_SUCCESS)
        {
            if(pRcvCtrlData->ulWaitResultCode==0) //无需等待处理结果
                CMU_SendResponsePack(pRcvCtrlData->ulRevID.ulFrameID, CMU_ERR_SUCCESS);
        }
    }
    else//获取数据
        {
            switch(pRcvCtrlData->ulRevID.ulComDataID.ulCmdType)
            {
                case GETCMD_TYPE_NOMAL://获取常规数据
                        ulRet = Crd_NomalGetCmdProcess(pRcvCtrlData);
                        break;
                case GETCMD_TYPE_INQ:        //获取周期性状态数据
                        ulRet = CMU_SendPeriodDataProc(pRcvCtrlData->ulRevID);
                        break;
                default:
                        break;
            }
        }

    return ulRet;
}
//------------------------------------------------------------------------------------

//设置电机控制参数(扩展)
uBit32 Motor_SetCtrlParmEx(uBit32 ulAxisNo, COM_RCV_CTRL_DATA *pRcvCtrlData)
{
    uBit32 ulParmType;
    uBit32 ulRet=CMU_ERR_SUCCESS;
    const AXIS_PARM *pAxisCtrlParmAddr;
    AXIS_PARM sAxisCtrlParm;

    pAxisCtrlParmAddr = m_sExternalFunTable.pf_SPM_GetAxisParmAddr();
    sAxisCtrlParm = pAxisCtrlParmAddr[ulAxisNo];
    ulParmType =  *(uBit32*)pRcvCtrlData->pRevBuf;

    //控制参数检查
    switch(ulParmType)
    {
    case MOTOR_PARM_MAXJOGSPD:    //(float32)最大手动速度,单位:0.001计数/毫秒,取值范围:大于0
        {
            memcpy(&sAxisCtrlParm.AxisMoveParm.dMaxJogSpeed, &pRcvCtrlData->pRevBuf[4], sizeof(float32));
            break;
        }
    case MOTOR_PARM_MAXJOGACC:    //(float32)最大手动加速度,单位:0.001计数/平方毫秒,取值范围:大于0
        {
            memcpy(&sAxisCtrlParm.AxisMoveParm.dMaxJogAcc, &pRcvCtrlData->pRevBuf[4], sizeof(float32));
            break;
        }
    case MOTOR_PARM_MAXFEEDSPD:    //(float32)最大加工速度,单位:0.001计数/毫秒,取值范围:大于0
        {
            memcpy(&sAxisCtrlParm.AxisMoveParm.dMaxFeedSpeed, &pRcvCtrlData->pRevBuf[4], sizeof(float32));
            break;
        }
    case MOTOR_PARM_MAXFEEDACC:    //(float32)最大加工加速度,单位:0.001计数/平方毫秒,取值范围:大于0
        {
            memcpy(&sAxisCtrlParm.AxisMoveParm.dMaxFeedAcc, &pRcvCtrlData->pRevBuf[4], sizeof(float32));
            break;
        }
    case MOTOR_PARM_RAPIDSPD:        //(float32)快移速度,单位:0.001计数/毫秒,取值范围:大于0
        {
            memcpy(&sAxisCtrlParm.AxisMoveParm.dRapidSpeed, &pRcvCtrlData->pRevBuf[4], sizeof(float32));
            break;
        }
    case MOTOR_PARM_RAPIDACC:        //(float32)快移加速度,单位:0.001计数/平方毫秒,取值范围:大于0
        {
            memcpy(&sAxisCtrlParm.AxisMoveParm.dRapidAcc, &pRcvCtrlData->pRevBuf[4], sizeof(float32));
            break;
        }

    default:
        return CMU_ERR_DATA_WRONG;
        break;
    }

    ulRet = m_sExternalFunTable.pf_SPM_SetAxisParm(ulAxisNo, &sAxisCtrlParm);
    return ulRet;
}




//设置轴基本控制参数
uBit32 Motor_SetAxisBaseCtrlPram(uBit32 ulAxisNo, COM_RCV_CTRL_DATA *pRcvCtrlData)
{
    uBit32 ulParmType;
    uBit32 ulRet=CMU_ERR_SUCCESS;
    const AXIS_PARM *pAxisCtrlParmAddr;
    AXIS_PARM sAxisCtrlParm;

    pAxisCtrlParmAddr = m_sExternalFunTable.pf_SPM_GetAxisParmAddr();
    sAxisCtrlParm = pAxisCtrlParmAddr[ulAxisNo];
    ulParmType =  *(uBit32*)pRcvCtrlData->pRevBuf;
    
    float fTempParm = (*(uBit32 *)(&pRcvCtrlData->pRevBuf[4]))/10000.0;
    
    
    //控制参数检查
    switch(ulParmType)
    {
    case AXIS_BASE_PARM_MAX_JOG_SPD:            //(float32)最大手动速度,单位:0.001计数/毫秒,取值范围:大于0
        {
            memcpy(&sAxisCtrlParm.AxisMoveParm.dMaxJogSpeed, &fTempParm, sizeof(float32));
            break;
        }
    case AXIS_BASE_PARM_MAX_JOG_ACC:            //(float32)最大手动加速度,单位:0.001计数/平方毫秒,取值范围:大于0
        {
            memcpy(&sAxisCtrlParm.AxisMoveParm.dMaxJogAcc, &fTempParm, sizeof(float32));
            break;
        }
    case AXIS_BASE_PARM_JOG_JERK:               //(float32)手动捷度,单位:用户计数单位/立方毫秒,取值范围:大于0
        {
            memcpy(&sAxisCtrlParm.AxisMoveParm.dJogJerk, &fTempParm, sizeof(float32));
            break;
        }
    case AXIS_BASE_PARM_GEAR_RATIO_POS_SCALE:   //(Bit32)轴脉冲当量分子,单位:用户计数单位,取值范围:大于0
        {
            memcpy(&sAxisCtrlParm.AxisScaleParm.lGearRatioPosScale, &pRcvCtrlData->pRevBuf[4], sizeof(Bit32));
            break;
        }
    case AXIS_BASE_PARM_GEAR_RATIO_PLUS_SCALE:  //(Bit32)轴脉冲当量分母,单位:脉冲数,取值范围:大于0
        {
            memcpy(&sAxisCtrlParm.AxisScaleParm.lGearRatioPlusScale, &pRcvCtrlData->pRevBuf[4], sizeof(Bit32));
            break;
        }
    case AXIS_BASE_PARM_SOFT_POS_LIM_P:         //(Bit32)正软限位,单位:用户计数单位,该值为0时,表示不检测正软限位
        {
            memcpy(&sAxisCtrlParm.AxisSafeParm.lSoftPosLimP, &pRcvCtrlData->pRevBuf[4], sizeof(Bit32));
            break;
        }
    case AXIS_BASE_PARM_SOFT_POS_LIM_N:         //(Bit32)负软限位,单位:用户计数单位,该值为0时,表示不检测负软限位
        {
            memcpy(&sAxisCtrlParm.AxisSafeParm.lSoftPosLimN, &pRcvCtrlData->pRevBuf[4], sizeof(Bit32));
            break;
        }

    default:
        return CMU_ERR_DATA_WRONG;
        break;
    }

    ulRet = m_sExternalFunTable.pf_SPM_SetAxisParm(ulAxisNo, &sAxisCtrlParm);   //设置电机控制参数
    return ulRet;
}




//电机指令处理函数--------------------------------------------------------------------
 /*
函数名称:uBit32 Motor_NomalSetCmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData)

功    能:电机或轴常规设置指令处理线程,处理的指令包括
            #define MOTOR_SETCMD_CTRL_PARM               (1) //设置电机控制参数
            #define MOTOR_SETCMD_PITCH_CMP_PARM          (2) //设置电机螺距补偿
            #define MOTOR_SETCMD_POS_MOTION              (3) //设置位置控制运动数据
            #define MOTOR_SETCMD_SPEED_MOTION            (4) //设置速度控制运动数据
            #define MOTOR_SETCMD_HOME                    (5) //电机回零
            #define MOTOR_SETCMD_STOP                    (6) //电机停止
            #define MOTOR_SETCMD_ESTOP                   (7) //电机急停
            #define MOTOR_SETCMD_CMD_POS                 (8) //设置电机当前位置
            #define MOTOR_SETCMD_QEI_POS                 (9) //设置电机编码器当前位置
            #define MOTOR_SETCMD_ENABLE                  (10)//设置伺服使能0-断使能1-加使能
                                                         
            #define MOTOR_SETCMD_AXIS_POS_MOTION         (11)
            #define MOTOR_SETCMD_AXIS_SPEED_MOTION       (12)
            #define MOTOR_SETCMD_AXIS_HOME               (13)
            #define MOTOR_SETCMD_AXIS_STOP               (14)
            #define MOTOR_SETCMD_AXIS_ESTOP              (15)
            #define MOTOR_SETCMD_AXIS_CMD_POS            (16)
            #define MOTOR_SETCMD_AXIS_QEI_POS            (17)
            #define MOTOR_SETCMD_AXIS_ENABLE             (18)

参    数:pRcvCtrlData--接收控制数据区

返 回 值:0--成功
          非0--错误码
注意事项:电机指令处理线程调用
*/ 
uBit32 Motor_NomalSetCmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData)
{
    uBit32 ulRet=CMU_ERR_INVALID_CMD;
    uBit32 ulMotorNo = pRcvCtrlData->ulRevID.ulComDataID.ulDevNo;
    uBit32 ulCrdIndex = pRcvCtrlData->ulRevID.ulComDataID.ulCrdNo;
    uBit32 ulAxisMask = ulMotorNo;
    uBit32 ulAxisIndex = ulMotorNo;

    switch(pRcvCtrlData->ulRevID.ulComDataID.ulCmdIndex)
    {
    case MOTOR_SETCMD_CTRL_PARM:                    //设置电机控制参数
        {
            ulRet = m_sExternalFunTable.pf_SPM_SetAxisParm(ulMotorNo, (AXIS_PARM*)pRcvCtrlData->pRevBuf);
            break;
        }
    case MOTOR_SETCMD_CTRL_PARM_EX:                 //设置电机控制参数
        {
            ulRet = Motor_SetCtrlParmEx(ulMotorNo, pRcvCtrlData);
            break;
        }
    case MOTOR_SETCMD_PITCH_CMP_PARM:                //设置电机螺距补偿
        {
            ulRet = m_sExternalFunTable.pf_SPM_SetAxisPitchCmpParm(ulMotorNo, (AXIS_PITCH_CMP_PARM*)pRcvCtrlData->pRevBuf);
            break;
        }
    case MOTOR_SETCMD_POS_MOTION:                   //设置位置控制运动数据
        {
            ulRet = m_sExternalFunTable.pf_CSM_SetMotorPosCtrlMotion(ulMotorNo, (POSCTRL_MOTION_DATA*)pRcvCtrlData->pRevBuf);
            break;
        }
    case MOTOR_SETCMD_SPEED_MOTION:                 //设置速度控制运动数据
        {
            ulRet = m_sExternalFunTable.pf_CSM_SetMotorSpeedCtrlMotion(ulMotorNo, (SPEEDCTRL_MOTION_DATA*)pRcvCtrlData->pRevBuf);
            break;
        }
    case MOTOR_SETCMD_HOME:                         //电机回零
        {
            ulRet = m_sExternalFunTable.pf_PAX_Home(ulMotorNo, *(BooLean*)pRcvCtrlData->pRevBuf, 0);
#ifndef WIN32_VER
            //MIII总线的回零指令回应需要至少4-6个通信周期
            if (ulRet==CMU_ERR_SUCCESS)
            {
                pRcvCtrlData->uCmdModule = 0;
                pRcvCtrlData->ulWaitResultCode = 1;
                pRcvCtrlData->uCmdType = DEV_CMD_SET_HOME;
                pRcvCtrlData->ulTimeOutCount = 10;
                pRcvCtrlData->uDevNo = ulMotorNo;
                pRcvCtrlData->dStartTick = m_sExternalFunTable.pf_SYS_GetSysTickCount();
            }
#endif
            break;
        }
    case MOTOR_SETCMD_STOP:                        //电机停止
        {
            ulRet = m_sExternalFunTable.pf_CSM_SetMotorJogStop(ulMotorNo);
            break;
        }
    case MOTOR_SETCMD_ESTOP:                        //电机急停
        {
            ulRet = m_sExternalFunTable.pf_CSM_MotorJogEStop(ulMotorNo);
            break;
        }
    case MOTOR_SETCMD_CMD_POS:                    //设置电机当前指令位置
        {//指令位置不下发,所有不等待
            ulRet = m_sExternalFunTable.pf_PAX_SetCmdPos(ulMotorNo, *(Bit32*)pRcvCtrlData->pRevBuf,  0);
            break;
        }
    case MOTOR_SETCMD_QEI_POS:                    //设置电机编码器当前位置
        {

            ulRet = m_sExternalFunTable.pf_PAX_SetQeiPos(ulMotorNo, *(Bit32*)pRcvCtrlData->pRevBuf, 0);
#ifndef WIN32_VER
            //MIII总线的位置设置指令回应需要至少4-6个通信周期
            if (ulRet==CMU_ERR_SUCCESS)
            {
                pRcvCtrlData->uCmdModule = 0;
                pRcvCtrlData->ulWaitResultCode = 1;
                pRcvCtrlData->uCmdType = DEV_CMD_SET_QEI_POS;
                pRcvCtrlData->ulTimeOutCount = 10;
                pRcvCtrlData->uDevNo = ulMotorNo;
                pRcvCtrlData->dStartTick = m_sExternalFunTable.pf_SYS_GetSysTickCount();
            }
#endif
            break;
        }
    case MOTOR_SETCMD_ENABLE:                        //设置伺服使能0-断使能1-加使能
        {

            ulRet = m_sExternalFunTable.pf_PAX_Enable(ulMotorNo, *(Bit32*)pRcvCtrlData->pRevBuf, 0);
#ifndef WIN32_VER
            //MIII总线的伺服使能回应需要至少4-42个通信周期
            if (ulRet==CMU_ERR_SUCCESS)
            {
                pRcvCtrlData->uCmdModule = 0;
                pRcvCtrlData->ulWaitResultCode = 1;
                pRcvCtrlData->uCmdType = DEV_CMD_ENABLE_SV;
                pRcvCtrlData->ulTimeOutCount = 50;
                pRcvCtrlData->uDevNo = ulMotorNo;
                pRcvCtrlData->dStartTick = m_sExternalFunTable.pf_SYS_GetSysTickCount();
            }
#endif
            break;
        }
    case MOTOR_SETCMD_RESET:                        //电机复位
        {
            ulRet = m_sExternalFunTable.pf_PAX_Reset(ulMotorNo);
            break;
        }
    case MOTOR_SETCMD_SV_PARM:                      //电机复位
        {
            ulRet = m_sExternalFunTable.pf_PAX_SendSvParm(ulMotorNo, pRcvCtrlData->pRevBuf);
            break;
        }
    case MOTOR_SETCMD_AXIS_POS_MOTION:              //设置轴位置模式运动 
        {
            ulRet = m_sExternalFunTable.pf_CSM_SetAxisPosCtrlMotion(ulCrdIndex, ulAxisMask, (POSCTRL_MOTION_DATA*)pRcvCtrlData->pRevBuf);
            break;
        }
    case MOTOR_SETCMD_AXIS_SPEED_MOTION:            //设置轴速度模式运动
        {
            ulRet = m_sExternalFunTable.pf_CSM_SetAxisSpeedCtrlMotion(ulCrdIndex, ulAxisMask, (SPEEDCTRL_MOTION_DATA*)pRcvCtrlData->pRevBuf);
            break;
        }
    case MOTOR_SETCMD_AXIS_HOME:                    //设置轴回零
        {
            ulRet = m_sExternalFunTable.pf_LAX_Home(ulCrdIndex, ulAxisIndex, *(Bit32*)pRcvCtrlData->pRevBuf, 0);
#ifndef WIN32_VER
            //MIII总线的回零回应需要至少4-6个通信周期
            if (ulRet==CMU_ERR_SUCCESS)
            {
                pRcvCtrlData->uCmdModule = 1;
                pRcvCtrlData->ulWaitResultCode = 1;
                pRcvCtrlData->uCmdType = DEV_CMD_SET_HOME;
                pRcvCtrlData->ulTimeOutCount = 10;
                pRcvCtrlData->uDevNo = ulAxisIndex;
                pRcvCtrlData->uCrdNo = ulCrdIndex;
                pRcvCtrlData->dStartTick = m_sExternalFunTable.pf_SYS_GetSysTickCount();
            }
#endif
            break;
        }
    case MOTOR_SETCMD_AXIS_STOP:
        {
            ulRet = m_sExternalFunTable.pf_CSM_SetAxisJogStop(ulCrdIndex, ulAxisMask);
            break;
        }
    case MOTOR_SETCMD_AXIS_ESTOP:
        {
            ulRet = m_sExternalFunTable.pf_CSM_AxisJogEStop(ulCrdIndex, ulAxisMask);
            break;
        }
    case MOTOR_SETCMD_AXIS_CMD_POS:    
        {//指令位置不下发,所以不等待
            ulRet = m_sExternalFunTable.pf_LAX_SetCmdPos(ulCrdIndex, ulAxisIndex, *(Bit32*)pRcvCtrlData->pRevBuf, 0);
            break;
        }
    case MOTOR_SETCMD_AXIS_QEI_POS:        
        {
            ulRet = m_sExternalFunTable.pf_LAX_SetQeiPos(ulCrdIndex, ulAxisIndex, *(Bit32*)pRcvCtrlData->pRevBuf,0);

#ifndef WIN32_VER
            //MIII总线的位置设置指令回应需要至少4-6个通信周期
            if (ulRet==CMU_ERR_SUCCESS)
            {
                pRcvCtrlData->uCmdModule = 1;
                pRcvCtrlData->ulWaitResultCode = 1;
                pRcvCtrlData->uCmdType = DEV_CMD_SET_QEI_POS;
                pRcvCtrlData->ulTimeOutCount = 10;
                pRcvCtrlData->uDevNo = ulAxisIndex;
                pRcvCtrlData->uCrdNo = ulCrdIndex;
                pRcvCtrlData->dStartTick = m_sExternalFunTable.pf_SYS_GetSysTickCount();
            }
#endif
            break;
        }
    case MOTOR_SETCMD_AXIS_ENABLE:
        {
            ulRet = m_sExternalFunTable.pf_LAX_Enable(ulCrdIndex, ulAxisIndex, *(BooLean*)pRcvCtrlData->pRevBuf,0);

#ifndef WIN32_VER
            //MIII总线的伺服使能回应需要至少4-42个通信周期
            if (ulRet==CMU_ERR_SUCCESS)
            {
                pRcvCtrlData->uCmdModule = 1;
                pRcvCtrlData->ulWaitResultCode = 1;
                pRcvCtrlData->uCmdType = DEV_CMD_ENABLE_SV;
                pRcvCtrlData->ulTimeOutCount = 50;
                pRcvCtrlData->uDevNo = ulAxisIndex;
                pRcvCtrlData->uCrdNo = ulCrdIndex;
                pRcvCtrlData->dStartTick = m_sExternalFunTable.pf_SYS_GetSysTickCount();
            }
#endif
            break;
        }
        
    case MOTOR_SETCMD_BASE_CTRL_PARM:   //基本控制参数设置 2017.12.11 Duanfeng
        {
            ulRet = Motor_SetAxisBaseCtrlPram(ulMotorNo, pRcvCtrlData);
            break;
        }
        
    case MOTOR_SETCMD_POS_MOTION_EX:    //设置位置控制运动数据(扩展)
        {
            POSCTRL_MOTION_DATA PosMotionData = {0};
            PosMotionData.dPos = (*(long int *)(&pRcvCtrlData->pRevBuf[0])) / 10000.0;
            PosMotionData.dSpeed = (*(long int *)(&pRcvCtrlData->pRevBuf[4])) / 10000.0;
            
            ulRet = m_sExternalFunTable.pf_CSM_SetMotorPosCtrlMotion(ulMotorNo, &PosMotionData);
            break;
        }
    case MOTOR_SETCMD_SPEED_MOTION_EX:  //设置速度控制运动数据(扩展)
        {
            SPEEDCTRL_MOTION_DATA SpeedMotionData = {0};
            SpeedMotionData.dAcc   = (*(long int *)(&pRcvCtrlData->pRevBuf[0])) / 10000.0;
            SpeedMotionData.dDec   = (*(long int *)(&pRcvCtrlData->pRevBuf[4])) / 10000.0;
            SpeedMotionData.dJerk  = (*(long int *)(&pRcvCtrlData->pRevBuf[8])) / 10000.0;
            SpeedMotionData.dSpeed = (*(long int *)(&pRcvCtrlData->pRevBuf[12]))/ 10000.0;
            ulRet = m_sExternalFunTable.pf_CSM_SetMotorSpeedCtrlMotion(ulMotorNo, &SpeedMotionData);
            break;
        }
    case MOTOR_SETCMD_PULSE_RATE:       //获取脉冲当量
        {
            float fPluseRate = (*(long int *)(&pRcvCtrlData->pRevBuf[0])) / 10000.0;
            ulRet = m_sExternalFunTable.pf_PAX_SetPulseRate(fPluseRate);
        }
        
    default:break;
    }

    return ulRet;
}


/*
函数名称:uBit32 Motor_NomalGetCmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData)

功    能:电机或轴常规获取指令处理线程,如果获取成功则配置回包,指令包括
                #define MOTOR_GETCMD_CTRL_PARM                      (1)//电机所有控制参数
                #define MOTOR_GETCMD_SIGNAL_PARM                    (2)//信号量控制参数
                #define MOTOR_GETCMD_SAFE_PARM                      (3)//安全控制参数
                #define MOTOR_GETCMD_HOME_PARM                      (4)//回零控制参数
                #define MOTOR_GETCMD_SCALE_PARM                     (5)//比例控制参数
                #define MOTOR_GETCMD_MOVE_PARM                      (6)//运动控制参数
                #define MOTOR_GETCMD_BACKLASH_CMP_PARM              (7)//反向间隙补偿
                #define MOTOR_GETCMD_PITCH_CMP_PARM                 (8)//螺距补偿参数
                #define MOTOR_GETCMD_SV_PARM                        (9)//获取伺服参数
参    数:pRcvCtrlData--接收控制数据区

返 回 值:0--成功
          非0--错误码
注意事项:电机指令处理线程调用
*/
uBit32 Motor_NomalGetCmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData)
{
    uBit32  ulRet = CMU_ERR_INVALID_CMD;
    uBit8 *pParmBuf;
    const AXIS_PARM* pAxisParm;
    const AXIS_PITCH_CMP_PARM *pAxisPitchCmpParm;
    
    pAxisParm = m_sExternalFunTable.pf_SPM_GetAxisParmAddr();
    pAxisParm += pRcvCtrlData->ulRevID.ulComDataID.ulDevNo;

    ulRet = CMU_ERR_SUCCESS;
    
    switch(pRcvCtrlData->ulRevID.ulComDataID.ulCmdIndex)
    {
    case MOTOR_GETCMD_PITCH_CMP_PARM:                    //获取螺距补偿参数
        {
            pAxisPitchCmpParm = m_sExternalFunTable.pf_SPM_GetAxisPitchCmpParmAddr();
            pAxisPitchCmpParm += pRcvCtrlData->ulRevID.ulComDataID.ulDevNo;
            CMU_ResetSendCtrlData(pRcvCtrlData->ulRevID.ulFrameID, (uBit8*)pAxisPitchCmpParm, sizeof(AXIS_PITCH_CMP_PARM));
            break;
        }
    case MOTOR_GETCMD_SV_PARM:                          //获取伺服参数
        {
            CMU_ResetSendCtrlData(pRcvCtrlData->ulRevID.ulFrameID, NULL, 0);
            pParmBuf = CMU_GetSendBufAddr();
            ulRet = m_sExternalFunTable.pf_PAX_GetSvParm(pRcvCtrlData->ulRevID.ulComDataID.ulDevNo,
                                                         *(uBit16*)pRcvCtrlData->pRevBuf, pParmBuf);
            CMU_AddToSendCtrlData(NULL, 4);
            break;
        }
    case MOTOR_GETCMD_CTRL_PARM:                        //电机所有控制参数
        {
            CMU_ResetSendCtrlData(pRcvCtrlData->ulRevID.ulFrameID, (uBit8*)pAxisParm, sizeof(AXIS_PARM));
            break;
        }
    case MOTOR_GETCMD_SIGNAL_PARM:                      //信号量控制参数
        {
            CMU_ResetSendCtrlData(pRcvCtrlData->ulRevID.ulFrameID, (uBit8*)&pAxisParm->AxisSignalConfig, sizeof(AXIS_SIGNAL_CONFIG));
            break;
        }
    case MOTOR_GETCMD_SAFE_PARM:                        //安全控制参数
        {
            CMU_ResetSendCtrlData(pRcvCtrlData->ulRevID.ulFrameID, (uBit8*)&pAxisParm->AxisSafeParm, sizeof(AXIS_SAFE_PARM));
            break;
        }
    case MOTOR_GETCMD_HOME_PARM:                        //回零控制参数
        {
            CMU_ResetSendCtrlData(pRcvCtrlData->ulRevID.ulFrameID, (uBit8*)&pAxisParm->AxisHomeParm, sizeof(AXIS_HOME_PARM));
            break;
        }
    case MOTOR_GETCMD_SCALE_PARM:                       //比例控制参数
        {
            CMU_ResetSendCtrlData(pRcvCtrlData->ulRevID.ulFrameID, (uBit8*)&pAxisParm->AxisScaleParm, sizeof(AXIS_SCALE_PARM));
            break;
        }
    case MOTOR_GETCMD_MOVE_PARM:                        //运动控制参数
        {
            CMU_ResetSendCtrlData(pRcvCtrlData->ulRevID.ulFrameID, (uBit8*)&pAxisParm->AxisMoveParm, sizeof(AXIS_MOVE_PARM));
            break;
        }
    case MOTOR_GETCMD_BACKLASH_CMP_PARM:                //反向间隙补偿
        {
            CMU_ResetSendCtrlData(pRcvCtrlData->ulRevID.ulFrameID, (uBit8*)&pAxisParm->AxisCmpParm, sizeof(AXIS_BACK_LASH_CMP_PARM));
            break;
        }
        
    //2017.12.16 Duanfeng 新增
    case MOTOR_GETCMD_RUNNING_STATUS:                   //获取电机运行状态
        {
            uBit32 ulAxisNO = (*(long int *)(&pRcvCtrlData->pRevBuf[0]));
            CMU_ResetSendCtrlData(pRcvCtrlData->ulRevID.ulFrameID,NULL,0);
            uBit8 *pSendBuf = CMU_GetSendBufAddr();
            
            *(uBit32 *)pSendBuf = m_sExternalFunTable.pf_PAX_GetRunningStatus(ulAxisNO);
            CMU_AddToSendCtrlData(NULL,sizeof(uBit32));
            break;
        }
    case MOTOR_GETCMD_CMD_POS:                          //获取指令位置
        {
            uBit32 ulAxisNO = (*(long int *)(&pRcvCtrlData->pRevBuf[0]));
            CMU_ResetSendCtrlData(pRcvCtrlData->ulRevID.ulFrameID,NULL,0);
            uBit8 *pSendBuf = CMU_GetSendBufAddr();
            
            *(uBit32 *)pSendBuf = (uBit32)(m_sExternalFunTable.pf_PAX_GetCmdPos(ulAxisNO) * 10000.0); //将浮点数转换成整型传输,放大因子:10000
            CMU_AddToSendCtrlData(NULL,sizeof(uBit32));
            break;
        }
        
    default: ulRet = CMU_ERR_INVALID_CMD;
    }
          

    return ulRet;
}

 /*
函数名称:uBit32 Motor_CmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData)

功    能:电机或轴指令处理线程,如果设置指令执行完成并成功则回包,指令类型包括
            #define SETCMD_TYPE_NOMAL       (0)         //常规设置指令
            #define SETCMD_TYPE_INQ         (1)         //周期查询设置指令
            #define GETCMD_TYPE_NOMAL       (0)         //常规获取指令
            #define GETCMD_TYPE_INQ         (1)         //周期新数据获取指令

参    数:pRcvCtrlData--接收控制数据区

返 回 值:0--成功
          非0--错误码
注意事项:数据块接收完成后调用
*/ 
uBit32 Motor_CmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData)
{
    uBit32 ulRet = CMU_ERR_INVALID_CMD;

    if (pRcvCtrlData->ulRevID.ulComDataID.ulOperType==DATA_OPRE_TYPE_SET)//设置数据
    {
        switch(pRcvCtrlData->ulRevID.ulComDataID.ulCmdType)
        {
        case SETCMD_TYPE_NOMAL://常规设置指令
            ulRet = Motor_NomalSetCmdProcess(pRcvCtrlData);
            break;
        case SETCMD_TYPE_INQ://周期性设置指令
            ulRet = CMU_AddInqRecord(pRcvCtrlData->ulRevID, *(uBit32*)pRcvCtrlData->pRevBuf);
            break;
        default:break;
        }

        if (ulRet==CMU_ERR_SUCCESS)//
        {
            if(pRcvCtrlData->ulWaitResultCode==0) //无需等待处理结果
                CMU_SendResponsePack(pRcvCtrlData->ulRevID.ulFrameID, CMU_ERR_SUCCESS);
        }
    }else//获取数据
    {
        switch(pRcvCtrlData->ulRevID.ulComDataID.ulCmdType)
        {
        case GETCMD_TYPE_NOMAL://获取常规数据
            ulRet = Motor_NomalGetCmdProcess(pRcvCtrlData);
            break;
        case GETCMD_TYPE_INQ:            //获取周期性状态数据
            ulRet = CMU_SendPeriodDataProc(pRcvCtrlData->ulRevID);
            break;
        default:break;
        }
    }

    return ulRet;
}

 /*
函数名称:uBit32 Io_NomalSetCmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData)

功    能:IO常规设置指令处理线程,处理的指令包括
        #define IO_SETCMD_STATE                 (1)//设置IO板输出状态
        #define IO_SETCMD_PWM_ENABLE            (2)//设置IO板PWM功能
        #define IO_SETCMD_PWM_DUTY_RATIO        (3)//设置IO板IO口PWM占空比
        #define IO_SETCMD_PWM_FRQ               (4)//设置IO板IO口PWM频率
参    数:pRcvCtrlData--接收控制数据区

返 回 值:0--成功
          非0--错误码
注意事项:
*/ 
uBit32 Io_NomalSetCmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData)
{
    uBit32 ulData = 0;
    uBit32 ulBitMask = 0;
    uBit32 ulRet=CMU_ERR_INVALID_CMD;
    uBit32 ulIoNo = pRcvCtrlData->ulRevID.ulComDataID.ulDevNo;

    memcpy(&ulBitMask, pRcvCtrlData->pRevBuf, sizeof(uBit32));
    memcpy(&ulData, pRcvCtrlData->pRevBuf+sizeof(uBit32),sizeof(uBit32));
    
    switch(pRcvCtrlData->ulRevID.ulComDataID.ulCmdIndex)
    {
    case IO_SETCMD_STATE:                    //设置IO板输出状态
        {
#ifdef RS_MONITOR_ENALBE
            g_sCmuRsMonitorData.ulOutputSetCount++;
#endif
            ulRet = m_sExternalFunTable.pf_IO_SetOutputStatus(ulIoNo, ulData, ulBitMask);
            break;
        }
    case IO_SETCMD_PWM_ENABLE:                //设置IO板PWM功能,ulBitMask为IO引脚编号
        {
            ulRet = m_sExternalFunTable.pf_IO_SetOutPutPWMMode(ulIoNo, ulBitMask, ulData);
            break;
        }
    case IO_SETCMD_PWM_DUTY_RATIO:            //设置IO板IO口PWM占空比,ulBitMask为IO引脚编号
        {
            ulRet = m_sExternalFunTable.pf_IO_SetOutPutPWMDuty(ulIoNo, ulBitMask, ulData);
            break;
        }
    case IO_SETCMD_PWM_FRQ:            //设置IO板IO口PWM频率,ulBitMask为IO引脚编号
        {
            ulRet = m_sExternalFunTable.pf_IO_SetOutPutPWMFreq(ulIoNo, ulBitMask, ulData);
            break;
        }
    case IO_SETCMD_HSPD_STATE:            //设置高速IO口输出状态
        {
            ulRet = m_sExternalFunTable.pf_GPIO_SetHSpdOutputState(ulBitMask, ulData);
            break;
        }
        
    //2017.12.01 新增 -- 杜寒枫
    case IO_SETCMD_HSPD_OUT_IONO:       //设置输出IO状态(编号方式控制)
        {
            uBit32 uIONO = 0; 
            uBit32 ulState = 0;
            memcpy(&uIONO, pRcvCtrlData->pRevBuf, sizeof(uBit32));
            memcpy(&ulState, pRcvCtrlData->pRevBuf+sizeof(uBit32),sizeof(ulState));
            
            m_sExternalFunTable.pf_GPIO_SetOutputPinState(uIONO, ulState);
            
            ulRet = CMU_ERR_SUCCESS;
            break;
        }
    default:break;
    }

    return ulRet;
}

 /*
函数名称:uBit32 Io_NomalGetCmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData);

功    能:IO常规获取指令处理线程,如果获取成功则配置回包,指令包括
                #define IO_GETCMD_HSPD_OUT_STATUS   (1)//获取高速IO输出口状态
                #define IO_GETCMD_HSPD_IN_STATUS    (2)//获取高速IO输入口状态
参    数:pRcvCtrlData--接收控制数据区

返 回 值:0--成功
          非0--错误码
注意事项:IO指令处理线程调用
*/ 
uBit32 Io_NomalGetCmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData)
{
    uBit8 *pSendBuf;
    uBit32  ulRet = CMU_ERR_INVALID_CMD;

    CMU_ResetSendCtrlData(pRcvCtrlData->ulRevID.ulFrameID,NULL,0);
    pSendBuf = CMU_GetSendBufAddr();


    switch(pRcvCtrlData->ulRevID.ulComDataID.ulCmdIndex)
    {
    case IO_GETCMD_HSPD_OUT_STATUS: //获取高速IO输出口状态
        {
            *((uBit32*)pSendBuf) = m_sExternalFunTable.pf_GPIO_GetHSpdOutputStatus();
            ulRet = CMU_ERR_SUCCESS;
            CMU_AddToSendCtrlData(NULL,sizeof(uBit32));
            break;
        }
        
    case IO_GETCMD_HSPD_IN_STATUS:  //获取高速IO输入口状态
        {
            *((uBit32*)pSendBuf) = m_sExternalFunTable.pf_GPIO_GetHSpdInputStatus();
            ulRet = CMU_ERR_SUCCESS;
            CMU_AddToSendCtrlData(NULL,sizeof(uBit32));
            break;
        }
        
    //2017.12.01 新增 -- 杜寒枫
    case IO_GETCMD_HSPD_OUT_IONO:   //获取高速IO输出口状态(编号方式控制)
        {
            uBit32 uIONO = 0; 
            memcpy(&uIONO, pRcvCtrlData->pRevBuf, sizeof(uBit32));
            
            *((uBit32*)pSendBuf) = m_sExternalFunTable.pf_GPIO_GetOutputPinState(uIONO);
            ulRet = CMU_ERR_SUCCESS;
            CMU_AddToSendCtrlData(NULL,sizeof(uBit32));
            break;
        }
    case IO_GETCMD_HSPD_IN_IONO:   //获取高速IO输出口状态(编号方式控制)
        {
            uBit32 uIONO = 0; 
            memcpy(&uIONO, pRcvCtrlData->pRevBuf, sizeof(uBit32));
            
            *((uBit32*)pSendBuf) = m_sExternalFunTable.pf_GPIO_GetInputPinState(uIONO);
            ulRet = CMU_ERR_SUCCESS;
            CMU_AddToSendCtrlData(NULL,sizeof(uBit32));
            break;
        }
    default:break;
    }
    
    return ulRet;
}

 /*
函数名称:uBit32 Io_CmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData)

功    能:IO指令处理线程,如果设置指令执行完成并成功则回包,指令类型包括
            #define SETCMD_TYPE_NOMAL       (0)         //常规设置指令
            #define SETCMD_TYPE_INQ         (1)         //周期查询设置指令
            #define GETCMD_TYPE_NOMAL       (0)         //常规获取指令
            #define GETCMD_TYPE_INQ         (1)         //周期新数据获取指令

参    数:pRcvCtrlData--接收控制数据区

返 回 值:0--成功
          非0--错误码
注意事项:数据块接收完成后调用
*/ 
uBit32 Io_CmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData)
{
    uBit32 ulRet = CMU_ERR_INVALID_CMD;

    if (pRcvCtrlData->ulRevID.ulComDataID.ulOperType==DATA_OPRE_TYPE_SET)//设置数据
    {
        switch(pRcvCtrlData->ulRevID.ulComDataID.ulCmdType)
        {
        case SETCMD_TYPE_NOMAL:    //常规设置指令
            ulRet = Io_NomalSetCmdProcess(pRcvCtrlData);
            break;
        case SETCMD_TYPE_INQ:    //周期性设置指令
            ulRet = CMU_AddInqRecord(pRcvCtrlData->ulRevID, *(uBit32*)pRcvCtrlData->pRevBuf);
            break;
        default:break;
        }

        if (ulRet==CMU_ERR_SUCCESS)//
        {
            if(pRcvCtrlData->ulWaitResultCode==0) //无需等待处理结果
                CMU_SendResponsePack(pRcvCtrlData->ulRevID.ulFrameID, CMU_ERR_SUCCESS);
        }
    }else//获取数据
    {
        switch(pRcvCtrlData->ulRevID.ulComDataID.ulCmdType)
        {
        case GETCMD_TYPE_NOMAL://获取常规数据
            ulRet = Io_NomalGetCmdProcess(pRcvCtrlData);
            break;
        case GETCMD_TYPE_INQ:        //获取周期性状态数据
            ulRet = CMU_SendPeriodDataProc(pRcvCtrlData->ulRevID);
            break;
        default:break;
        }
    }

    return ulRet;
}


 /*
函数名称:uBit32 Adda_NomalSetCmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData)

功    能:ADDA常规设置指令处理线程,处理的指令包括
            #define ADDA_SETCMD_DATA     (1)            //设置DA板输出
            #define ADDA_SETCMD_HDATA    (2)            //设置高速DA输出
参    数:pRcvCtrlData--接收控制数据区

返 回 值:0--成功
          非0--错误码
注意事项:ADDA指令处理线程调用
*/ 
uBit32 Adda_NomalSetCmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData)
{
    //uBit32 ulData;
    uBit32 ulRet=CMU_ERR_INVALID_CMD;

    switch(pRcvCtrlData->ulRevID.ulADDADataID.ulCmdIndex)
    {
    case ADDA_SETCMD_DATA:                    //设置DA输出
        {
            ulRet = m_sExternalFunTable.pf_ADDA_SetDAStatus(pRcvCtrlData->ulRevID.ulADDADataID.ulADDANo,\
                *(Bit32*)pRcvCtrlData->pRevBuf,\
                pRcvCtrlData->ulRevID.ulADDADataID.ulChMask);

            break;
        }
    case ADDA_SETCMD_HDATA:
        {
            ulRet = m_sExternalFunTable.pf_MDAC_SetHSpdDaData(pRcvCtrlData->ulRevID.ulADDADataID.ulChMask,*(uBit32*)pRcvCtrlData->pRevBuf);
            if (ulRet)
                ulRet = CMU_ERR_DATA_WRONG;

            break;
        }
    default:break;
    }

    return ulRet;
}

 /*
函数名称:uBit32 Adda_NomalGetCmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData)

功    能:ADDA常规获取指令处理线程,如果获取成功则配置回包,指令包括
            #define ADDA_GETCMD_HDATA                (1)//获取高速DA输出值

参    数:pRcvCtrlData--接收控制数据区

返 回 值:0--成功
          非0--错误码
注意事项:ADDA指令处理线程调用
*/ 
uBit32 Adda_NomalGetCmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData)
{
    uBit8 *pSendBuf;
    uBit32  ulRet = CMU_ERR_INVALID_CMD;

    CMU_ResetSendCtrlData(pRcvCtrlData->ulRevID.ulFrameID,NULL,0);
    pSendBuf = CMU_GetSendBufAddr();


    switch(pRcvCtrlData->ulRevID.ulADDADataID.ulCmdIndex)
    {
    case ADDA_GETCMD_HDATA://获取高速DA输出状态
        {
            ulRet = m_sExternalFunTable.pf_MDAC_GetHSpdDaData(pRcvCtrlData->ulRevID.ulADDADataID.ulChMask, (uBit32*)pSendBuf);
            
            if (ulRet == 0)
            {
                CMU_AddToSendCtrlData(NULL,sizeof(uBit32));
            }else
            {
                ulRet = CMU_ERR_DATA_WRONG;
            }

            break;
        }
    default:break;
    }

    return ulRet;
}


 /*
函数名称:uBit32 Adda_CmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData)

功    能:ADDA指令处理线程,如果设置指令执行完成并成功则回包,指令类型包括
            #define SETCMD_TYPE_NOMAL       (0)         //常规设置指令
            #define SETCMD_TYPE_INQ         (1)         //周期查询设置指令
            #define GETCMD_TYPE_NOMAL       (0)         //常规获取指令
            #define GETCMD_TYPE_INQ         (1)         //周期新数据获取指令

参    数:pRcvCtrlData--接收控制数据区

返 回 值:0--成功
          非0--错误码
注意事项:数据块接收完成后调用
*/ 
uBit32 Adda_CmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData)
{
    uBit32 ulRet = CMU_ERR_INVALID_CMD;

    if (pRcvCtrlData->ulRevID.ulADDADataID.ulOperType==DATA_OPRE_TYPE_SET)//设置数据
    {
        switch(pRcvCtrlData->ulRevID.ulADDADataID.ulCmdType)
        {
        case SETCMD_TYPE_NOMAL://常规设置指令,ulChMask表示DA通道编号
            {
                ulRet = Adda_NomalSetCmdProcess(pRcvCtrlData);
                break;
            }
        case SETCMD_TYPE_INQ://周期性设置指令
            {
                ulRet = CMU_AddInqRecord(pRcvCtrlData->ulRevID, *(uBit32*)pRcvCtrlData->pRevBuf);
                break;
            }
        default:break;
        }

        if (ulRet==CMU_ERR_SUCCESS)//
        {
            if(pRcvCtrlData->ulWaitResultCode==0) //无需等待处理结果
                CMU_SendResponsePack(pRcvCtrlData->ulRevID.ulFrameID, CMU_ERR_SUCCESS);
        }
    }else//获取周期性状态数据
    {
        switch(pRcvCtrlData->ulRevID.ulADDADataID.ulCmdType)
        {
        case GETCMD_TYPE_NOMAL:
            ulRet = Adda_NomalGetCmdProcess(pRcvCtrlData);
            break;
        case GETCMD_TYPE_INQ:
            ulRet = CMU_SendPeriodDataProc(pRcvCtrlData->ulRevID);
            break;
        default:break;
        }
    }

    return ulRet;
}


uBit32 Reg_CmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData)
{
    uBit32 ulRet = CMU_ERR_INVALID_CMD;
    uBit8* pSendBuf;

    if (pRcvCtrlData->ulRevID.ulComDataID.ulOperType==DATA_OPRE_TYPE_SET)//设置数据
    {
        if (pRcvCtrlData->ulRevID.ulComDataID.ulCmdType != SETCMD_TYPE_NOMAL ||\
            pRcvCtrlData->ulRevID.ulRegDataID.ulCmdIndex != REG_SETCMD_DATA)
        {
            return ulRet;
        }

        ulRet = m_sExternalFunTable.pf_UREG_SetRegData(pRcvCtrlData->ulRevID.ulRegDataID.ulRegIndex,\
                                                                    (uBit32*)pRcvCtrlData->pRevBuf,\
                                                                    pRcvCtrlData->ulRevID.ulRegDataID.ulRegCount);

        if (ulRet==CMU_ERR_SUCCESS)//
        {
            if(pRcvCtrlData->ulWaitResultCode==0) //无需等待处理结果
                CMU_SendResponsePack(pRcvCtrlData->ulRevID.ulFrameID, CMU_ERR_SUCCESS);
        }
    }else//获取数据
    {
        if (pRcvCtrlData->ulRevID.ulComDataID.ulCmdType != GETCMD_TYPE_NOMAL ||\
            pRcvCtrlData->ulRevID.ulRegDataID.ulCmdIndex != REG_GETCMD_DATA)
        {
            return CMU_ERR_INVALID_CMD;
        }

        CMU_ResetSendCtrlData(pRcvCtrlData->ulRevID.ulFrameID, NULL,0);

        if (CMU_CheckSendBufFreeSize(pRcvCtrlData->ulRevID.ulRegDataID.ulRegCount*sizeof(uBit32))==false)
            return CMU_ERR_SEND_OVERFLOW;


        pSendBuf = CMU_GetSendBufAddr();
        ulRet = m_sExternalFunTable.pf_UREG_GetRegData(pRcvCtrlData->ulRevID.ulRegDataID.ulRegIndex,\
                                                        (uBit32*)pSendBuf, pRcvCtrlData->ulRevID.ulRegDataID.ulRegCount);

        if (ulRet==CMU_ERR_SUCCESS)
            CMU_AddToSendCtrlData(NULL,pRcvCtrlData->ulRevID.ulRegDataID.ulRegCount*sizeof(uBit32));
    }

    return ulRet;
}

uBit32 Servo_CmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData)
{
  return CMU_ERR_SUCCESS;
}



 /*
函数名称:uBit32 Custom_NomalSetCmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData)

功    能:IO常规设置指令处理线程,处理的指令包括
        #define IO_SETCMD_STATE                 (1)//设置IO板输出状态
        #define IO_SETCMD_PWM_ENABLE            (2)//设置IO板PWM功能
        #define IO_SETCMD_PWM_DUTY_RATIO        (3)//设置IO板IO口PWM占空比
        #define IO_SETCMD_PWM_FRQ               (4)//设置IO板IO口PWM频率
参    数:pRcvCtrlData--接收控制数据区

返 回 值:0--成功
          非0--错误码
注意事项:
*/ 
uBit32 Custom_NomalSetCmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData)
{
    uBit32 ulRow = 0;
    uBit32 ulCol = 0;
    uBit32 ulRet = CMU_ERR_INVALID_CMD;

    memcpy(&ulRow, pRcvCtrlData->pRevBuf, sizeof(uBit32));
    memcpy(&ulCol, pRcvCtrlData->pRevBuf+sizeof(uBit32),sizeof(uBit32));
    
    switch(pRcvCtrlData->ulRevID.ulComDataID.ulCmdIndex)
    {
    case VM_SETCMD_AISLE_MOTOR:                //设置IO板PWM功能,ulBitMask为IO引脚编号
        {
            ulRet = m_sExternalFunTable.pf_VM_EnabletAisleMotor(ulRow, ulCol);
            break;
        }
    default:break;
    }

    return ulRet;
}



 /*
函数名称:uBit32 Custom_NomalGetCmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData);

功    能:IO常规获取指令处理线程,如果获取成功则配置回包,指令包括
                #define IO_GETCMD_HSPD_OUT_STATUS   (1)//获取高速IO输出口状态
                #define IO_GETCMD_HSPD_IN_STATUS    (2)//获取高速IO输入口状态
参    数:pRcvCtrlData--接收控制数据区

返 回 值:0--成功
          非0--错误码
注意事项:IO指令处理线程调用
*/ 
uBit32 Custom_NomalGetCmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData)
{
    uBit8 *pSendBuf;
    uBit32  ulRet = CMU_ERR_INVALID_CMD;

    CMU_ResetSendCtrlData(pRcvCtrlData->ulRevID.ulFrameID,NULL,0);
    pSendBuf = CMU_GetSendBufAddr();

    switch(pRcvCtrlData->ulRevID.ulComDataID.ulCmdIndex)
    {
    case VM_GETCMD_AISLE_MOTOR_STATUS: //获取货道电机完成状态
        {
            *((uBit32*)pSendBuf) = m_sExternalFunTable.pf_VM_GetAisleMotorRunningState();
            ulRet = CMU_ERR_SUCCESS;
            CMU_AddToSendCtrlData(NULL,sizeof(uBit32));
            break;
        }
        
    default:break;
    }
    
    return ulRet;
}



 /*
函数名称:uBit32 Io_CmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData)

功    能:IO指令处理线程,如果设置指令执行完成并成功则回包,指令类型包括
            #define SETCMD_TYPE_NOMAL       (0)         //常规设置指令
            #define SETCMD_TYPE_INQ         (1)         //周期查询设置指令
            #define GETCMD_TYPE_NOMAL       (0)         //常规获取指令
            #define GETCMD_TYPE_INQ         (1)         //周期新数据获取指令

参    数:pRcvCtrlData--接收控制数据区

返 回 值:0--成功
          非0--错误码
注意事项:数据块接收完成后调用
*/ 
uBit32 Custom_CmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData)
{
    uBit32 ulRet = CMU_ERR_INVALID_CMD;

    if (pRcvCtrlData->ulRevID.ulComDataID.ulOperType==DATA_OPRE_TYPE_SET)//设置数据
    {
        switch(pRcvCtrlData->ulRevID.ulComDataID.ulCmdType)
        {
        case SETCMD_TYPE_NOMAL:    //常规设置指令
            ulRet = Custom_NomalSetCmdProcess(pRcvCtrlData);
            break;
        default:break;
        }

        if (ulRet==CMU_ERR_SUCCESS)//
        {
            if(pRcvCtrlData->ulWaitResultCode==0) //无需等待处理结果
                CMU_SendResponsePack(pRcvCtrlData->ulRevID.ulFrameID, CMU_ERR_SUCCESS);
        }
    }else//获取数据
    {
        switch(pRcvCtrlData->ulRevID.ulComDataID.ulCmdType)
        {
        case GETCMD_TYPE_NOMAL://获取常规数据
            ulRet = Custom_NomalGetCmdProcess(pRcvCtrlData);
            break;
        default:break;
        }
    }

    return ulRet;
}


 /*
函数名称:uBit32 CMU_CmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData);

功    能:数据块处理线程,如果设置指令处理完成并成功则发生成功回包,如果获取指令成功则配置回包,
          处理发送错误由上层处理报错

参    数:pRcvCtrlData--接收控制数据区

返 回 值:0--成功
          非0--错误码
注意事项:数据块接收完成后调用
*/ 
uBit32 CMU_CmdProcess(COM_RCV_CTRL_DATA *pRcvCtrlData)
{
    uBit32 ulRet = CMU_ERR_INVALID_CMD;

    //重发数据块处理（只有设置指令的重发数据包且上次执行成功才处理）
    if (pRcvCtrlData->ulRevID.ulComDataID.ulSendRetry && 
        pRcvCtrlData->ulRevID.ulComDataID.ulOperType==DATA_OPRE_TYPE_SET)
    {
        if (((m_sRcvTimeOutCtrlData.ulRevID.ulFrameID&SAME_TYPE_PACK_MASK)==(pRcvCtrlData->ulRevID.ulFrameID&SAME_TYPE_PACK_MASK)) &&
            //(pRcvCtrlData->ulRevID.ulComDataID.ulSendRetry==m_sRcvTimeOutCtrlData.ulRevID.ulComDataID.ulSendRetry-1) &&
            (m_sRcvTimeOutCtrlData.cCheckNum== pRcvCtrlData->cCheckNum) &&
            m_sRcvTimeOutCtrlData.ulExeResult==CMU_ERR_SUCCESS)
        {
            //m_sRcvTimeOutCtrlData.ulRevID = pRcvCtrlData->ulRevID;
            CMU_SendResponsePack(pRcvCtrlData->ulRevID.ulFrameID, CMU_ERR_SUCCESS);
            return CMU_ERR_SUCCESS;
        }
    }

    switch(pRcvCtrlData->ulRevID.ulComDataID.ulDataType)//数据类型
    {
    case DATA_TYPE_SYS:
        ulRet = Sys_CmdProcess(pRcvCtrlData);
        break;
    case DATA_TYPE_CRD:
        ulRet = Crd_CmdProcess(pRcvCtrlData);
        break;
    case DATA_TYPE_AXIS:
        ulRet = Motor_CmdProcess(pRcvCtrlData);
        break;
    case DATA_TYPE_IO:
        ulRet = Io_CmdProcess(pRcvCtrlData);
        break;
    case DATA_TYPE_ADDA:
        ulRet = Adda_CmdProcess(pRcvCtrlData);
        break;
    case DATA_TYPE_REG:
        ulRet = Reg_CmdProcess(pRcvCtrlData);
        break;
    case DATA_TYPE_SERVO:
        ulRet = Servo_CmdProcess(pRcvCtrlData);
        break;
    case DATA_TYPE_CUSTOM:      //2017.12.13 Duanfeng  水果机接口 新增
        ulRet = Custom_CmdProcess(pRcvCtrlData);
        break;
    default: break;
    }


    if (pRcvCtrlData->ulRevID.ulComDataID.ulOperType==DATA_OPRE_TYPE_SET)
    {
        m_sRcvTimeOutCtrlData.cCheckNum = pRcvCtrlData->cCheckNum;
        m_sRcvTimeOutCtrlData.ulRevID = pRcvCtrlData->ulRevID;
        m_sRcvTimeOutCtrlData.ulExeResult = ulRet;
    }

    return ulRet;
}
