/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：SYS_Init.c

摘    要：系统初始化

运行环境：LPC43xx 

修改描述：

当前版本：1.0

修改作者：毛军

修改日期：2015年10月16日

使用说明：

*/

#include <string.h>

#include "CNCSYS_Interface.h"
#include "CNC/ErrorCodeMan/ECM_ErrorCodeMan.h"
#include "CNC/ErrorCodeDef/IPR_ErrorCodeDef.h"
#include "CNC/AHB/AHB_Init.h"
#include "CNC/MemoryAddr/SDRAM_AddrDef.h"
#include "CNC/MemoryAddr/UREG_Interface.h"
#include "CNC/DVM/DEV_Interface.h"
#include "CNC/CSM/CSM_Interface.h"
#include "CNC/MST/MST_Interface.h"
#include "CNC/SPM/SPM_SysCtrlParm.h"
#include "CNC/IPO/IPO_Interface.h"

#include "CNC/CNCSYS/CNCSYS_Interface.h"



#if 0

#include "LPC43xx.h"
#include "lpc43xx_cgu.h"
#include "lpc43xx_gpio.h"
#include "lpc43xx_scu.h"
#include "lpc43xx_timer.h"

#include "CNCSYS_Interface.h"

#include "../CMU/UIN_GlobalDataDef.h"
#include "../IPO/IPO_M0/IPO_Interface.h"
#include "../DatastructDef/SYS_ConfigDef.h"


#include "../../SysPeripheral/FlashRW/FlashAddrDef.h"
#include "../../SysPeripheral/FlashRW/lpc43xx_flash.h"
#include "../../SysPeripheral/SystemTick/SystemTick.h"


#include "../../SysPeripheral/CAN/CanInterface.h"
#include "../../SysPeripheral/Enet/EnetInterface.h"
#endif


#if ENABLE_CAN_BUS
#include "CNC/DVM/CAN_Device/CAN_DEV_Interface.h"
#include "CNC/DVM/CAN_Device/CAN_DEV_DataStructDef.h"
#define DEV_CAN_NODE_MASK   (0X01)
#endif

#if ENABLE_MIII_BUS
#include "CNC/DVM/MIII_Device/MIII_DEV_Interface.h"
#include "CNC/DVM/MIII_Device/MIII_DEV_DataStructDef.h"
#endif

//


static uBit8 m_uIpoInitFlag = 0;            //IPO初始化标志，只有IPO已经初始化了，timer0中断才可以执行IPO主函数
static uBit8 m_uEnableIPOInTime0 = 1;       //插补中断使用TIMER0标志，如果挂接了MIII总线驱动，则使用MIII中断作为插补中断




//////////////
CNCSYS_INTERFACE  m_CNCSYSInterface;

///////////////

int CNCSYS_InitInterface(CNCSYS_INTERFACE* pCNCSYSInterface)
{  
    m_CNCSYSInterface = *pCNCSYSInterface;
    
    return 0;
}




//MIII总线存在时，挂接到MIII中断模块，由MIII中断调用执行
void SYS_IPOInMetro(void)
{
    //升级主控bootloader时禁止插补中断
    if ((SYS_GetSysStatus() & SYS_STATUS_IPO_CLOSE_MASK)==0)
    {
        IPO_MainCtrl();       
    }
    else
    {
        DEV_MIIISynCycleService();
    }
}

//TIMER0中断服务函数中调用
void SYS_IPOInTime0(void)
{
    if (m_uEnableIPOInTime0)
    {
        //升级主控bootloader时禁止插补中断
        if ((SYS_GetSysStatus() & SYS_STATUS_IPO_CLOSE_MASK)==0)
        {
            if(m_uIpoInitFlag)
                IPO_MainCtrl();
        }
    }
}


void SYS_EnableIPOInTime0(uBit8 uEnable)
{
    m_uEnableIPOInTime0 = uEnable;
}


