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
#include "WB01_HwCtrl.h"
#include "WB01_HardwareDef.h"

#include "DataType/DataType.h"
#include "SysPeripheral/GPIO/GPIO_Man.h"
#include "SysPeripheral/SysTimer/SysTimer.h"
#include "SysPeripheral/UART/UART.h"
#include "SysPeripheral/KEY/KEY.h"
#include "SysPeripheral/PWM/PWM.h"

#include "ExtPeripheral/LogicIC/HC595.h"
      
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define WB01_DEBUG

#ifdef WB01_DEBUG
//#define DEBUF_PRINT(x)      UART_SendStr(WB01_DEBUG_UART_NODE, x)
#define DEBUF_PRINT(x)      UART_SendStr(WB01_COM_UART_NODE, x)
#else 
#define DEBUF_PRINT(x)   
#endif


/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/

/**
  * @brief  IO初始化
  * @param  None
  * @retval None
  */
static void WB01_IOConfig(void)
{
    //设置控制IO资源表
    GPIO_SetCtrlTable(&g_GcpIOTable);
    
    //初始化资源表内的IO
    GPIO_InitIOTable(&g_GcpIOTable);
    
    //设置IO逻辑翻转
    GPIO_MAN_SetInputPinLogicToggle(INPUT_IO_HALL_SENSOR, true);
    GPIO_MAN_SetInputPinLogicToggle(INPUT_IO_IN_DOOR_OPEN_LIMIT, true);
    GPIO_MAN_SetInputPinLogicToggle(INPUT_IO_IN_DOOR_CLOSE_LIMIT, true);
    GPIO_MAN_SetInputPinLogicToggle(INPUT_IO_OUT_DOOR_OPEN_LIMIT, true);
    GPIO_MAN_SetInputPinLogicToggle(INPUT_IO_OUT_DOOR_CLOSE_LIMIT, true);
    GPIO_MAN_SetInputPinLogicToggle(INPUT_IO_LIGHT_REACTION, true);
    
//    GPIO_MAN_SetInputPinLogicToggle(INPUT_IO_MAIN_AXIS_UP, true);
//    GPIO_MAN_SetInputPinLogicToggle(INPUT_IO_MAIN_AXIS_DOWN, true);
//    GPIO_MAN_SetInputPinLogicToggle(INPUT_IO_MAIN_AXIS_STOP, true);
    
    //初始化电机的默认电平
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MAIN_AXIS_EN, true);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_MAIN_AXIS_DIR, false);
    //GPIO_MAN_SetOutputPinState(OUTPUT_IO_MAIN_AXIS_S, false);
    
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_TRANSF_MOTOR, false);
    
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_IN_DOOR_DIR, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_IN_DOOR_EN, false);
    
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OUT_DOOR_DIR, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_OUT_DOOR_EN, false);
    
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_HC595_SCK, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_HC595_RCK, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_HC595_OE, false);
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_HC595_SI, false);
    
    GPIO_MAN_SetOutputPinState(OUTPUT_IO_LIGHT_POWER, true);    //直接打开光栅供电
    
}


/*****************************************************************************
 * 硬件配置接口
 ****************************************************************************/

/**
  * @brief  系统硬件初始化
  * @param  None
  * @retval None
  */
void WB01_HwInit(void)
{
    //初始化IO
    WB01_IOConfig();
    
    //初始化串口
    UART_Init(WB01_COM_UART_NODE, 115200);      //上位机通信串口
    UART_Init(WB01_DEBUG_UART_NODE, 115200);    //下位机调试串口
    
    //初始化按键识别功能(霍尔传感器)
    uBit32 ulKeyPinGourp[] = {INPUT_IO_HALL_SENSOR};
    KEY_SetScanPinGroup(ulKeyPinGourp, sizeof(ulKeyPinGourp)/sizeof(ulKeyPinGourp[0]));
    
    //初始化步进电机PWM输出通道
    PWM_Init(WB01_MOTOR_PWM_NODE, WB01_MOTOR_PWM_CH_MASK);
    PWM_SetOutputPwmDutyRatio(WB01_MOTOR_PWM_NODE, WB01_MOTOR_PWM_CH_MASK, 50);
    PWM_SetOutputPwmFrq(WB01_MOTOR_PWM_NODE, 200);
    PWM_OutputEnable(WB01_MOTOR_PWM_NODE, false);
    
    //初始化HC595
    HC595_Init(OUTPUT_IO_HC595_SCK, OUTPUT_IO_HC595_RCK, OUTPUT_IO_HC595_SI, 16);
    
    //关闭货道电机
    WB01_SetAsileMotor(0, 0, 0);
    
}


/*****************************************************************************
 * LED显示线程接口
 ****************************************************************************/
#define WB01_LED_TOGGLE_TIME          (100)     //LED翻转时间(MS)
static SYS_TIME_DATA m_LedCtrlTimer  = {1};     //LED控定时器

/**
  * @brief  LED 流水灯循环
  * @param  None
  * @retval None
  */
void WB01_MainWorkLedShow(void)
{
    if (SysTime_CheckExpiredState(&m_LedCtrlTimer))
    {
        SysTime_StartOneShot(&m_LedCtrlTimer, WB01_LED_TOGGLE_TIME); //设置下一次执行的时间
        
        GPIO_ToggleOutputState(OUTPUT_IO_LED0);
    }

}


/*****************************************************************************
 * 主轴电机线程接口
 ****************************************************************************/

//电机运行状态定义
typedef enum
{
    WB01_MOTOR_STATUS_STOP = 0,     //停止
    WB01_MOTOR_STATUS_CW  ,         //正转
    WB01_MOTOR_STATUS_ACW ,         //反转
}WB01_MOTOR_STATUS;



//主轴电机当前运动方向定义
#define WB01_MOTOR_DIR_CW                   (0) //正转方向
#define WB01_MOTOR_DIR_ACW                  (1) //逆转方向

static uBit8 m_uCurMotorDir = WB01_MOTOR_DIR_CW;    //当前电机运动方向 0-正转 1-逆转
static uBit8 m_uCurAxisMotorStatus = 0;             //当前主轴电机状态 0-停止 1-正转 2-反转


/**
  * @brief  主轴电机状态设置
  * @param  uMotorStatus:
  *     @arg WB01_MOTOR_STATUS_STOP   停止
  *     @arg WB01_MOTOR_STATUS_CW     正转
  *     @arg WB01_MOTOR_STATUS_ACW    反转
  * @retval None
  */
void WB01_SetMainAxisMotorStatus(uBit8 uMotorStatus)
{
    switch (uMotorStatus)
    {
    case WB01_MOTOR_STATUS_STOP:
        m_uCurAxisMotorStatus = uMotorStatus;
        PWM_OutputEnable(WB01_MOTOR_PWM_NODE, false);
        break;
    case WB01_MOTOR_STATUS_CW: 
        m_uCurMotorDir = WB01_MOTOR_DIR_CW;
        m_uCurAxisMotorStatus = uMotorStatus;
        GPIO_MAN_SetOutputPinState(OUTPUT_IO_MAIN_AXIS_DIR, true);
        PWM_OutputEnable(WB01_MOTOR_PWM_NODE, true);
        break;
    case WB01_MOTOR_STATUS_ACW: 
        m_uCurMotorDir = WB01_MOTOR_DIR_ACW;
        m_uCurAxisMotorStatus = uMotorStatus;
        GPIO_MAN_SetOutputPinState(OUTPUT_IO_MAIN_AXIS_DIR, false);
        PWM_OutputEnable(WB01_MOTOR_PWM_NODE, true);
        break;
    default: break;
    }
    
}


