#include <string.h>
#include "BRD_AxisDev.h"
#include "CNC/DVM/DEV_COM_CmdDef.h"
#include "CNC/ErrorCodeDef/DVM_ErrorCodeDef.h"
#include "MotorCtrl/MotorCtrl.h"



static AXIS_PARM  m_brd_AxisParm;              //轴控参数

//板载轴设备初始化，总线初始化时调用
void BRD_AxisInit(void)
{
    //PWM_Init();   //PWM模块初始化

    m_brd_AxisParm.AxisScaleParm.nType = 1;
    m_brd_AxisParm.AxisScaleParm.lGearRatioPosScale = 1;        
    m_brd_AxisParm.AxisScaleParm.lGearRatioPlusScale = 1;    
    m_brd_AxisParm.AxisScaleParm.lInternalPulseScale = 1;         
    m_brd_AxisParm.AxisScaleParm.lQEIPulseScale = 1;            
    m_brd_AxisParm.AxisScaleParm.lCycleQeiPulseCount = 10000;            

    m_brd_AxisParm.AxisMoveParm.dMaxJogSpeed = 1000;    
    m_brd_AxisParm.AxisMoveParm.dMaxJogAcc = 0.05;        
    m_brd_AxisParm.AxisMoveParm.dJogJerk = 0.01;        
    m_brd_AxisParm.AxisMoveParm.dMaxFeedSpeed = 1000;    
    m_brd_AxisParm.AxisMoveParm.dMaxFeedAcc = 5;    
    m_brd_AxisParm.AxisMoveParm.dFeedJerk = 0.1;        
    m_brd_AxisParm.AxisMoveParm.dRapidSpeed = 1000;    
    m_brd_AxisParm.AxisMoveParm.dRapidAcc = 5;        
    m_brd_AxisParm.AxisMoveParm.dRapidJerk = 0.1;        

    m_brd_AxisParm.AxisHomeParm.dHomeRapidSpeed = 10;
    m_brd_AxisParm.AxisHomeParm.dHomeApproachSpeed = 10;
    m_brd_AxisParm.AxisHomeParm.dHomeLocatSpeed = 5;
    m_brd_AxisParm.AxisHomeParm.dHomeIndexSpeed = 1;
    //
    m_brd_AxisParm.AxisSignalConfig.SvAlarmSignal = 1;
    m_brd_AxisParm.AxisSignalConfig.SvArrivSignal = 1;
    m_brd_AxisParm.AxisSignalConfig.QeiIndexSignal = 1;
    m_brd_AxisParm.AxisSignalConfig.SvReadySignal = 1;
    m_brd_AxisParm.AxisSignalConfig.SvEnableSignal = 1;
    m_brd_AxisParm.AxisSignalConfig.HardLimNSignal = 1;
    m_brd_AxisParm.AxisSignalConfig.HardLimPSignal = 1;
    m_brd_AxisParm.AxisSignalConfig.HomeSignal = 1;
    m_brd_AxisParm.AxisSignalConfig.QeiEnable = 1;
    m_brd_AxisParm.AxisSignalConfig.SvAlarmEnable = 1;
    m_brd_AxisParm.AxisSignalConfig.SvArriveEnable = 1;
    m_brd_AxisParm.AxisSignalConfig.SvReadyEnable = 1;

    //轴状态初始化
    //memset(&m_brd_AxisState,0,sizeof(m_brd_AxisState));
    //m_brd_AxisState.ulRunningStatus = AXIS_STATUS_ZERO_SPEED|AXIS_STATUS_FSTOP|AXIS_STATUS_ENABLE;  
}