//在SysTick中断中运行插补器
void CNCSYS_IPOInSysTick(void)
{
    if(m_uIpoInitFlag == 0)
    {
        return ;
    }

    //升级主控bootloader时禁止插补中断
    if ((SYS_GetSysStatus() & SYS_STATUS_IPO_CLOSE_MASK)==0)
    {
        IPO_MainCtrl();
    }

}

//更新插补器插补中断周期
uBit32 SYS_UpdateIPOTimeCycle(uBit32 ulTimeCycleUs)
{

    
    return 0;
}


/*
函 数 名：int SYS_InitDevDrv()
功    能：初始化设备驱动
参    数：无
返 回 值：成功返回0，否则返回非0值
调用位置：
注意事项：
*/
int SYS_InitDevDrv()
{
    int iError = 0;

    BUS_DEVICE_DRIVE BusDevDrv;
    memset(&BusDevDrv, 0, sizeof(BusDevDrv));

#if ENABLE_CAN_BUS
    CAN_INTERFACE CanInterface;
    CAN_DEV_EX_INTERFACE CanDevExInterface;
#endif

#if ENABLE_MIII_BUS
    MIII_DEV_EX_INTERFACE MIIIDevExInterface;
#endif


#if ENABLE_CAN_BUS
    //初始化CAN设备驱动------------------
    memset(&CanInterface, 0, sizeof(CanInterface));
    memset(&CanDevExInterface, 0, sizeof(CanDevExInterface));
    CAN_ExportInterface(&CanInterface);  
    CAN_DEV_SetComInterface(CanInterface);

    CanDevExInterface.pf_SYS_GetSysTickCount = SYS_GetSysTickCount;
    CAN_DEV_SetExternalApi(CanDevExInterface);

    //注册CAN设备驱动到设备管理层
    CAN_DEV_GetBusDriver(&BusDevDrv);

    if (DEV_DrvReg(DEV_BUS_TYPE_CAN, DEV_CAN_NODE_MASK, &BusDevDrv))
    {
        return 1;
    }
#endif

#if ENABLE_MIII_BUS
    //初始化MIII设备驱动---------------------
    memset(&MIIIDevExInterface, 0, sizeof(MIIIDevExInterface));
    MIIIDevExInterface.pf_SYS_GetSysTickCount = SYS_GetSysTickCount;
    MIII_DEV_SetExternalApi(MIIIDevExInterface);

    //注册MIII设备驱动到设备管理层
    MIII_DEV_GetBusDriver(&BusDevDrv);

    if(DEV_DrvReg(DEV_BUS_TYPE_MECHALINK_III, 0, &BusDevDrv))
    {
        return 1;
    }
#endif

    //设备管理层设备总线初始化---------------
    if (DEV_BusInit())
    {
        iError = 1;
        //return 1;
    }

    //配置设备管理层系统控制参数访问地址
    DEV_SetSysCtrlParmReadAddr(SPM_GetSysCtrlParmAddr());
    
    //配置设备管理层轴参数访问地址
    DEV_SetAxisParmReadAddr(SPM_GetAxisParmAddr());
  
    return iError;
}