/**
  * @brief  主轴电机速度设置
  * @param  ulSpeed PWM的输出频率
  * @retval None
  */
void WB01_SetMainAxisMotorSpeed(uBit32 ulSpeed)
{
    PWM_SetOutputPwmFrq(WB01_MOTOR_PWM_NODE, ulSpeed);
    
}


/**
  * @brief  主轴电机状态获取
  * @param  uMotorStatus: 0-停止 1-正转 2-反转 3-刹车
  * @retval 电机状态
  *     @arg WB01_MOTOR_STATUS_STOP   停止
  *     @arg WB01_MOTOR_STATUS_CW     正转
  *     @arg WB01_MOTOR_STATUS_ACW    反转
  *     @arg WB01_MOTOR_STATUS_ESTOP  刹车
  */
uBit8 WB01_GetMainAxisMotorStatus(void)
{
    
    return m_uCurAxisMotorStatus;
}


/*****************************************************************************
 * 出货流程线程接口
 ****************************************************************************/

//主轴电机出货流程速度定义
#define WB01_OUTGOODS_START_SPEED           (200)   //启动速度
#define WB01_OUTGOODS_FAST_SPEED            (800)   //快进速度
#define WB01_OUTGOODS_SLOW_SPEED            (400)   //慢进速度
#define WB01_OUTGOODS_RESET_SPEED           (400)   //复位速度

#define WB01_OUTGOODS_ACC_SPEED             (200)   //加速度(单位:Hz/S)

#define WB01_OUTGOODS_SLOW_GRID_COUNT       (1)     //慢进的格子数

#define WB01_OUTGOODS_GRID_TIMEROVER        (3000)  //单格超时时间

#define WB01_OUTGOODS_PROC_TIME             (20)    //出货流程监控间隔(MS)
#define WB01_OUTGOODS_RESET_LIMIT_STOP_TIME (1000)  //出货复位流程限位间隔(MS)

typedef enum
{
    WB01_OUTGOODS_STEP_STOP = 0,        //停止
    WB01_OUTGOODS_STEP_START,           //启动
    WB01_OUTGOODS_STEP_SPEED_UP,        //加速
    WB01_OUTGOODS_STEP_FAST_KEEP,       //快进速度保持
    WB01_OUTGOODS_STEP_SPEED_DOWN,      //减速
    WB01_OUTGOODS_STEP_SLOW_KEEP,       //慢进速度保持
    WB01_OUTGOODS_STEP_FINISH,          //结束处理
    WB01_OUTGOODS_STEP_BUSY,            //忙(复位过程中,设置为此状态)
    
}WB01_OUTGOODS_STEP;


typedef enum
{
    WB01_SELF_LEARN_STEP_STOP = 0,          //停止
    WB01_SELF_LEARN_STEP_RUN_UP_LIMIT,      //运行到上限位
    WB01_SELF_LEARN_STEP_UP_KEEP,           //限位保持
    WB01_SELF_LEARN_STEP_RUN_DOWN_LIMIT,    //运行到下限位
    WB01_SELF_LEARN_STEP_DOWN_KEEP,         //限位保持
    WB01_SELF_LEARN_STEP_FINISH,            //结束处理
    
}WB01_OUTGOODS_RESET_STEP;

static SYS_TIME_DATA m_OutGoodsCtrlTimer = {0};         //出货流程控制定时器
static SYS_TIME_DATA m_SelfLearnCtrlTimer = {0};        //自学习流程控制定时器
static SYS_TIME_DATA m_OutGoodsResetCtrlTimer = {0};    //出货复位流程控制定时器

static Bit32 m_lMaxGridCount = 25;          //最大的格子数
static Bit32 m_lCurGridNumber = 0;          //当前转过的格子
static Bit32 m_lObjGridNumber = 0;          //要出货的格子数(目标格子数)

static WB01_OUTGOODS_STEP m_CurOutGoodsStep = WB01_OUTGOODS_STEP_STOP;    //当前出货流程
static WB01_OUTGOODS_RESET_STEP m_SelfLearnStep = WB01_SELF_LEARN_STEP_STOP; //当前自学习流程
static uBit32 ulCurOutGoodsSpeed = 0;               //当前出货速度

/**
  * @brief  目标柜号获取
  * @param  None
  * @retval 目标柜号
  */
uBit32 WB01_GetObjGridNumber(void)
{
    
    return m_lObjGridNumber;
}


/**
  * @brief  总柜数获取
  * @param  None
  * @retval 总柜数
  */
uBit32 WB01_GetMaxGridCount(void)
{
    
    return m_lMaxGridCount;
}


/**
  * @brief  自学习开始
  * @param  None
  * @retval 0-成功 1-忙
  */
uBit32 WB01_StartSelfLearn(void)
{
    if ((m_SelfLearnStep != WB01_SELF_LEARN_STEP_STOP) ||
        (m_CurOutGoodsStep != WB01_OUTGOODS_STEP_STOP))
    {
        return 1;
    }
    
    //设置出货流程状态
    m_CurOutGoodsStep = WB01_OUTGOODS_STEP_BUSY;
    
    //设置出货复位流程状态
    m_SelfLearnStep = WB01_SELF_LEARN_STEP_RUN_UP_LIMIT;
    
    //设置控制定时器
    SysTime_StartOneShot(&m_SelfLearnCtrlTimer, WB01_OUTGOODS_PROC_TIME);
    
    m_lMaxGridCount = 0xFF;
    m_lCurGridNumber = 0;
    
    return 0;
}


/**
  * @brief  自学习任务处理
  * @param  None
  * @retval None
  */
