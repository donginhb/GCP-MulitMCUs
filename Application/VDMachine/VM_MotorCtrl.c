/**
  ******************************************************************************
  * @file    VM_MotorCtrl.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.12.05
  * @brief   VM Motor Ctrl Application
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "VM_MotorCtrl.h"
#include "VM_ErrorDef.h"
#include "VM_HardwareDef.h"
#include "DataType/DataType.h"
      
#include "CNC/DataStructDef/IPO_DataStructDefPub.h"
#include "CNC/DataStructDef/DEV_DataStructDef.h"
#include "CNC/CSM/CSM_Interface.h"
#include "CNC/CSM/CSM_CrdSysState.h"
#include "CNC/DVM/DEV_Interface.h"
#include "CNC/SPM/SPM_SysCtrlParm.h"

#include "SysPeripheral/SysTimer/SysTimer.h"
#include "SysPeripheral/GPIO/GPIO_Man.h"

#include <math.h>
#include <string.h>



/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

//系统报警字
#define VM_ALARM_SYS_ALARM			(0x00000001)		//系统报警位
#define VM_ALARM_CRD_ALARM			(0x00000002)		//通道报警位
#define VM_ALARM_AXIS_ALARM			(0x00000004)		//轴报警位
#define VM_ALARM_PO_ALARM			(0x00000008)		//系统掉电报警
#define VM_ALARM_CONF_INVALID		(0x10000000)		//系统配置故障
#define VM_ALARM_MOTOR_ENABLE       (0x20000000)        //电机使能失败
//#define VM_ALARM_CRD_MODE         (0x40000000)        //坐标系模式设置错误
#define VM_ALARM_CRDAXIS_MAP        (0x80000000)        //坐标系轴映射表错误

static uBit32 m_VmAlarm = 0;                            //系统报警字


/**
  * @brief  硬件配置信息设置(设置硬件配置信息后，坐标系映射表需重新建立)
  * @param  None
  * @retval None
  */
static void VM_MoveLiftMotorConfig(void)
{
    uBit32 ulRet;
	uBit32 ulCurDevCount=0;
	const DEVICE_INFO *pDeviceInfo;
	DEVICE_CONFIG DeviceConfig[8];
    CRDSYS_AXIS_MAP_TABLE CrdAxisMap;
    
    ulCurDevCount = DEV_GetCount(); //获取当前扫描到的设备数量 -- 仅注释 杜寒枫 2017.06.27

    //如果当前设备的数量不为1,则置警告 -- 仅注释 杜寒枫 2017.06.27
    if(ulCurDevCount != 1)
    {
      m_VmAlarm |= VM_ALARM_CONF_INVALID;
      return;
    }
    
    pDeviceInfo = DEV_GetDevInfoReadAddr(); //获取设备信息数组的首地址 -- 仅注释 杜寒枫 2017.06.30
   	memset(DeviceConfig, 0, sizeof(DeviceConfig));//直接在声明的时候对结构体初始化不更好? -- 仅注释 杜寒枫 2017.06.27
   	memset(&CrdAxisMap, 0, sizeof(CrdAxisMap)); 

    //如果设备为输入设备,则将其设备编号设置为设备ID的低3位+2
    //如果设备是输出设备,则将其序号作为设备ID
    //但是这样会不会存在冲突?
    // -- 仅注释 杜寒枫 2017.06.27
    for(uBit8 i=0; i<ulCurDevCount; i++)
    {
        if(pDeviceInfo[i].nDeviceType==DEV_TYPE_INPUT)
        {
            DeviceConfig[i].nDeviceNO = (pDeviceInfo[i].nDevieID&0X07)+2;
        }
        else
        {
            DeviceConfig[i].nDeviceNO = i;
        }
        
        DeviceConfig[i].nDevieID = pDeviceInfo[i].nDevieID;
    }
    
    //设置坐标系轴映射表
    CrdAxisMap.nCrdAxisCount=1;
    for(uBit8 i=0; i<8; i++)
    {
        CrdAxisMap.CrdSysAxisMap[i].cAxisName[0]='X';
        CrdAxisMap.CrdSysAxisMap[i].cAxisName[1]='X';
        CrdAxisMap.CrdSysAxisMap[i].cMotorNO[0]=DEV_INVALID_NO;
        CrdAxisMap.CrdSysAxisMap[i].cMotorNO[1]=DEV_INVALID_NO;
    }
    CrdAxisMap.CrdSysAxisMap[0].cMotorNO[0]=0;  
    
    //设置硬件配置信息
    //成功则返回0,否则则返回其错误码
    // -- 仅注释 杜寒枫 2017.06.27
    ulRet = SPM_SetConfig(DeviceConfig,ulCurDevCount);  
    //ulRet = DEV_SetConfig(DeviceConfig,ulCurDevCount); 

    if(ulRet)
    {
      m_VmAlarm |= VM_ALARM_CONF_INVALID;
      return;
    }

    //设置坐标系坐标轴映射表
    //成功则返回0,否则则返回其错误码
    // -- 仅注释 杜寒枫 2017.06.27
    ulRet = SPM_SetCrdSysAxisMapTable(&CrdAxisMap, 1);
    
    if(ulRet)
    {
       m_VmAlarm |= VM_ALARM_CRDAXIS_MAP;
    }
    
    m_VmAlarm &= ~VM_ALARM_CONF_INVALID;
}