/*
函 数 名：int SYS_InitIPO()
功    能：初始化插补器
参    数：无
返 回 值：成功返回0，否则返回非0值
调用位置：
注意事项：
*/
int SYS_InitIPO()
{
    IPO_INTERFACE IPOInterface;

    //设置系统控制参数读取地址
    IPO_SetSysParmReadAddr(SPM_GetSysCtrlParmAddr(), SPM_GetCrdSysAxisMapTableAddr(), SPM_GetCrdSysParmAddr(), SPM_GetAxisParmAddr());
    

    //设置插补缓冲区地址
    IPO_SetIPOBufAddr(IPO_BUF_START_ADDR, IPO_BUF_TOTAL_LEN);

    //设置快速速度规划表存储地址
    IPO_SetRapidSpeedCtrlTableAddr(RAPID_SPEED_CTRL_TABLE_BUF_ADDR, RAPID_SPEED_CTRL_TABLE_BUF_LEN);

    //设置手动位置控制速度规划表存储地址
    IPO_SetJogPosCtrlTableAddr(JOG_POSCTRL_TABLE_BUF_START_ADDR, JOG_POSCTRL_TABLE_BUF_TOTAL_LEN);


    //设置插补器AHB数据交换起始地址
    IPO_SetAHBDataTransAddr((uBit32)&g_pAHBDataStruct->IPO_M0_M4_DataTrans);

    //初始化插补器外部依赖接口
    memset(&IPOInterface, 0, sizeof(IPOInterface));
    IPOInterface.pf_DEV_SynCycleService = DEV_SynCycleService;
    IPOInterface.pf_PAX_SendFeed = PAX_SendFeed;
    IPOInterface.pf_PAX_GetStateData = PAX_GetStateData;
    IPOInterface.pf_PAX_GetRegCount = PAX_GetRegCount;
    IPOInterface.pf_PAX_LockForIPO = PAX_LockForIPO;

    IPOInterface.pf_SYS_GetAlarmStatus = SYS_GetAlarmStatus;
    IPOInterface.pf_SYS_UpdateIPOTimeCycle = SYS_UpdateIPOTimeCycle;

    IPOInterface.pf_ECM_GenerateErrorCode = ECM_GenerateErrorCode;
    IPOInterface.pf_ECM_PutErrorToBuf = ECM_PutErrorToBuf;
    
    IPOInterface.pf_SYS_UpdateIPOTimeCycle = SYS_UpdateIPOTimeCycle;
    
#if 0
    IPOInterface.pf_IPOTick_TimingStart = IPOTick_TimingStart;
    IPOInterface.pf_IPOTick_GetTimePassed = IPOTick_GetTimePassed;
   
    ////////////////
    //测试用
    IPOInterface.pf_SysTick_CountSysTickStart = SysTick_CountSysTickStart;
    IPOInterface.pf_SysTick_CountSysTickEnd = SysTick_CountSysTickEnd;
    IPOInterface.pf_SysTick_GetCycleTime = SysTick_GetCycleTime;
    ////////////////
#endif
    
    if (IPO_InitInterface(&IPOInterface))
    {
        return 1;
    }

    IPO_InitData();

    //
    if (!SYS_GetAlarmBit(SYS_ALARM_SDRAM_INIT_ERROR))
    { //SDRAM检验正确
        IPO_CreateRapidSpeedCtrlTable();
        IPO_CreateJogPosCtrlTable();
    }

    //开启插补中断（在外部统一开启）
    m_uIpoInitFlag = 1;
    
    return 0;
}



/*
函 数 名：int MST()
功    能：初始化MST模块
参    数：无
返 回 值：成功返回0，否则返回非0值
调用位置：
注意事项：
*/
int SYS_InitMST()
{
    MST_INTERFACE MSTInterface;

    //
    memset(&MSTInterface, 0, sizeof(MSTInterface));

    MSTInterface.pf_CSM_ProgStop = CSM_ProgStop;
    MSTInterface.pf_IO_SetOutputStatus = IO_SetOutputStatus;
    MSTInterface.pf_ECM_GenerateErrorCode = ECM_GenerateErrorCode;
    MSTInterface.pf_ECM_PutErrorToBuf = ECM_PutErrorToBuf;
    MSTInterface.pf_SYS_GetSysTickCount = SYS_GetSysTickCount;

    if (MST_InitInterface(&MSTInterface))
    {
        return 1;
    }

    //
    MST_Init();

    return 0;
}