void WB01_SelfLearnHandler(void)
{
    switch (m_SelfLearnStep)
    {
    case WB01_SELF_LEARN_STEP_STOP           :  //停止          
        break;
    case WB01_SELF_LEARN_STEP_RUN_UP_LIMIT   :  //运行到上限位
        
        if (SysTime_CheckExpiredState(&m_SelfLearnCtrlTimer))
        {
            SysTime_StartOneShot(&m_SelfLearnCtrlTimer, WB01_OUTGOODS_PROC_TIME);
            
            //若到达了上限位,则停止电机并进入限位保持状态
            if (GPIO_MAN_GetInputPinState(INPUT_IO_MAIN_AXIS_UP))
            {
                ulCurOutGoodsSpeed = 0;
                m_SelfLearnStep = WB01_SELF_LEARN_STEP_UP_KEEP;
                WB01_SetMainAxisMotorStatus(WB01_MOTOR_STATUS_STOP);
                
                SysTime_StartOneShot(&m_SelfLearnCtrlTimer, WB01_OUTGOODS_RESET_LIMIT_STOP_TIME);
                
                DEBUF_PRINT("WB01_SelfLearnHandler: In Up Limit And Keep 1S!\r\n");
                break;
            }
            
            //加速到快进速度
            if (ulCurOutGoodsSpeed < WB01_OUTGOODS_RESET_SPEED)
            {
                ulCurOutGoodsSpeed += WB01_OUTGOODS_ACC_SPEED*WB01_OUTGOODS_PROC_TIME/1000;
                WB01_SetMainAxisMotorSpeed(ulCurOutGoodsSpeed);
                WB01_SetMainAxisMotorStatus(WB01_MOTOR_STATUS_CW);
            }
        }
        
        break;
    case WB01_SELF_LEARN_STEP_UP_KEEP        :  //限位保持
        
        if (SysTime_CheckExpiredState(&m_SelfLearnCtrlTimer))
        {
            m_SelfLearnStep = WB01_SELF_LEARN_STEP_RUN_DOWN_LIMIT;
            SysTime_StartOneShot(&m_SelfLearnCtrlTimer, WB01_OUTGOODS_PROC_TIME);
            
            //复位相关变量
            m_lMaxGridCount = 0xFF;
            m_lCurGridNumber = 0;
        }
        
        break;
    case WB01_SELF_LEARN_STEP_RUN_DOWN_LIMIT :  //运行到下限位
        
        if (SysTime_CheckExpiredState(&m_SelfLearnCtrlTimer))
        {
            SysTime_StartOneShot(&m_SelfLearnCtrlTimer, WB01_OUTGOODS_PROC_TIME);
            
            //若到达了下限位,则停止电机并进入限位保持状态
            if (GPIO_MAN_GetInputPinState(INPUT_IO_MAIN_AXIS_DOWN))
            {
                ulCurOutGoodsSpeed = 0;
                m_SelfLearnStep = WB01_SELF_LEARN_STEP_DOWN_KEEP;
                WB01_SetMainAxisMotorStatus(WB01_MOTOR_STATUS_STOP);
                
                SysTime_StartOneShot(&m_SelfLearnCtrlTimer, WB01_OUTGOODS_RESET_LIMIT_STOP_TIME);
                
                DEBUF_PRINT("WB01_SelfLearnHandler: In Down Limit And Keep 1S!\r\n");
                break;
            }
            
            //加速到快进速度
            if (ulCurOutGoodsSpeed < WB01_OUTGOODS_RESET_SPEED)
            {
                ulCurOutGoodsSpeed += WB01_OUTGOODS_ACC_SPEED*WB01_OUTGOODS_PROC_TIME/1000;
                WB01_SetMainAxisMotorSpeed(ulCurOutGoodsSpeed);
                WB01_SetMainAxisMotorStatus(WB01_MOTOR_STATUS_ACW);
            }
        }
        
        break;
    case WB01_SELF_LEARN_STEP_DOWN_KEEP      :  //限位保持
        
        if (SysTime_CheckExpiredState(&m_SelfLearnCtrlTimer))
        {
            m_SelfLearnStep = WB01_SELF_LEARN_STEP_FINISH;
            SysTime_StartOneShot(&m_SelfLearnCtrlTimer, WB01_OUTGOODS_PROC_TIME);
        }
        
        break;
    case WB01_SELF_LEARN_STEP_FINISH         :  //结束处理
        
        //若柜号有效,则记录柜号
        if (m_lCurGridNumber)
        {
            m_lMaxGridCount = m_lCurGridNumber;
        }
        
        ulCurOutGoodsSpeed = 0;
        m_SelfLearnStep = WB01_SELF_LEARN_STEP_STOP;
        WB01_SetMainAxisMotorStatus(WB01_MOTOR_STATUS_STOP);
        
        m_CurOutGoodsStep = WB01_OUTGOODS_STEP_STOP;
        
        DEBUF_PRINT("WB01_SelfLearnHandler: Finish!\r\n");
        
        break;
    default: break;
    }
    
}


/**
  * @brief  出货复位任务开始
  * @param  None
  * @retval 0-成功 1-忙
  */
uBit32 WB01_StartOutGoodsResetTask(void)
{
    if ((m_SelfLearnStep != WB01_SELF_LEARN_STEP_STOP) ||
        (m_CurOutGoodsStep != WB01_OUTGOODS_STEP_STOP))
    {
        DEBUF_PRINT("WB01_StartOutGoodsResetTask: Err, System Busy!\r\n");
        return 1;
    }
    
    //设置出货流程状态
    m_CurOutGoodsStep = WB01_OUTGOODS_STEP_BUSY;
    
    //启动复位定时器
    SysTime_StartOneShot(&m_OutGoodsResetCtrlTimer, WB01_OUTGOODS_PROC_TIME);
    
    DEBUF_PRINT("WB01_StartOutGoodsResetTask: Success!\r\n");
    
    return 0;
}


/**
  * @brief  出货复位任务
  * @param  None
  * @retval None
  */
void WB01_OutGoodsResetHandler(void)
{
    if (SysTime_CheckExpiredState(&m_OutGoodsResetCtrlTimer))
    {
        SysTime_StartOneShot(&m_OutGoodsResetCtrlTimer, WB01_OUTGOODS_PROC_TIME);
        
        //若到达了下限位,则停止电机并将当前位置设置为零点
        if (GPIO_MAN_GetInputPinState(INPUT_IO_MAIN_AXIS_DOWN))
        {
            ulCurOutGoodsSpeed = 0;
            WB01_SetMainAxisMotorStatus(WB01_MOTOR_STATUS_STOP);
            m_CurOutGoodsStep = WB01_OUTGOODS_STEP_STOP;
            m_lCurGridNumber = 0;
            m_lObjGridNumber = 0;
            
            //清除定时器
            SysTime_Cancel(&m_OutGoodsResetCtrlTimer);
            
            DEBUF_PRINT("WB01_OutGoodsResetHandler: Down Limit Trigger And Stop Motor!\r\n");
            
            return;
        }
        
        //加速到快进速度
        if (ulCurOutGoodsSpeed < WB01_OUTGOODS_RESET_SPEED)
        {
            ulCurOutGoodsSpeed += WB01_OUTGOODS_ACC_SPEED*WB01_OUTGOODS_PROC_TIME/1000;
            WB01_SetMainAxisMotorSpeed(ulCurOutGoodsSpeed);
            WB01_SetMainAxisMotorStatus(WB01_MOTOR_STATUS_ACW);
        }
    }
    
}


/**
  * @brief  目标柜号设置
  * @param  ulGridNumber 目标柜号
  * @retval 0-成功  1-正在运行,设置失败  2-设置指超过最大柜号
  */
uBit32 WB01_SetObjGridNumber(uBit32 ulGridNumber)
{
    uBit32 ulRet = 0;
    
    //校验柜号是否有效
    if (ulGridNumber >= m_lMaxGridCount)
    {
        DEBUF_PRINT("WB01_SetObjGridNumber: Grid Number Over!\r\n");
        return 2;
    }
    
    //校验当前设备是否在忙
    if (m_CurOutGoodsStep != WB01_OUTGOODS_STEP_STOP)
    {
        DEBUF_PRINT("WB01_SetObjGridNumber: System Busy!\r\n");
        return 1;
    }
    
    //设置目标柜号
    m_lObjGridNumber = ulGridNumber;
    
    //设置当前工作步骤
    m_CurOutGoodsStep = WB01_OUTGOODS_STEP_START;
    
    DEBUF_PRINT("WB01_SetObjGridNumber: Success!\r\n");
    
    return ulRet;
}


/**
  * @brief  出货流程处理
  * @param  None
  * @retval None
  */