/*****************************************************************************
 * 升降电机相关控制接口
 ****************************************************************************/

//升降平台位置状态定义
#define VM_LIFT_PLATFORM_IN_UNKNOWN                 (0)     //未知位置
#define VM_LIFT_PLATFORM_IN_UP_LIMIT_SITE           (1)     //在上限位
#define VM_LIFT_PLATFORM_IN_DOWN_LIMIT_SITE         (2)     //在下限位
#define VM_LIFT_PLATFORM_IN_CENTRE_SITE             (3)     //在中间(上下限位之间)

static uBit32 m_vm_ulLiftPlatformPosition = VM_LIFT_PLATFORM_IN_UNKNOWN; //提升平台的位置



/**
  * @brief  升降电机初始化
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 VM_InitLiftMotor(void)
{
    //升降电机配置
    VM_MoveLiftMotorConfig();
    
    PAX_SetCmdPos(VM_LIFT_MOTOR_DEV_NO, 0,1);  //设置轴指令位置
    PAX_SetQeiPos(VM_LIFT_MOTOR_DEV_NO, 0,1);  //设置编码器位置
    
    //使能电机
    uBit32 ulRet =PAX_Enable(VM_LIFT_MOTOR_DEV_NO, 1, 1);
    ulRet = CSM_SetCtrlMode(0, CRDSYS_STATUS_MODE_STEP);
    
    return ulRet;
}


static float m_vm_fPulseRate = 170.0;   //脉冲当量

/**
  * @brief  脉冲当量设置
  * @param  pPosCtrlMotion 位置运动参数
  * @retval 0-成功 非0-失败
  */
uBit32 VM_SetPulseRate(float fPulseRate)
{
    m_vm_fPulseRate = fPulseRate;
    return 0;
}


/**
  * @brief  升降电机运动控制
  * @param  pPosCtrlMotion 位置运动参数
  * @retval 0-成功 非0-失败
  */
uBit32 VM_MoveLiftMotor(Bit32 iMotorNO, POSCTRL_MOTION_DATA* pPosCtrlMotion)
{
    POSCTRL_MOTION_DATA TempPosCtrlMotion = {0};

    TempPosCtrlMotion.dPos = pPosCtrlMotion->dPos * m_vm_fPulseRate;
    TempPosCtrlMotion.dSpeed = fabs((pPosCtrlMotion->dSpeed * m_vm_fPulseRate)/1000);
    
    //设置工作模式
    if(CSM_GetCtrlMode(0) != CRDSYS_STATUS_MODE_STEP)
    {
        if(CSM_SetCtrlMode(0,CRDSYS_STATUS_MODE_STEP))
        {
            return VM_ERR_CRD_MODE;
        }
    }
    
    //设置电机移动
    if(CSM_SetMotorPosCtrlMotion(VM_LIFT_MOTOR_DEV_NO, &TempPosCtrlMotion))
    {
        return VM_ERR_LIFT_MOTOR;
    }
    
	return VM_ERR_SUCCEED;
}


/**
  * @brief  升降电机限位检测处理
  * @param  None
  * @retval None
  */
