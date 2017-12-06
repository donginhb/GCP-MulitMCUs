/**
  ******************************************************************************
  * @file    Demo.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.12.05
  * @brief   demo
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
#include "VMErrorDef.h"
#include "DataType/DataType.h"
      
#include "CNC/DataStructDef/IPO_DataStructDefPub.h"
#include "CNC/DataStructDef/DEV_DataStructDef.h"
#include "CNC/CSM/CSM_Interface.h"
#include "CNC/CSM/CSM_CrdSysState.h"
#include "CNC/DVM/DEV_Interface.h"
#include "CNC/SPM/SPM_SysCtrlParm.h"

#include "SysPeripheral/SysTimer/SysTimer.h"

#include <math.h>
#include <string.h>


#define LIFT_MOTOR_DEV_NO (0)

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
//#define VM_ALARM_CRD_MODE         (0x40000000)      //坐标系模式设置错误
#define VM_ALARM_CRDAXIS_MAP        (0x80000000)        //坐标系轴映射表错误

static uBit32 m_VmAlarm = 0;

/*****************************************************************************
 * DEMO相关控制接口
 ****************************************************************************/


//设置硬件配置信息(设置硬件配置信息后，坐标系映射表需重新建立)
void VM_SetConfig(void)
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



//升降电机运动控制 fPos mm fSpeed mm/s,bCheckLimit-是否检测极限
uBit16 VM_MoveLiftMotor(float32 fPos, float32 fSpeed, uBit8 bCheckLimit)
{
	POSCTRL_MOTION_DATA sPosCtrlMotion = {0};
    
    sPosCtrlMotion.dPos = fPos * 100;   //100是脉冲当量
    sPosCtrlMotion.dSpeed = fabs((fSpeed * 100) / 1000.0);
    
    //设置工作模式
    if(CSM_GetCtrlMode(0) != CRDSYS_STATUS_MODE_STEP)
    {
        if(CSM_SetCtrlMode(0,CRDSYS_STATUS_MODE_STEP))
            return VM_ERR_CRD_MODE;
    }
    
    if(CSM_SetMotorPosCtrlMotion(LIFT_MOTOR_DEV_NO, &sPosCtrlMotion))
        return VM_ERR_LIFT_MOTOR;

	return VM_ERR_SUCCEED;
}



/*****************************************************************************
 * LED显示线程接口
 ****************************************************************************/
#define VM_MOTOR_TEST_PERIOD        (5000)                 //电机测试周期(单位: MS)
static SYS_TIME_DATA m_MotorCtrlTimer  = {1};               //电机控制定时器



void VM_MoveLiftMotorTest(void)
{
    static float fPos = 1750;
        
    if (SysTime_CheckExpiredState(&m_MotorCtrlTimer))
    {
        SysTime_StartOneShot(&m_MotorCtrlTimer, VM_MOTOR_TEST_PERIOD); //设置下一次执行的时间
        
        VM_MoveLiftMotor(fPos, 1000, 0);
        
        fPos = -fPos;
    }
    
}