void WB01_OutGoodsHandler(void)
{
    switch (m_CurOutGoodsStep)
    {
    case WB01_OUTGOODS_STEP_STOP       : //停止
        break;
    case WB01_OUTGOODS_STEP_START      : //启动
        
        //设置启动速度
        ulCurOutGoodsSpeed = WB01_OUTGOODS_START_SPEED;
        WB01_SetMainAxisMotorSpeed(ulCurOutGoodsSpeed);
        
        //启动电机
        if (m_lCurGridNumber < m_lObjGridNumber)
        {
            DEBUF_PRINT("WB01_OutGoodsHandler: m_lCurGridNumber < m_lObjGridNumber, cw!\r\n");
            WB01_SetMainAxisMotorStatus(WB01_MOTOR_STATUS_CW);
        }
        else if (m_lCurGridNumber > m_lObjGridNumber)
        {
            DEBUF_PRINT("WB01_OutGoodsHandler: m_lCurGridNumber > m_lObjGridNumber, acw!\r\n");
            WB01_SetMainAxisMotorStatus(WB01_MOTOR_STATUS_ACW);
        }
        else 
        {
            DEBUF_PRINT("WB01_OutGoodsHandler: m_lCurGridNumber == m_lObjGridNumber, finish!\r\n");
            m_CurOutGoodsStep = WB01_OUTGOODS_STEP_FINISH;
            break;
        }
        
        //设置控制定时器
        SysTime_StartOneShot(&m_OutGoodsCtrlTimer, WB01_OUTGOODS_PROC_TIME);
        
        break;
    case WB01_OUTGOODS_STEP_SPEED_UP   : //加速
        
        if (SysTime_CheckExpiredState(&m_OutGoodsCtrlTimer))
        {
            SysTime_StartOneShot(&m_OutGoodsCtrlTimer, WB01_OUTGOODS_PROC_TIME);
            
            if (abs(m_lCurGridNumber - m_lObjGridNumber) == 0)
            {
                //假如当前位置为目标位置,则紧急停止电机,并执行相应的处理
                m_CurOutGoodsStep = WB01_OUTGOODS_STEP_FINISH;
                ulCurOutGoodsSpeed = 0;
                WB01_SetMainAxisMotorStatus(WB01_MOTOR_STATUS_STOP);
                
                DEBUF_PRINT("WB01_OutGoodsHandler: m_lCurGridNumber == m_lObjGridNumber, finish!\r\n");
            }
            else if (abs(m_lCurGridNumber - m_lObjGridNumber) <= WB01_OUTGOODS_SLOW_GRID_COUNT)
            {
                //假如当前格子跟目标格子相差小于风雨2格,则加速到慢进速度
                if (ulCurOutGoodsSpeed < WB01_OUTGOODS_SLOW_SPEED)
                {
                    ulCurOutGoodsSpeed += WB01_OUTGOODS_ACC_SPEED*WB01_OUTGOODS_PROC_TIME/1000;
                    WB01_SetMainAxisMotorSpeed(ulCurOutGoodsSpeed);
                }
                else 
                {
                    //考虑到存在速度大于慢进速度而小于快进速度时,走到目标格子数的2格以内的情况
                    //跳转到减速操作,而保证在最后2格是以慢进速度运行
                    m_CurOutGoodsStep = WB01_OUTGOODS_STEP_SPEED_DOWN;
                    DEBUF_PRINT("WB01_OutGoodsHandler: Run to slow speed!\r\n");
                }
            }
            else 
            {
                //假如当前格子跟目标格子相差大于2格,则加速到快进速度
                if (ulCurOutGoodsSpeed < WB01_OUTGOODS_FAST_SPEED)
                {
                    ulCurOutGoodsSpeed += WB01_OUTGOODS_ACC_SPEED*WB01_OUTGOODS_PROC_TIME/1000;
                    WB01_SetMainAxisMotorSpeed(ulCurOutGoodsSpeed);
                }
                else 
                {
                    m_CurOutGoodsStep = WB01_OUTGOODS_STEP_FAST_KEEP;
                    
                    DEBUF_PRINT("WB01_OutGoodsHandler: Keep fast speed!\r\n");
                }
            }
        }
        
        break;
    case WB01_OUTGOODS_STEP_FAST_KEEP  : //快进速度保持
        
        if (abs(m_lCurGridNumber - m_lObjGridNumber) == 0)
        {
            //假如当前位置为目标位置,则紧急停止电机,并执行相应的处理
            m_CurOutGoodsStep = WB01_OUTGOODS_STEP_FINISH;
            ulCurOutGoodsSpeed = 0;
            WB01_SetMainAxisMotorStatus(WB01_MOTOR_STATUS_STOP);
            
            DEBUF_PRINT("WB01_OutGoodsHandler: m_lCurGridNumber == m_lObjGridNumber, finish!\r\n");
        }
        else if (abs(m_lCurGridNumber - m_lObjGridNumber) <= WB01_OUTGOODS_SLOW_GRID_COUNT)
        {
            //假如当前格子跟目标格子相差小于风雨2格,则到达减速步骤
            m_CurOutGoodsStep = WB01_OUTGOODS_STEP_SPEED_DOWN;
            
            DEBUF_PRINT("WB01_OutGoodsHandler: Run to slow speed!\r\n");
            
            //设置控制间隔定时器
            SysTime_StartOneShot(&m_OutGoodsCtrlTimer, WB01_OUTGOODS_PROC_TIME);
        }
        
        break;
    case WB01_OUTGOODS_STEP_SPEED_DOWN : //减速
        
        if (SysTime_CheckExpiredState(&m_OutGoodsCtrlTimer))
        {
            SysTime_StartOneShot(&m_OutGoodsCtrlTimer, WB01_OUTGOODS_PROC_TIME);
            
            if (abs(m_lCurGridNumber - m_lObjGridNumber) == 0)
            {
                //假如当前位置为目标位置,则紧急停止电机,并执行相应的处理
                m_CurOutGoodsStep = WB01_OUTGOODS_STEP_FINISH;
                ulCurOutGoodsSpeed = 0;
                WB01_SetMainAxisMotorStatus(WB01_MOTOR_STATUS_STOP);
                
                DEBUF_PRINT("WB01_OutGoodsHandler: m_lCurGridNumber == m_lObjGridNumber, finish!\r\n");
            }
            else if (abs(m_lCurGridNumber - m_lObjGridNumber) <= WB01_OUTGOODS_SLOW_GRID_COUNT)
            {
                //假如当前格子跟目标格子相差小于风雨2格,则加速到慢进速度
                if (ulCurOutGoodsSpeed > WB01_OUTGOODS_SLOW_SPEED)
                {
                    ulCurOutGoodsSpeed -= WB01_OUTGOODS_ACC_SPEED*WB01_OUTGOODS_PROC_TIME/1000;
                    WB01_SetMainAxisMotorSpeed(ulCurOutGoodsSpeed);
                }
                else 
                {
                    m_CurOutGoodsStep = WB01_OUTGOODS_STEP_SLOW_KEEP;
                    
                    DEBUF_PRINT("WB01_OutGoodsHandler: Keep slow speed!\r\n");
                }
            }
        }
        
        break;
    case WB01_OUTGOODS_STEP_SLOW_KEEP  : //慢进速度保持
        
        //此步骤中,一直检测是否达到目标柜号,若到达,则停止
        if (abs(m_lCurGridNumber - m_lObjGridNumber) == 0)
        {
            m_CurOutGoodsStep = WB01_OUTGOODS_STEP_FINISH;
            ulCurOutGoodsSpeed = 0;
            WB01_SetMainAxisMotorStatus(WB01_MOTOR_STATUS_STOP);
            
            DEBUF_PRINT("WB01_OutGoodsHandler: m_lCurGridNumber == m_lObjGridNumber, finish!\r\n");
        }
        
        break;
    case WB01_OUTGOODS_STEP_FINISH     : //结束处理
        
        //清空相关变量
        m_lObjGridNumber = 0;
        m_CurOutGoodsStep = WB01_OUTGOODS_STEP_STOP;
        ulCurOutGoodsSpeed = 0;
        
        //清除定时器
        SysTime_Cancel(&m_OutGoodsCtrlTimer);
        
        //停止电机
        WB01_SetMainAxisMotorStatus(WB01_MOTOR_STATUS_STOP);
        
        DEBUF_PRINT("WB01_OutGoodsHandler: Finish Operation!\r\n");
        
        break;
        
    case WB01_OUTGOODS_STEP_BUSY: break;
        
    default: break;
    }
    
}