/*
函 数 名：int SYS_InitCSM()
功    能：初始化通道管理模块
参    数：无
返 回 值：成功返回0，否则返回非0值
调用位置：
注意事项：
*/
int SYS_InitCSM()
{
    CSM_INTERFACE CSMInterface;

    //设置系统控制参数读取地址
    CSM_SetSysParmReadAddr(SPM_GetSysCtrlParmAddr(), SPM_GetCrdSysAxisMapTableAddr(), SPM_GetCrdSysParmAddr(), SPM_GetAxisParmAddr());

    //设置程序缓冲区地址
    CSM_SetProgBufAddr(PROG_BUF_START_ADDR, PROG_BUF_TOTAL_LEN);

    //设置用户自定义数据缓冲区地址
    CSM_SetUserBufAddr(USER_BUF_START_ADDR, USER_BUF_TOTAL_LEN);

    //
    memset(&CSMInterface, 0, sizeof(CSMInterface));

    CSMInterface.pf_PAX_GetStateData = PAX_GetStateData;
    CSMInterface.pf_PAX_GetRunningStatus = PAX_GetRunningStatus;
    CSMInterface.pf_PAX_GetRegCount = PAX_GetRegCount;
    CSMInterface.pf_PAX_Enable = PAX_Enable;
    CSMInterface.pf_PAX_Home = PAX_Home;
    CSMInterface.pf_PAX_SetCmdPos = PAX_SetCmdPos;
    CSMInterface.pf_PAX_SetQeiPos = PAX_SetQeiPos;
    CSMInterface.pf_DEV_CheckLastCmdExeState = DEV_CheckLastCmdExeState;

    
    CSMInterface.pf_IPO_Reset = IPO_Reset;
    CSMInterface.pf_IPO_SetCtrlMode = IPO_SetCtrlMode;
    CSMInterface.pf_IPO_GetStateData = IPO_GetStateData;
    CSMInterface.pf_IPO_GetBufState = IPO_GetBufState;

    CSMInterface.pf_IPO_SetCrdSysFeedOverride = IPO_SetCrdSysFeedOverride;
    CSMInterface.pf_IPO_SetCrdSysRapidOverride = IPO_SetCrdSysRapidOverride;
    CSMInterface.pf_IPO_SetVerifyEnable = IPO_SetVerifyEnable;
       CSMInterface.pf_IPO_EnableSingleSegmentMode = IPO_EnableSingleSegmentMode;   
    CSMInterface.pf_IPO_ProgRun = IPO_ProgRun;
    CSMInterface.pf_IPO_ProgStop = IPO_ProgStop;
    CSMInterface.pf_IPO_ProgEStop = IPO_ProgEStop;
    CSMInterface.pf_IPO_ProgSegmentEndStop = IPO_ProgSegmentEndStop;
    CSMInterface.pf_IPO_ProgHold = IPO_ProgHold;
    CSMInterface.pf_IPO_AddMotionDataToIPOBuf = IPO_AddMotionDataToIPOBuf;
    CSMInterface.pf_IPO_CloseAxisOutput = IPO_CloseAxisOutput;

    CSMInterface.pf_JOG_SetAxisPosCtrlMotion = JOG_SetAxisPosCtrlMotion;
    CSMInterface.pf_JOG_SetAxisSpeedCtrlMotion = JOG_SetAxisSpeedCtrlMotion;
    CSMInterface.pf_JOG_SetMotorPosCtrlMotion = JOG_SetMotorPosCtrlMotion;
    CSMInterface.pf_JOG_SetMotorSpeedCtrlMotion = JOG_SetMotorSpeedCtrlMotion;
    CSMInterface.pf_JOG_SetMotionStop = JOG_SetMotionStop;
    CSMInterface.pf_JOG_MotorEStop = JOG_MotorEStop;

    CSMInterface.pf_MST_Reset = MST_Reset;
    CSMInterface.pf_MST_Run = MST_Run;
    CSMInterface.pf_MST_Stop = MST_Stop;
    CSMInterface.pf_MST_Cancel = MST_Cancel;
    CSMInterface.pf_MST_ProcessBeforeActionMST =MST_ProcessBeforeActionMST;
    CSMInterface.pf_MST_ProcessAfterActionMST = MST_ProcessAfterActionMST;
    CSMInterface.pf_MST_CheckBeforeActionMSTExist = MST_CheckBeforeActionMSTExist;
    CSMInterface.pf_MST_CheckAfterActionMSTExist = MST_CheckAfterActionMSTExist;
    CSMInterface.pf_MST_GetStateData = MST_GetStateData;

    CSMInterface.pf_SYS_GetAlarmStatus = SYS_GetAlarmStatus;

    CSMInterface.pf_ECM_GenerateErrorCode = ECM_GenerateErrorCode;
    CSMInterface.pf_ECM_PutErrorToBuf = ECM_PutErrorToBuf;

    if (CSM_InitInterface(&CSMInterface))
    {
        return 1;
    }

    //
    CSM_InitData();

    return 0;
}





