/**
  ******************************************************************************
  * @file    GCIOCtrl.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.10.20
  * @brief   
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
   
   
/***********************************<INCLUDES>**********************************/
#include "VM_HwCtrl.h"
#include "VM_StateMan.h"
#include "VM_ErrorDef.h"
#include "VM_HardwareDef.h"
      
#include "DataType/DataType.h"
#include "SysCtrl/SysUpdate/SysUpdate.h"
#include "SysPeripheral/GPIO/GPIO_Man.h"
#include "SysPeripheral/KEY/KEY.h"
#include "SysPeripheral/SysTimer/SysTimer.h"
#include "SysPeripheral/UART/UART.h"
#include "SysPeripheral/PWM/PWM.h"
#include "SysPeripheral/PWM/PWM_MotorCtrl.h"
#include "MotorCtrl/MotorCtrl.h"

#include "ExtPeripheral/NixieTube/NixieTube.h"

#include <stdio.h>
#include <string.h>

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

/**
  * @brief  IO初始化
  * @param  None
  * @retval None
  */
static void VM_IOConfig(void)
{
    //设置控制IO资源表
    GPIO_SetCtrlTable(&g_GcpIOTable);
    
    //先单独初始化IO使能脚
    GPIO_InitIOGroup(&g_GcpIOTable.pOutputGroup[OUTPUT_IO_OE1_N], 4, GPIO_MODE_OUPUT);  //模式配置
    
    //设置IO使能脚逻辑翻转
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_OE1_N, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_OE2_N, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_OE3_N, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_OE4_N, true);
    
    //在初始化其他IO之前,禁止IO输出
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OE1_N, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OE2_N, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OE3_N, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OE4_N, false);
    
    //初始化资源表内的IO
    GPIO_InitIOTable(&g_GcpIOTable);
    
    //对输出IO进行逻辑翻转
    //10个电机行控制信号
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_ROW_OUTPUT1,  true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_ROW_OUTPUT2,  true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_ROW_OUTPUT3,  true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_ROW_OUTPUT4,  true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_ROW_OUTPUT5,  true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_ROW_OUTPUT6,  true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_ROW_OUTPUT7,  true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_ROW_OUTPUT8,  true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_ROW_OUTPUT9,  true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_ROW_OUTPUT10, true);
    //10个电机行控制信号
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_COL_OUTPUT11, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_COL_OUTPUT12, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_COL_OUTPUT13, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_COL_OUTPUT14, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_COL_OUTPUT15, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_COL_OUTPUT16, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_COL_OUTPUT17, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_COL_OUTPUT18, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_COL_OUTPUT19, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_MOTRO_COL_OUTPUT20, true);
    //7个通用OUTPUT
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_OUTPUT21, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_OUTPUT22, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_OUTPUT23, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_OUTPUT24, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_OUTPUT25, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_OUTPUT26, true);
    GPIO_MAN_SetOutputPinLogicToggle(OUTPUT_IO_OUTPUT27, true);
    
    //设置默认电平
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_ROW_OUTPUT1,  false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_ROW_OUTPUT2,  false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_ROW_OUTPUT3,  false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_ROW_OUTPUT4,  false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_ROW_OUTPUT5,  false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_ROW_OUTPUT6,  false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_ROW_OUTPUT7,  false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_ROW_OUTPUT8,  false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_ROW_OUTPUT9,  false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_ROW_OUTPUT10, false);
    
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_COL_OUTPUT11, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_COL_OUTPUT12, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_COL_OUTPUT13, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_COL_OUTPUT14, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_COL_OUTPUT15, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_COL_OUTPUT16, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_COL_OUTPUT17, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_COL_OUTPUT18, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_COL_OUTPUT19, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_COL_OUTPUT20, false);
    
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OUTPUT21, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OUTPUT22, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OUTPUT23, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OUTPUT24, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OUTPUT25, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OUTPUT26, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OUTPUT27, false);
    
    //使能IO
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OE1_N, true);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OE2_N, true);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OE3_N, true);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OE4_N, true);
    
}