/*****************************************************************************
 * 霍尔传感器监控线程接口
 ****************************************************************************/
#define WB01_KEY_SCAN_INTERVAL       (20)       //按键扫描间隔(MS)
static  SYS_TIME_DATA m_KeyScanTimer = {1};     //扫描定时器

/**
  * @brief  霍尔传感器处理
  * @param  None
  * @retval None
  */
void WB01_HallSensorProc(void)
{
    if (SysTime_CheckExpiredState(&m_KeyScanTimer))
    {
        SysTime_StartOneShot(&m_KeyScanTimer, WB01_KEY_SCAN_INTERVAL);   //设置下一次执行的时间
        
        uBit32 ulKeyVlue = 0;
        uBit32 ulCurTrg = KEY_Scan(&ulKeyVlue);
        
        //识别到磁场变化
        if (ulCurTrg == 1)
        {
            //DEBUF_PRINT("Detect Hall Sensor Trigger\r\n");
            
            if (m_SelfLearnStep == WB01_SELF_LEARN_STEP_STOP)
            {
                if (m_uCurMotorDir == WB01_MOTOR_DIR_CW)
                {
                    m_lCurGridNumber++;
                    
                    if (m_lCurGridNumber >= m_lMaxGridCount)
                    {
                        
                        //报警
                    }
                    
                }
                else if (m_uCurMotorDir == WB01_MOTOR_DIR_ACW)
                {
                    m_lCurGridNumber--;
                    
                    if (m_lCurGridNumber < 0)
                    {
                        m_lCurGridNumber = 0;
                        
                        //报警
                    }
                }
                
                
            }
            else 
            {
                m_lCurGridNumber++;
            }
            
            
        }
    }
      
}


/*****************************************************************************
 * 进货门出货门电机控制线程接口
 ****************************************************************************/

#define WB01_INDOOR_USAGE                       (1)
#define WB01_OUTDOOR_USAGE                      (1)

//门工作状态定义
typedef enum
{
    DOOR_WORK_STATUS_STOP = 0,  //空闲
    DOOR_WORK_STATUS_OPEN ,     //开门操作
    DOOR_WORK_STATUS_CLOSE,     //关门操作
    
}DOOR_WORK_STATUS;


//电机当前运动状态定义
typedef enum
{
    MOTOR_CUR_STATUS_UNKNOW  = 0,   //未知(中间位置/超时导致的位置异常/信号异常导致的位置异常等)
    MOTOR_CUR_STATUS_RUNNING    ,   //运行中
    MOTOR_CUR_STATUS_OPEN_LIMIT ,   //在开门限位
    MOTOR_CUR_STATUS_CLOSE_LIMIT,   //在关门限位
    
}MOTOR_CUR_STATUS;

#define WB01_DOOR_OVER_TIME                     (10*1000)                       //超时时间
#define WB01_TRANSFER_MOTOR_STOP_DELAY_TIME     (5*1000)                        //传输皮带电机延迟关闭时间

static WB01_MOTOR_STATUS m_CurIndoorMotorStatus = WB01_MOTOR_STATUS_STOP;       //当前进货门电机状态
static WB01_MOTOR_STATUS m_CurOutdoorMotorStatus = WB01_MOTOR_STATUS_STOP;      //当前出货门电机状态

static DOOR_WORK_STATUS m_CurIndoorWorkStatus = DOOR_WORK_STATUS_STOP;          //当前进货门工作状态
static DOOR_WORK_STATUS m_CurOutdoorWorkStatus = DOOR_WORK_STATUS_STOP;         //当前出货门工作状态
        
static MOTOR_CUR_STATUS m_CurIndoorStatus = MOTOR_CUR_STATUS_UNKNOW;            //当前进货门执行状态
static MOTOR_CUR_STATUS m_CurOutdoorStatus = MOTOR_CUR_STATUS_UNKNOW;           //当前出货门执行状态
        
static SYS_TIME_DATA m_IndoorOverlTimer  = {0};                                 //入货门超时定时器
static SYS_TIME_DATA m_OutdoorOverlTimer  = {0};                                //出货门超时定时器
static SYS_TIME_DATA m_TransferMotorDelayTimer = {0};                           //传输皮带电机延时关闭定时器


/**
  * @brief  进货门电机控制
  * @param  uMotorStatus:
  *     @arg WB01_MOTOR_STATUS_STOP   停止
  *     @arg WB01_MOTOR_STATUS_CW     正转
  *     @arg WB01_MOTOR_STATUS_ACW    反转
  * @retval None
  */
void WB01_SetIndoorMotorStatus(uBit8 uMotorStatus)
{
#if WB01_INDOOR_USAGE
    switch (uMotorStatus)
    {
    case WB01_MOTOR_STATUS_STOP:
        DEBUF_PRINT("WB01_SetIndoorMotorStatus: Stop!\r\n");
        m_CurIndoorMotorStatus = WB01_MOTOR_STATUS_STOP;
        GPIO_SetOutputState(OUTPUT_IO_IN_DOOR_EN, false); 
        while (GPIO_GetOutputState(OUTPUT_IO_IN_DOOR_EN) != false);
        GPIO_SetOutputState(OUTPUT_IO_IN_DOOR_DIR, false); 
        while (GPIO_GetOutputState(OUTPUT_IO_IN_DOOR_DIR) != false);
        break;
    case WB01_MOTOR_STATUS_CW: 
        DEBUF_PRINT("WB01_SetIndoorMotorStatus: CW!\r\n");
        m_CurIndoorMotorStatus = WB01_MOTOR_STATUS_CW;
        GPIO_SetOutputState(OUTPUT_IO_IN_DOOR_DIR, false); 
        while (GPIO_GetOutputState(OUTPUT_IO_IN_DOOR_DIR) != false);
        GPIO_SetOutputState(OUTPUT_IO_IN_DOOR_EN, true); 
        while (GPIO_GetOutputState(OUTPUT_IO_IN_DOOR_EN) != true);
        break;
    case WB01_MOTOR_STATUS_ACW: 
        DEBUF_PRINT("WB01_SetIndoorMotorStatus: ACW!\r\n");
        m_CurIndoorMotorStatus = WB01_MOTOR_STATUS_ACW;
        GPIO_SetOutputState(OUTPUT_IO_IN_DOOR_DIR, true); 
        while (GPIO_GetOutputState(OUTPUT_IO_IN_DOOR_DIR) != true);
        GPIO_SetOutputState(OUTPUT_IO_IN_DOOR_EN, true); 
        while (GPIO_GetOutputState(OUTPUT_IO_IN_DOOR_EN) != true);
        break;
        
    default: break;
    }
#endif
}


/**
  * @brief  出货门电机控制
  * @param  uMotorStatus:
  *     @arg WB01_MOTOR_STATUS_STOP   停止
  *     @arg WB01_MOTOR_STATUS_CW     正转
  *     @arg WB01_MOTOR_STATUS_ACW    反转
  * @retval None
  */