//获取轴状态数据
void BRD_AxisGetAxisState(AXIS_STATE_DATA *pAxisStateData)
{
    double dQeiSpeed;
    
    //刷新轴实际位置
#ifdef BRD_ENABLE_QEI
    pAxisStateData->lQeiPos = HQEI_GetHSpdQeiPos()+m_brd_QeiGap;
    
    //获取实际速度
    dQeiSpeed = (double)m_brd_AxisParm.AxisScaleParm.lGearRatioPosScale/(double)m_brd_AxisParm.AxisScaleParm.lGearRatioPlusScale;
    pAxisStateData->dQeiSpeed = dQeiSpeed*HQEI_GetHSpdQeiSpeed();   
    
    if(LPC_GPIO_PORT->PIN[BRD_AXIS_ENEBLE_PORT]&(0x01<< BRD_AXIS_ENEBLE_PIN))//使能
        pAxisStateData->ulRunningStatus &= ~AXIS_STATUS_ENABLE;   
    else
        pAxisStateData->ulRunningStatus |= AXIS_STATUS_ENABLE;   
    
    if(LPC_GPIO_PORT->PIN[BRD_AXIS_ALARM_PORT]&(0x01<< BRD_AXIS_ALARM_PIN))//报警
        pAxisStateData->ulRunningStatus &= ~AXIS_STATUS_SV_ALARM;   
    else
        pAxisStateData->ulRunningStatus |= AXIS_STATUS_SV_ALARM;   
#else
    pAxisStateData->lQeiPos = PWM_GetActPulse();
    
    //获取实际速度
    dQeiSpeed = (double)m_brd_AxisParm.AxisScaleParm.lGearRatioPosScale/(double)m_brd_AxisParm.AxisScaleParm.lGearRatioPlusScale;
    pAxisStateData->dQeiSpeed = dQeiSpeed*PWM_GetPulseSpeed(); 
    
    //刷新轴运动状态
    pAxisStateData->ulRunningStatus |= AXIS_STATUS_ENABLE;
#endif
    
    if(PWM_CheckBusy())
    {
        pAxisStateData->ulRunningStatus &= ~(AXIS_STATUS_ZERO_SPEED);//|AXIS_STATUS_FSTOP
    }else
    {
        pAxisStateData->ulRunningStatus |= (AXIS_STATUS_ZERO_SPEED);//|AXIS_STATUS_FSTOP     
    }
}

//设置轴控参数
void BRD_AxisSetAxisParm(const AXIS_PARM *pAxisParm)
{
   m_brd_AxisParm = *pAxisParm;
}

//设置系统控制参数
uBit32 BRD_AxisSetSysCtrlParm(const SYS_CTRL_PARM *pSysCtrlParm)
{
    if(PWM_UpdateTimeCycle(pSysCtrlParm->nTimeCycle))
    {
        return DVM_ERR_BUS_BUSY;
    }
    
    return DVM_ERR_SUCCESS;    
}

//设置进给
uBit32 BRD_AxisSetFeed(Bit32 lPulseInc)
{
    if(PWM_SendFeed(lPulseInc))
    {
        return DVM_ERR_PLUSE_BUFFER_FULL;
    }
    
    return DVM_ERR_SUCCESS;
}

//轴指令处理
uBit32 BRD_AxisSetCmd(uBit32 ulCmdType, Bit32 lCmdPara1, Bit32 lCmdPara2)
{
    uBit32 ulRet = DVM_ERR_INVALID_CMD;
    
    switch(ulCmdType)
    {
    case DEV_CMD_ENABLE_SV:         //使能电机
        {
#ifdef BRD_ENABLE_QEI
            if(lCmdPara1)//使能
                LPC_GPIO_PORT->CLR[BRD_AXIS_ENEBLE_PORT] |= (0x01 << BRD_AXIS_ENEBLE_PIN);   //
            else
                LPC_GPIO_PORT->SET[BRD_AXIS_ENEBLE_PORT] |= (0x01 << BRD_AXIS_ENEBLE_PIN);   //
            ulRet = DVM_ERR_SUCCESS;
            break;
#endif
        }
    case  DEV_CMD_RESET:            //系统复位
    case  DEV_CMD_SET_HOME:         //回零
    case  DEV_CMD_SET_VERIFY_MODE:  //设置进入校验模式
        return DVM_ERR_SUCCESS;     
    case  DVM_CMD_SET_AXIS_CMDPOS:  //设置轴当前位置
        {
            PWM_SetCmdPulse(lCmdPara1);
            ulRet = DVM_ERR_SUCCESS;
            break;
        }
    case  DEV_CMD_SET_QEI_POS:      //设置编码器位置
        {
#ifdef BRD_ENABLE_QEI
            m_brd_QeiGap = lCmdPara1-HQEI_GetHSpdQeiPos();
#else
            PWM_SetActPulse(lCmdPara1);        
#endif
            ulRet = DVM_ERR_SUCCESS;
            break;        
        }
    case  DEV_CMD_SET_OUTPUT:       //设置IO输出
    case  DEV_CMD_SET_IO_PWM_MODE:  //设置IO进入PWM模式
    case  DEV_CMD_SET_IO_PWM_DUTY:  //设置IO进入PWM占空比
    case  DEV_CMD_SET_IO_PWM_FREQ:  //设置IO进入PWM频率
        break;
        
    default:break;
    }
    
    return ulRet;
}