/*
函 数 名：int SYS_Init()
功    能：系统初始化
参    数：无
返 回 值：成功返回0，否则返回非0值
调用位置：系统上电时调用
注意事项：
*/
int CNCSYS_Init()
{
    //
    SYS_InitStateData();
    
    
    //初始化AHB
    if (AHB_Init())
    {
        SYS_SetAlarmBit(SYS_ALARM_AHB_INIT_ERROR);
    }

    //初始化SDRAM
    if (SDRAM_Init())
    {
        SYS_SetAlarmBit(SYS_ALARM_SDRAM_INIT_ERROR);
    }

    //初始化系统参数管理模块
    SPM_InitSysCtrlParm();

    //初始化设备驱动
    if (SYS_InitDevDrv())
    {
        SYS_SetAlarmBit(SYS_ALARM_DEV_DRV_INIT_ERROR);
    }

    //初始化插补器
    if (SYS_InitIPO())
    {
        SYS_SetAlarmBit(SYS_ALARM_IPO_INIT_ERROR);
    }


    //初始化通道管理模块
    if (SYS_InitCSM())
    {
        SYS_SetAlarmBit(SYS_ALARM_CSM_INIT_ERROR);
    }

    //初始化MST
    if (SYS_InitMST())
    {
        SYS_SetAlarmBit(SYS_ALARM_MST_INIT_ERROR);
    }

////////////////////////////////////////////////////
    
    /*
    /////////////////////////////////////
    //laomao  2016-05-13 修改  无论是否启用安川总线总断作为插补中断，都需要通知M4更新插补周期（M4需要重新设置计时周期）
    //通知M4重新配置TIME0（M4会重新修改自身的计时周期）
    if (IPO_UpdateSycCtrlParm())
    {
        SYS_SetAlarmBit(SYS_ALARM_UPDATE_SYSCTRL_PARM_FAIL);
    }
    */
    
    SYS_SetSysTickTimeCylce(SPM_GetSysTimeCycleUs());

    /////////////////////////////////////
    /*  //售货机版本中插补器挂在SysTick中断中运行
    //更新系统控制参数到插补器
    if (!DEV_CheckBusInitSuccess(DEV_BUS_TYPE_MECHALINK_III))
    {//未启动安川总线中断作为插补中断
        /////////////////////////////////////
        //laomao  2016-05-13 修改  无论是否启用安川总线总断作为插补中断，都需要通知M4更新插补周期（M4需要重新设置计时周期）
        //通知M4重新配置TIME0
        if (IPO_UpdateSycCtrlParm())
        {
            SYS_SetAlarmBit(SYS_ALARM_UPDATE_SYSCTRL_PARM_FAIL);
        }

        SYS_SetSysTickTimeCylce(SPM_GetSysTimeCycleUs());
        /////////////////////////////////////

        //挂接插补器到TIME0
        // SYS_EnableIPOInTime0(1);   //售货机版本中插补器挂在SysTick中断中运行
    }
    else
    {
        //挂接插补器到MIII
        SYS_EnableIPOInTime0(0);
        
#if ENABLE_MIII_BUS
        MIII_DEV_HockToMIII(SYS_IPOInMetro);
#endif
    }
    */
    
    
    //扫描硬件设备
    DEV_Scan();

    //加载系统配置
    if (!SPM_LoadConfig())
    { //加载系统配置成功
        //if (SPM_GetSysTimeCycleUs() != SYS_IPO_TIME_CYCLE_DEFAULT)//Alan??? 删除，不需要，在配置硬件配置信息时会同步更新系统控制参数
        //{
        //    SPM_SendSysCtrlParmToDev();
        //}

        //更新坐标系映射表到插补器
        if (IPO_UpdateCrdSysAxisMapTable())
        {
            SYS_SetAlarmBit(SYS_ALARM_LOAD_SYS_CONFIG_FAIL);
        }

        //更新通道管理模块坐标系映射表
        if (CSM_UpdateCrdSysAxisMapTable())
        {
            SYS_SetAlarmBit(SYS_ALARM_LOAD_SYS_CONFIG_FAIL);
        }
    }
    
////////////////////////////////////////////


    return 0;
}