void WB01_SetOutdoorMotorStatus(uBit8 uMotorStatus)
{
#if WB01_OUTDOOR_USAGE
    switch (uMotorStatus)
    {
    case WB01_MOTOR_STATUS_STOP:
        m_CurOutdoorMotorStatus = WB01_MOTOR_STATUS_STOP;
        GPIO_SetOutputState(OUTPUT_IO_OUT_DOOR_EN, false); 
        while (GPIO_GetOutputState(OUTPUT_IO_OUT_DOOR_EN) != false);
        GPIO_SetOutputState(OUTPUT_IO_OUT_DOOR_DIR, false); 
        while (GPIO_GetOutputState(OUTPUT_IO_OUT_DOOR_DIR) != false);
        break;
    case WB01_MOTOR_STATUS_CW: 
        m_CurOutdoorMotorStatus = WB01_MOTOR_STATUS_CW;
        GPIO_SetOutputState(OUTPUT_IO_OUT_DOOR_DIR, false); 
        while (GPIO_GetOutputState(OUTPUT_IO_OUT_DOOR_DIR) != false);
        GPIO_SetOutputState(OUTPUT_IO_OUT_DOOR_EN, true); 
        while (GPIO_GetOutputState(OUTPUT_IO_OUT_DOOR_EN) != true);
        break;
    case WB01_MOTOR_STATUS_ACW: 
        m_CurOutdoorMotorStatus = WB01_MOTOR_STATUS_ACW;
        GPIO_SetOutputState(OUTPUT_IO_OUT_DOOR_DIR, true); 
        while (GPIO_GetOutputState(OUTPUT_IO_OUT_DOOR_DIR) != true);
        GPIO_SetOutputState(OUTPUT_IO_OUT_DOOR_EN, true); 
        while (GPIO_GetOutputState(OUTPUT_IO_OUT_DOOR_EN) != true);
        break;
        
    default: break;
    }
#endif
    
}


/**
  * @brief  入货门状态控制
  * @param  bIsOpen 0-关门  1-开门
  * @retval None
  */
void WB01_SetIndoorStatus(bool bIsOpen)
{
#if WB01_INDOOR_USAGE
    if (bIsOpen)
    {
        DEBUF_PRINT("WB01_SetIndoorStatus: Open!\r\n");
        m_CurIndoorWorkStatus = DOOR_WORK_STATUS_OPEN;
    }
    else 
    {
        DEBUF_PRINT("WB01_SetIndoorStatus: Close!\r\n");
        m_CurIndoorWorkStatus = DOOR_WORK_STATUS_CLOSE;
    }
    
    switch (m_CurIndoorMotorStatus)
    {
    case  WB01_MOTOR_STATUS_STOP : DEBUF_PRINT("WB01_SetIndoorStatus: Current Motor Status Is STOP!\r\n"); break;
    case  WB01_MOTOR_STATUS_CW   : DEBUF_PRINT("WB01_SetIndoorStatus: Current Motor Status Is CW!\r\n");   break;
    case  WB01_MOTOR_STATUS_ACW  : DEBUF_PRINT("WB01_SetIndoorStatus: Current Motor Status Is ACW!\r\n");  break;
    }
    
    
    
    //设置当前状态
    m_CurIndoorStatus = MOTOR_CUR_STATUS_RUNNING;
    
    //设置超时报警定时器
    SysTime_StartOneShot(&m_IndoorOverlTimer, WB01_DOOR_OVER_TIME);
#endif
    
}


/**
  * @brief  入货门状态获取
  * @param  None
  * @retval 入货门状态
  *   @arg  MOTOR_CUR_STATUS_UNKNOW         //未知(中间位置,或超时/信号异常导致的位置异常等)
  *   @arg  MOTOR_CUR_STATUS_RUNNING        //运行中
  *   @arg  MOTOR_CUR_STATUS_OPEN_LIMIT     //在开门限位
  *   @arg  MOTOR_CUR_STATUS_CLOSE_LIMIT    //在关门限位
  */
uBit8 WB01_GetIndoorStatus(void)
{
#if WB01_INDOOR_USAGE
    return m_CurIndoorStatus;
#else 
    return MOTOR_CUR_STATUS_UNKNOW;
#endif
}


/**
  * @brief  入货门电机控制
  * @param  None
  * @retval None
  */
void WB01_IndoorHandler(void)
{
#if WB01_INDOOR_USAGE
    static uBit32 s_ulTmpValue = 0;
    
    switch (m_CurIndoorWorkStatus)
    {
    case DOOR_WORK_STATUS_STOP:
        
        break;
    case DOOR_WORK_STATUS_OPEN:
        
        //打开传输皮带电机
        GPIO_MAN_SetOutputPinState(OUTPUT_IO_TRANSF_MOTOR, true);
        
        //判断当前限位状态,若在限位处,则停止电机并退出
        if (GPIO_MAN_GetInputPinState(INPUT_IO_IN_DOOR_OPEN_LIMIT))
        {
            //设置当前状态
            m_CurIndoorStatus = MOTOR_CUR_STATUS_OPEN_LIMIT;
            
            //设置当前工作步骤
            m_CurIndoorWorkStatus = DOOR_WORK_STATUS_STOP;
            
            //停止电机
            WB01_SetIndoorMotorStatus(WB01_MOTOR_STATUS_STOP);
            
            //关闭超时计时器
            SysTime_Cancel(&m_IndoorOverlTimer);
            
            DEBUF_PRINT("WB01_IndoorHandler: Open Limit Signal Trigger, Stop!\r\n");
            
            break;
        }
        
        //判断当前电机工作状态
        switch (m_CurIndoorMotorStatus)
        {
        case WB01_MOTOR_STATUS_STOP: 
            //若停止电机2S以上,则开始正转
            s_ulTmpValue++;
            if (s_ulTmpValue >= 20)
            {
                WB01_SetIndoorMotorStatus(WB01_MOTOR_STATUS_CW);
                DEBUF_PRINT("WB01_IndoorHandler: Delay to CW!\r\n");
            }
            break;
            
            break;
        case WB01_MOTOR_STATUS_CW  :
            break;
        case WB01_MOTOR_STATUS_ACW :
            //如果当前电机是以相反的方向转动,则停止电机
            s_ulTmpValue = 0;
            WB01_SetIndoorMotorStatus(WB01_MOTOR_STATUS_STOP);
            DEBUF_PRINT("WB01_IndoorHandler: Stop ACW, And Delay to CW!\r\n");
            break;
        }
        
        break;
    case DOOR_WORK_STATUS_CLOSE:
        
        //判断当前限位状态,若在限位处,则停止电机并退出
        if (GPIO_MAN_GetInputPinState(INPUT_IO_IN_DOOR_CLOSE_LIMIT))
        {
            //设置当前工作步骤
            m_CurIndoorWorkStatus = DOOR_WORK_STATUS_STOP;
            
            //停止电机
            WB01_SetIndoorMotorStatus(WB01_MOTOR_STATUS_STOP);
            
            //设置当前状态
            m_CurIndoorStatus = MOTOR_CUR_STATUS_CLOSE_LIMIT;
            
            //设置传输皮带电机延时关闭定时器
            SysTime_StartOneShot(&m_TransferMotorDelayTimer, WB01_TRANSFER_MOTOR_STOP_DELAY_TIME);
            
            //关闭超时计时器
            SysTime_Cancel(&m_IndoorOverlTimer);
            
            DEBUF_PRINT("WB01_IndoorHandler: Close Limit Signal Trigger, Stop!\r\n");
            
            break;
        }
        
        //判断当前电机工作状态
        switch (m_CurIndoorMotorStatus)
        {
        case WB01_MOTOR_STATUS_STOP: 
            //若停止电机2S以上,则开始正转
            s_ulTmpValue++;
            if (s_ulTmpValue >= 20)
            {
                WB01_SetIndoorMotorStatus(WB01_MOTOR_STATUS_ACW);
                DEBUF_PRINT("WB01_IndoorHandler: Delay to ACW!\r\n");
            }
            break;
        case WB01_MOTOR_STATUS_CW  : 
            //如果当前电机是以相反的方向转动,则停止电机
            s_ulTmpValue = 0;
            WB01_SetIndoorMotorStatus(WB01_MOTOR_STATUS_STOP);
            DEBUF_PRINT("WB01_IndoorHandler: Stop CW, And Delay to ACW!\r\n");
            break;
        case WB01_MOTOR_STATUS_ACW :
            break;
        }
        
        break;
    }
    
    //超时处理
    if (SysTime_CheckExpiredState(&m_IndoorOverlTimer))
    {
        //设置相关状态
        m_CurIndoorWorkStatus = DOOR_WORK_STATUS_STOP;
        m_CurIndoorStatus = MOTOR_CUR_STATUS_UNKNOW;
        
        //停止电机
        WB01_SetIndoorMotorStatus(WB01_MOTOR_STATUS_STOP);
        
        DEBUF_PRINT("WB01_IndoorHandler: Over Time, Stop!\r\n");
    }
    
    //传输皮带电机关闭
    if (SysTime_CheckExpiredState(&m_TransferMotorDelayTimer))
    {
        GPIO_MAN_SetOutputPinState(OUTPUT_IO_TRANSF_MOTOR, false);
        
        DEBUF_PRINT("WB01_IndoorHandler: Close Transfer Motor!\r\n");
    }
    
#endif
}