void VM_LiftPlatformLimitHandler(void)
{
    //假如当前升降电机的位置未知,则读取限位电平以确定其电机位置
    if (m_vm_ulLiftPlatformPosition == VM_LIFT_PLATFORM_IN_UNKNOWN)
    {
        //查询上限位状态
        if (GPIO_GetInputState(INPUT_IO_LIFT_MOTOR_UP_LIMIT) == VM_LIFT_MOTOR_TRIGGER_LEVEL)
        {
            m_vm_ulLiftPlatformPosition = VM_LIFT_PLATFORM_IN_UP_LIMIT_SITE;       //升降平台在上限位
        }
        //查询下限位状态
        else if (GPIO_GetInputState(INPUT_IO_LIFT_MOTOR_DOWN_LIMIT) == VM_LIFT_MOTOR_TRIGGER_LEVEL)
        {
            m_vm_ulLiftPlatformPosition = VM_LIFT_PLATFORM_IN_DOWN_LIMIT_SITE;     //升降平台在上限位
        }
        else
        {
            m_vm_ulLiftPlatformPosition = VM_LIFT_PLATFORM_IN_CENTRE_SITE;         //升降平台在中间
        }
        
        return;
    }
    
    //查询上限位状态
    if (GPIO_MAN_GetInputPinState(INPUT_IO_LIFT_MOTOR_UP_LIMIT) == VM_LIFT_MOTOR_TRIGGER_LEVEL)
    {
        //假如先前的升降平台不在上限位上而当前到达上限位,则急停电机
        if (m_vm_ulLiftPlatformPosition != VM_LIFT_PLATFORM_IN_UP_LIMIT_SITE)
        {
            //设置当前升降平台位置
            m_vm_ulLiftPlatformPosition = VM_LIFT_PLATFORM_IN_UP_LIMIT_SITE;
            
            //急停电机
            CSM_MotorJogEStop(VM_LIFT_MOTOR_DEV_NO);
        }
    }
    //查询下限位状态
    else if (GPIO_MAN_GetInputPinState(INPUT_IO_LIFT_MOTOR_DOWN_LIMIT) == VM_LIFT_MOTOR_TRIGGER_LEVEL)
    {
        //假如先前的升降平台不在下限位上而当前到达下限位,则急停电机
        if (m_vm_ulLiftPlatformPosition != VM_LIFT_PLATFORM_IN_DOWN_LIMIT_SITE)
        {
            //设置当前升降平台位置
            m_vm_ulLiftPlatformPosition = VM_LIFT_PLATFORM_IN_DOWN_LIMIT_SITE;
            
            //急停电机
            CSM_MotorJogEStop(VM_LIFT_MOTOR_DEV_NO);
        }
        
    }
    else 
    {
        //设置当前升降平台位置
        m_vm_ulLiftPlatformPosition = VM_LIFT_PLATFORM_IN_CENTRE_SITE;
    }
    
}


//升降平台回零操作步骤
#define VM_LP_HOME_RESET            (0)     //复位(默认状态)
#define VM_LP_HOME_START            (1)     //开始
#define VM_LP_HOME_DOWN             (2)     //下降
#define VM_LP_HOME_PREPARE_UP       (3)     //准备上升(机器惯性的原因,此阶段延时一小会)
#define VM_LP_HOME_UP               (4)     //上升
#define VM_LP_HOME_STOP             (5)     //停止
#define VM_LP_HOME_INDEX            (6)     //偏移
#define VM_LP_HOME_BAK              (7)     //抱闸

static uBit8 m_vm_uLiftPlatformHomeStep = VM_LP_HOME_RESET; //升降平台回零步骤

//零点搜寻状态定义
#define VM_HOME_STATUS_NORMAL       (0)     //正常
#define VM_HOME_STATUS_SCAN         (1)     //寻找零点中

static uBit8 m_vm_uScanHomeStatus = VM_HOME_STATUS_NORMAL;  //零点搜寻状态
static SYS_TIME_DATA m_vm_HomeCtrlTimer = {0};              //回零控制定时器

/**
  * @brief  零点搜寻开始
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 VM_StartLiftPlatformHomeScan(void)
{
    if (m_vm_uLiftPlatformHomeStep == VM_LP_HOME_RESET)
    {
        m_vm_uLiftPlatformHomeStep = VM_LP_HOME_START;
        
        return 0;
    }
    
    return 1;
}


/**
  * @brief  零点搜寻停止
  * @param  None
  * @retval 0-成功 非0-失败
  */
uBit32 VM_StopLiftPlatformHomeScan(void)
{
    m_vm_uLiftPlatformHomeStep = VM_LP_HOME_BAK;
    
    //停止电机
    CSM_SetMotorJogStop(VM_LIFT_MOTOR_DEV_NO);
    
    //设置抱闸延时
    SysTime_StartOneShot(&m_vm_HomeCtrlTimer, 1000);   //延时1S
    
    return 0;
}


/**
  * @brief  零点搜寻状态获取
  * @param  None
  * @retval 0-正常 1-零点搜寻中
  */
uBit32 VM_GetHomeScanStatus(void)
{
    
    return m_vm_uScanHomeStatus;
}


/**
  * @brief  升降平台零点搜寻处理
  * @param  None
  * @retval None
  */