/*
函 数 名：uBit32 SYS_SendIprError(Bit32 iCh, uBit32 ulErrNo)
功    能：解释器发送解释过程中错误
参    数：nCh        -- 通道号
          nErrNo    -- 解释进程错误编号
返 回 值：返回读取的数据长度
调用位置：
注意事项：
*/
uBit32 SYS_SendIprError(Bit32 iCh, uBit32 ulErrNo)
{
    return ECM_PutErrorToBuf(SYS_SOFT_MODULE_IPR, 
        iCh, 
        ulErrNo, 
        IPR_CMD_IPR_WORKING, 
        ERR_DEVICE_NO_INVALID, 
        ERR_LEVEL_NORMAL);
}



//
uBit32  CNCSYS_MainCtrl()
{
   uBit32 ulNonAxisAlarm;
  
    //
    CSM_MainCtrl();   


    LKH_LookaheadMainCtrl();


    DEV_AsynCycleService();
  
        //
    //非轴设备报警检测，如果有报警则停止坐标系运动(急停)
    ulNonAxisAlarm = DEV_CheckNonAxisDevAlarm();

    if (ulNonAxisAlarm==0)
    {
        SYS_ClrAlarmBit(SYS_ALARM_IO_OVER_CURRENT|SYS_ALARM_DEV_OFF_LINE|SYS_ALARM_DEV_OFF_LINE);
    }
    else
    {
        if (ulNonAxisAlarm&IO_STATUS_OVER_CURRENT)//IO模块过流报警
            SYS_SetAlarmBit(SYS_ALARM_IO_OVER_CURRENT);
        else
            SYS_ClrAlarmBit(SYS_ALARM_IO_OVER_CURRENT);

        if (ulNonAxisAlarm&IO_STATUS_OFF_LINE)//IO脱离总线
            SYS_SetAlarmBit(SYS_ALARM_DEV_OFF_LINE);
        else
            SYS_ClrAlarmBit(SYS_ALARM_DEV_OFF_LINE);

        if (ulNonAxisAlarm&IO_STATUS_SEND_CYCLE_DATA_FAIL) //发送周期数据失败
            SYS_SetAlarmBit(SYS_ALARM_DEV_SEND_CYCLE_DATA_FAIL);
        else
            SYS_ClrAlarmBit(SYS_ALARM_DEV_SEND_CYCLE_DATA_FAIL);

        /*
        //
        pSysCtrlParm = SPM_GetSysCtrlParmAddr();
        
        if(pSysCtrlParm->nCrdSysCount)
        {
              if(CSM_CheckEStop(0)==0)
                    IPO_ProgEStop((1<<pSysCtrlParm->nCrdSysCount)-1);
        }
        */
        CSM_AllChProgEStop();
    }
    
    return 0;
}