/**
  * @brief  出货门状态控制
  * @param  bIsOpen 0-关门  1-开门
  * @retval None
  */
void WB01_SetOutdoorStatus(bool bIsOpen)
{
#if WB01_OUTDOOR_USAGE
    
    if (bIsOpen)
    {
        m_CurOutdoorWorkStatus = DOOR_WORK_STATUS_OPEN;
    }
    else 
    {
        m_CurOutdoorWorkStatus = DOOR_WORK_STATUS_CLOSE;
    }
    
    //设置当前状态
    m_CurOutdoorStatus = MOTOR_CUR_STATUS_RUNNING;
    
    //设置超时报警定时器
    SysTime_StartOneShot(&m_OutdoorOverlTimer, WB01_DOOR_OVER_TIME);
#endif
    
}


/**
  * @brief  出货门状态获取
  * @param  None
  * @retval 出货门状态
  *   @arg  MOTOR_CUR_STATUS_UNKNOW         //未知(中间位置,或超时/信号异常导致的位置异常等)
  *   @arg  MOTOR_CUR_STATUS_RUNNING        //运行中
  *   @arg  MOTOR_CUR_STATUS_OPEN_LIMIT     //在开门限位
  *   @arg  MOTOR_CUR_STATUS_CLOSE_LIMIT    //在关门限位
  */
uBit8 WB01_GetOutdoorStatus(void)
{
#if WB01_OUTDOOR_USAGE
    return m_CurOutdoorStatus;
#else 
    return MOTOR_CUR_STATUS_UNKNOW;
#endif
}


/**
  * @brief  出货门电机控制
  * @param  None
  * @retval None
  */
void WB01_OutdoorHandler(void)
{
#if WB01_OUTDOOR_USAGE
    static uBit32 s_ulTmpValue = 0;
    
    switch (m_CurOutdoorWorkStatus)
    {
    case DOOR_WORK_STATUS_STOP:
        
        break;
    case DOOR_WORK_STATUS_OPEN:
        
        //判断当前限位状态,若在限位处,则停止电机并退出
        if (GPIO_MAN_GetInputPinState(INPUT_IO_OUT_DOOR_OPEN_LIMIT))
        {
            //设置当前状态
            m_CurOutdoorStatus = MOTOR_CUR_STATUS_OPEN_LIMIT;
            
            //设置当前工作步骤
            m_CurOutdoorWorkStatus = DOOR_WORK_STATUS_STOP;
            
            //停止电机
            WB01_SetOutdoorMotorStatus(WB01_MOTOR_STATUS_STOP);
            
            //关闭超时计时器
            SysTime_Cancel(&m_OutdoorOverlTimer);
            
            break;
        }
        
        //判断当前电机工作状态
        switch (m_CurOutdoorMotorStatus)
        {
        case WB01_MOTOR_STATUS_STOP: 
            //若停止电机2S以上,则开始正转
            s_ulTmpValue++;
            if (s_ulTmpValue >= 20)
            {
                s_ulTmpValue = 0;
                WB01_SetOutdoorMotorStatus(WB01_MOTOR_STATUS_CW);
            }
            break;
            
            break;
        case WB01_MOTOR_STATUS_CW  :
            break;
        case WB01_MOTOR_STATUS_ACW :
            //如果当前电机是以相反的方向转动,则停止电机
            s_ulTmpValue = 0;
            WB01_SetOutdoorMotorStatus(WB01_MOTOR_STATUS_STOP);
            break;
        }
        
        break;
    case DOOR_WORK_STATUS_CLOSE:
        
        //判断当前限位状态,若在限位处,则停止电机并退出
        if (GPIO_MAN_GetInputPinState(INPUT_IO_OUT_DOOR_CLOSE_LIMIT))
        {
            //设置当前工作步骤
            m_CurOutdoorWorkStatus = DOOR_WORK_STATUS_STOP;
            
            //停止电机
            WB01_SetOutdoorMotorStatus(WB01_MOTOR_STATUS_STOP);
            
            //设置当前状态
            m_CurOutdoorStatus = MOTOR_CUR_STATUS_CLOSE_LIMIT;
            
            //关闭超时计时器
            SysTime_Cancel(&m_OutdoorOverlTimer);
            
            break;
        }
        
        //判断红外检测信号,若在关门的过程中,检测到有货物,则停止关门并重新开门
        if (GPIO_MAN_GetInputPinState(INPUT_IO_GOODS_DECTECT))
        {
            m_CurOutdoorWorkStatus = DOOR_WORK_STATUS_OPEN;
            
            //重新进行超时计时
            SysTime_StartOneShot(&m_OutdoorOverlTimer, WB01_DOOR_OVER_TIME);
            
            break;
        }
        
        //判断当前电机工作状态
        switch (m_CurOutdoorMotorStatus)
        {
        case WB01_MOTOR_STATUS_STOP: 
            //若停止电机2S以上,则开始正转
            s_ulTmpValue++;
            if (s_ulTmpValue >= 20)
            {
                s_ulTmpValue = 0;
                WB01_SetOutdoorMotorStatus(WB01_MOTOR_STATUS_ACW);
            }
            break;
        case WB01_MOTOR_STATUS_CW  : 
            //如果当前电机是以相反的方向转动,则停止电机
            s_ulTmpValue = 0;
            WB01_SetOutdoorMotorStatus(WB01_MOTOR_STATUS_STOP);
            break;
        case WB01_MOTOR_STATUS_ACW :
            break;
        }
        
        break;
    }
    
    //超时处理
    if (SysTime_CheckExpiredState(&m_OutdoorOverlTimer))
    {
        //设置相关状态
        m_CurOutdoorWorkStatus = DOOR_WORK_STATUS_STOP;
        m_CurOutdoorStatus = MOTOR_CUR_STATUS_UNKNOW;
        
        //停止电机
        WB01_SetOutdoorMotorStatus(WB01_MOTOR_STATUS_STOP);
    }
#endif
    
}


/*****************************************************************************
 * 货道限位IO检测线程接口
 ****************************************************************************/

#if 0

MH1，MH2，MH3，MV1以及B1

H低电平而V高电平时,启动货道电机
当限位信号变成下降沿时,到达限位

#endif

//定义货道电机资源
#define WB01_AISLE_MAX_ROW                      (1)             //货道最大行数
#define WB01_AISLE_MAX_COL                      (3)             //货道最大列数