void VM_LiftPlatformHomeHandler(void)
{
    POSCTRL_MOTION_DATA PosCtrlMotion = {0};
    
    switch (m_vm_uLiftPlatformHomeStep)
    {
    case VM_LP_HOME_RESET:  break;
    case VM_LP_HOME_START:
        
        if (m_vm_ulLiftPlatformPosition != VM_LIFT_PLATFORM_IN_UNKNOWN)
        {
            //设置零点寻找状态:搜寻中
            m_vm_uScanHomeStatus = VM_HOME_STATUS_SCAN;
            
            //开抱闸
            //GPIO_MAN_SetOutputPinState(OUTPUT_IO_OUTPUT26, true);
            
            //若升降平台在下限位,则往上走,否则则往下走
            if (m_vm_ulLiftPlatformPosition == VM_LIFT_PLATFORM_IN_DOWN_LIMIT_SITE)
            {
                m_vm_uLiftPlatformHomeStep = VM_LP_HOME_UP;
                
                PosCtrlMotion.dPos = 10000;
                PosCtrlMotion.dSpeed = VM_LIFT_MOTOR_HOME_SPEED;
                
                VM_MoveLiftMotor(VM_LIFT_MOTOR_DEV_NO, &PosCtrlMotion);
            }
            else 
            {
                m_vm_uLiftPlatformHomeStep = VM_LP_HOME_DOWN;   
                
                POSCTRL_MOTION_DATA PosCtrlMotion = {0};
                
                PosCtrlMotion.dPos = -10000;
                PosCtrlMotion.dSpeed = VM_LIFT_MOTOR_HOME_SPEED;
                
                VM_MoveLiftMotor(VM_LIFT_MOTOR_DEV_NO, &PosCtrlMotion);
            }
        }
        
        break;
    case VM_LP_HOME_DOWN :
        
        //假如到达下限位,则往上走
        if (m_vm_ulLiftPlatformPosition == VM_LIFT_PLATFORM_IN_DOWN_LIMIT_SITE)
        {
            m_vm_uLiftPlatformHomeStep = VM_LP_HOME_PREPARE_UP;
            
            SysTime_StartOneShot(&m_vm_HomeCtrlTimer, 1000);   //延时1S
        }
        
        break;
    case VM_LP_HOME_PREPARE_UP :
        
        if (SysTime_CheckExpiredState(&m_vm_HomeCtrlTimer))
        {
            m_vm_uLiftPlatformHomeStep = VM_LP_HOME_UP;
                
            PosCtrlMotion.dPos = 10000;
            PosCtrlMotion.dSpeed = VM_LIFT_MOTOR_HOME_SPEED;
            
            VM_MoveLiftMotor(VM_LIFT_MOTOR_DEV_NO, &PosCtrlMotion);
        }
        
        break;
    case VM_LP_HOME_UP   :
        
        //假如平台离开限位,则停止电机
        if (m_vm_ulLiftPlatformPosition == VM_LIFT_PLATFORM_IN_CENTRE_SITE)
        {
            m_vm_uLiftPlatformHomeStep = VM_LP_HOME_STOP;
        }
        
        break;
    case VM_LP_HOME_STOP :                                                                              
        
        m_vm_uLiftPlatformHomeStep = VM_LP_HOME_INDEX;
        
        //急停电机
        CSM_SetMotorJogStop(VM_LIFT_MOTOR_DEV_NO);
        
        //设置当前位置为零点
        PAX_SetCmdPos(VM_LIFT_MOTOR_DEV_NO, 0, 0);
        
#if 0
        //往上走一定距离,作为零点偏移
        PosCtrlMotion.dPos = VM_LIFT_MOTOR_HOME_INDEX;
        PosCtrlMotion.dSpeed = VM_LIFT_MOTOR_HOME_SPEED*5;

        VM_MoveLiftMotor(VM_LIFT_MOTOR_DEV_NO, &PosCtrlMotion);
#endif
        
        //设置下个步骤的执行时间
        SysTime_StartOneShot(&m_vm_HomeCtrlTimer, 2000);   //延时1S
        
        break;
    case VM_LP_HOME_INDEX:
        
        if (SysTime_CheckExpiredState(&m_vm_HomeCtrlTimer))
        {
            m_vm_uLiftPlatformHomeStep = VM_LP_HOME_BAK;
            
            //设置当前位置为零点(偏移后的位置)
            PAX_SetCmdPos(VM_LIFT_MOTOR_DEV_NO, 0, 0);
            
            //设置下个步骤的执行时间
            SysTime_StartOneShot(&m_vm_HomeCtrlTimer, 20);   //延时
        }
        
        
        break;
    case VM_LP_HOME_BAK:
        
        if (SysTime_CheckExpiredState(&m_vm_HomeCtrlTimer))
        {
            m_vm_uLiftPlatformHomeStep = VM_LP_HOME_RESET;
            
            //设置零点寻找状态
            m_vm_uScanHomeStatus = VM_HOME_STATUS_NORMAL;
            
            //关抱闸
            //GPIO_MAN_SetOutputPinState(OUTPUT_IO_OUTPUT26, false);
        }
            
        break;
    default:    //不应该到这里
        m_vm_uLiftPlatformHomeStep = VM_LP_HOME_RESET;
        break;
    }
    
}