/*****************************************************************************
 * 硬件配置接口
 ****************************************************************************/

/**
  * @brief  系统硬件初始化
  * @param  None
  * @retval None
  */
void VM_HwInit(void)
{
    //初始化IO
    VM_IOConfig();
    
    //初始化轴
    AXIS_Init(0, 3, 1, 0, 0, OUTPUT_IO_MOTOR_DIR);
    
    //设置Bootloader LED灯
    SYS_UPDATE_SetMainWorkLed(g_GcpIOTable.pOutputGroup[OUTPUT_IO_LED_RUN1].nPort, 
                              g_GcpIOTable.pOutputGroup[OUTPUT_IO_LED_RUN1].nPin);
    
}


/*****************************************************************************
 * LED显示线程接口
 ****************************************************************************/
#define VM_LED_TOGGLE_TIME          (100)       //LED翻转时间(MS)
static SYS_TIME_DATA m_LedCtrlTimer  = {1};     //LED控定时器

/**
  * @brief  LED 流水灯循环
  * @param  None
  * @retval None
  */
void VM_MainWorkLedShow(void)
{
    if (SysTime_CheckExpiredState(&m_LedCtrlTimer))
    {
        SysTime_StartOneShot(&m_LedCtrlTimer, VM_LED_TOGGLE_TIME); //设置下一次执行的时间
        
        GPIO_ToggleOutputState(OUTPUT_IO_LED_RUN1);
        
    }

}


/*****************************************************************************
 * 货道限位IO检测线程接口
 ****************************************************************************/

//货道电机运行状态定义
#define VM_AISLE_MOTOR_STATUS_IDLE       (0)            //货道电机空闲
#define VM_AISLE_MOTOR_STATUS_RUNNING    (1)            //货道电机运行中
#define VM_AISLE_MOTOR_STATUS_OVER       (2)            //超时

//货道电机时间参数定义
#define VM_AISLE_MOTOR_DELAY_TIME       (100)           //货道电机检测延时时间(MS)
#define VM_AISLE_MOTOR_OVER_TIME        (5000)          //货道电机超时时间(MS)
static SYS_TIME_DATA m_AisleDelayTimer = {0};           //货道电机检测延时定时器
static SYS_TIME_DATA m_AisleOverTimer = {0};            //货道电机超时定时器

static uBit32 m_vm_ulCurRow = 0xFF;                     //当前行
static uBit32 m_vm_ulCurCol = 0xFF;                     //当前列

static bool m_vm_bInLimitPosFlag = false;               //限位状态标志
static uBit32 m_vm_ulAisleMotorRunningStatus = 0;       //货道电机运行状态

     
/**
  * @brief  货道电机启动
  * @param  ulRow 行信号 [0-9]
  * @param  ulCol 列信号 [0-9]
  * @retval 0-成功 非0-失败
  */
uBit32 VM_EnableAisleMotor(uBit32 ulRow, uBit32 ulCol)
{
    //校验入参参数
    if ((ulRow >= VM_AISLE_MAX_ROW) || (ulCol >= VM_AISLE_MAX_COL))
    {
        //设置货道不存在报警
        VM_SetAlarmBit(VM_ERR_CH_INVALID);
        return 1;
    }
    
    //判断电机是否在运行
    if (m_vm_ulAisleMotorRunningStatus == VM_AISLE_MOTOR_STATUS_RUNNING)
    {
        //设置货道冲突报警
        //在货道电机停止前,禁止启动新的货道电机
        VM_SetAlarmBit(VM_ERR_CH_CONFLICT);
        
        return 1;
    }
    
    m_vm_ulCurRow = ulRow;
    m_vm_ulCurCol = ulCol;
    
    //启动货道电机
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_ROW_OUTPUT1 + m_vm_ulCurRow, 1);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_COL_OUTPUT11 + m_vm_ulCurCol, 1);
    
    //设置电机运行标志位
    m_vm_ulAisleMotorRunningStatus = VM_AISLE_MOTOR_STATUS_RUNNING;
    
    //启动检测延时定时器
    //货道电机的限位信号需要在货道电机启动之后才有效,否则可能会导致误判,所以需要
    //增加一点延时时间,在此段时间之后,才对货道电机的限位信号进行检测
    SysTime_Start(&m_AisleDelayTimer, VM_AISLE_MOTOR_DELAY_TIME);
    
    //启动超时定时器
    SysTime_Start(&m_AisleOverTimer, VM_AISLE_MOTOR_OVER_TIME);
    
    return 0;
}
     