//货道电机限位信号电平状态
#define WB01_AISLE_INDEX_SIGNAL_VALID           (false)         //限位信号有效信号
#define WB01_AISLE_INDEX_SIGNAL_INVALID         (true)          //限位信号无效信号

//货道电机时间参数定义
#define WB01_AISLE_MOTOR_DELAY_TIME             (1500)          //货道电机检测延时时间(MS)
#define WB01_AISLE_MOTOR_OVER_TIME              (4000)          //货道电机超时时间

static SYS_TIME_DATA m_AisleDelayTimer = {0};                   //货道电机检测延时定时器
static SYS_TIME_DATA m_AisleOverTimer = {0};                    //货道电机超时定时器

static uBit32 m_ulCurRow = 0xFF;                                //当前行
static uBit32 m_ulCurCol = 0xFF;                                //当前列
static bool m_bIndexInvalidFlag = false;                        //限位信号无效标志

static WB01_AISLE_MOTOR_STATUS m_vm_AisleMotorRunningStatus = WB01_AISLE_MOTOR_STATUS_IDLE; //货道电机运行状态

static uBit16 SaleAinStart[6] = {0x003e, 0x003d, 0x003b, 0x0037, 0x002f, 0x001f}; //层数 L0-L5
static uBit16 MotorNumStart[10] = {0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000, 0x0040, 0x0080}; //电机m0-m9


/**
  * @brief  货道电机设置
  * @param  ulRow 行号,从0算起
  * @param  bState 信号状态
  * @retval None
  */
void WB01_SetAsileMotor(uBit32 ulRow, uBit32 ulCol, bool bState)
{
    uBit16 nTmpVlue = 0;
    
    if ((ulRow >= WB01_AISLE_MAX_ROW) || (ulCol >= WB01_AISLE_MAX_COL))
    {
        return;
    }
    
    if (bState)
    {
        nTmpVlue = (SaleAinStart[ulRow] | MotorNumStart[ulCol]);
    }
    else 
    {
        nTmpVlue = 0x003F;
    }
    
    HC595_SetData(nTmpVlue);
    
}


/**
  * @brief  货道电机行限位信号获取
  * @param  ulRow 行号,从0算起
  * @retval 信号状态
  */
static bool WB01_GetAisleIndexSignal(uBit32 ulRow)
{
    
    return GPIO_MAN_GetInputPinState(INPUT_IO_CH_MOTOR_LIMIT);
}


/**
  * @brief  货道电机启动
  * @param  ulRow 行号,从0算起
  * @param  ulCol 列号,从0算起
  * @retval 0-成功 1-货道电机正在运行 2-参数范围错误
  */
uBit32 WB01_EnableAisleMotor(uBit32 ulRow, uBit32 ulCol)
{
    //校验入参参数
    if ((ulRow >= WB01_AISLE_MAX_ROW) || (ulCol >= WB01_AISLE_MAX_COL))
    {
        return 2;
    }
    
    //判断电机是否在运行
    if (m_vm_AisleMotorRunningStatus == WB01_AISLE_MOTOR_STATUS_RUNNING)
    {
        return 1;
    }
    
    m_ulCurRow = ulRow;  //行
    m_ulCurCol = ulCol;  //列
    m_bIndexInvalidFlag = false;
    
    //使能货道电机行信号
    WB01_SetAsileMotor(ulRow, ulCol, true);
    
    //设置电机运行标志位
    m_vm_AisleMotorRunningStatus = WB01_AISLE_MOTOR_STATUS_RUNNING;
    
    //启动检测延时定时器
    SysTime_Start(&m_AisleDelayTimer, WB01_AISLE_MOTOR_DELAY_TIME);
    
    //设置超时报警定时器
    SysTime_StartOneShot(&m_AisleOverTimer, WB01_AISLE_MOTOR_OVER_TIME);
    
    return 0;
}
     

/**
  * @brief  货道电机运行状态获取
  * @param  None
  * @retval 货道电机运行状态
  *   @arg  WB01_AISLE_MOTOR_STATUS_IDLE     货道电机空闲
  *         WB01_AISLE_MOTOR_STATUS_RUNNING  货道电机运行中
  *         WB01_AISLE_MOTOR_STATUS_OVER     超时
  *         WB01_AISLE_MOTOR_STATUS_POS_ERR  限位信号异常
  */
uBit32 WB01_GetAisleMotorRunningState(void)
{
    
    return m_vm_AisleMotorRunningStatus;
}


/**
  * @brief  货道电机管理
  * @param  None
  * @retval None
  */
void WB01_AisleMotorHandler(void)
{
    if (m_vm_AisleMotorRunningStatus == WB01_AISLE_MOTOR_STATUS_RUNNING)
    {
        //限位信号延时检测
        if (SysTime_CheckExpiredState(&m_AisleDelayTimer))
        {
            //检测限位信号
            if (WB01_GetAisleIndexSignal(m_ulCurRow) == WB01_AISLE_INDEX_SIGNAL_VALID)
            {
                if (m_bIndexInvalidFlag)
                {
                    //关闭货道电机输出
                    WB01_SetAsileMotor(m_ulCurRow, m_ulCurCol, 0);
                    
                    //初始化行列号
                    m_ulCurRow = 0xFF;
                    m_ulCurCol = 0xFF;
                    m_bIndexInvalidFlag = false;
                    
                    //关闭定时器
                    SysTime_Cancel(&m_AisleDelayTimer);
                    SysTime_Cancel(&m_AisleOverTimer);
                    
                    m_vm_AisleMotorRunningStatus = WB01_AISLE_MOTOR_STATUS_IDLE;
                }
            }
            else 
            {
                m_bIndexInvalidFlag = true;
            }
            
        }
        
        //超时报警检测
        if (SysTime_CheckExpiredState(&m_AisleOverTimer))
        {
            //关闭货道电机输出
            WB01_SetAsileMotor(m_ulCurRow, m_ulCurCol, 0);
            
            //初始化行列号
            m_ulCurRow = 0xFF;
            m_ulCurCol = 0xFF;
            m_bIndexInvalidFlag = false;
            
            //关闭定时器
            SysTime_Cancel(&m_AisleDelayTimer);
            SysTime_Cancel(&m_AisleOverTimer);
            
            m_vm_AisleMotorRunningStatus = WB01_AISLE_MOTOR_STATUS_OVER;
        }
        
    }

}


/*****************************************************************************
 * 测试线程接口
 ****************************************************************************/

#define WB01_TEST_INTERVAL       (2000)     //按键扫描间隔(MS)
static  SYS_TIME_DATA m_TestTimer = {1};    //测试定时器


/**
  * @brief  测试处理
  * @param  None
  * @retval None
  */
void WB01_TestHandler(void)
{
    if (SysTime_CheckExpiredState(&m_TestTimer))
    {
        SysTime_StartOneShot(&m_TestTimer, WB01_TEST_INTERVAL);   //设置下一次执行的时间
        
#if 0
        if (ulCurOutGoodsSpeed < 36*1000)
        {
            //设置启动速度
            WB01_SetMainAxisMotorStatus(WB01_MOTOR_STATUS_CW);
            ulCurOutGoodsSpeed += 200;
            WB01_SetMainAxisMotorSpeed(ulCurOutGoodsSpeed);
            
            //打印输出信息
            uBit8 uDisBuff[64] = {0};
            sprintf((char *)uDisBuff, "Cur Frequency: %d\r\n", ulCurOutGoodsSpeed);
            DEBUF_PRINT(uDisBuff);
        }
#endif
    }
    
}