/**
  * @brief  货道电机运行状态获取
  * @param  None
  * @retval 货道电机运行状态
  *   @arg  VM_AISLE_MOTOR_STATUS_IDLE    货道电机空闲
  *         VM_AISLE_MOTOR_STATUS_RUNNING 货道电机运行中
  *         VM_AISLE_MOTOR_STATUS_OVER    超时
  */
uBit32 VM_GetAisleMotorRunningState(void)
{
    
    return m_vm_ulAisleMotorRunningStatus;
}


/**
  * @brief  货道电机管理
  * @param  None
  * @retval None
  */
void VM_AisleMotorHandler(void)
{
    //检测延时时间到达判断
    if (SysTime_CheckExpiredState(&m_AisleDelayTimer))
    {
        if ((m_vm_ulCurRow != 0xFF) && (m_vm_ulCurCol != 0xFF))
        {
            //假如当前在限位信号上,则等待转过限位信号
            if (m_vm_bInLimitPosFlag == false)
            {
                if (GPIO_MAN_GetInputPinState(INPUT_IO_MOTRO_INDEX_INPUT1 + m_vm_ulCurRow) == 0)
                {
                    m_vm_bInLimitPosFlag = true;
                }
            }
            if (m_vm_bInLimitPosFlag == true) 
            {
                //检测限位信号
                if (GPIO_MAN_GetInputPinState(INPUT_IO_MOTRO_INDEX_INPUT1 + m_vm_ulCurRow) == 1)
                {
                    //若到达限位信号,关闭货道电机输出
                    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_ROW_OUTPUT1 + m_vm_ulCurRow, 0);
                    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_COL_OUTPUT11 + m_vm_ulCurCol, 0);
                    
                    //初始化行列号
                    m_vm_ulCurRow = 0xFF;
                    m_vm_ulCurCol = 0xFF;
                    m_vm_bInLimitPosFlag = false;
                    
                    //关闭超时定时器
                    SysTime_Cancel(&m_AisleDelayTimer);
                    SysTime_Cancel(&m_AisleOverTimer);
                    
                    //设置货道电机状态
                    m_vm_ulAisleMotorRunningStatus = VM_AISLE_MOTOR_STATUS_IDLE;
                }
            }
        }
    }
    
    //超时时间到达判断
    if (SysTime_CheckExpiredState(&m_AisleOverTimer))
    {
        //关闭货道电机输出
        GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_ROW_OUTPUT1 + m_vm_ulCurRow, 0);
        GPIO_MAN_SetOutputPinState(OUTPUT_IO_MOTRO_COL_OUTPUT11 + m_vm_ulCurCol, 0);
        m_vm_ulCurRow = 0xFF;
        m_vm_ulCurCol = 0xFF;
        m_vm_bInLimitPosFlag = false;
        
        //设置报警位
        VM_SetAlarmBit(VM_ERR_CH_INVALID);
        
        //关闭超时定时器
        SysTime_Cancel(&m_AisleDelayTimer);
        SysTime_Cancel(&m_AisleOverTimer);
        
        //设置货道电机状态
        m_vm_ulAisleMotorRunningStatus = VM_AISLE_MOTOR_STATUS_OVER;
    }
    
}

