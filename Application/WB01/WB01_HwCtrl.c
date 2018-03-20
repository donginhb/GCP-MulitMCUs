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

//#define WB01_DEBUG

#ifdef WB01_DEBUG
#define DEBUF_PRINT(x)      UART_SendStr(WB01_DEBUG_UART_NODE, x)
//#define DEBUF_PRINT(x)      UART_SendStr(WB01_COM_UART_NODE, x)
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
    PWM_SetOutputPwmFrq(WB01_MOTOR_PWM_NODE, 2000);
    PWM_OutputEnable(WB01_MOTOR_PWM_NODE, true);
    
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

//主轴电机运行状态定义
#define WB01_MOTOR_STATUS_STOP              (0) //停止
#define WB01_MOTOR_STATUS_CW                (1) //正转
#define WB01_MOTOR_STATUS_ACW               (2) //反转

//主轴电机当前运动方向定义
#define WB01_MOTOR_DIR_CW                   (0) //正转方向
#define WB01_MOTOR_DIR_ACW                  (1) //逆转方向

//#define WB01_MAIN_AXIS_GRID_TIMEROVER       (3000)  //单格超时时间

#define WB01_MAIN_START_SPEED               (200)   //启动速度
#define WB01_MAIN_FAST_SPEED                (1000)  //快进速度
#define WB01_MAIN_SLOW_SPEED                (500)   //慢进速度

static uBit8 m_uCurMotorDir = WB01_MOTOR_DIR_CW;    //当前电机运动方向 0-正转 1-逆转
static uBit8 m_uCurAxisMotorStatus = 0;             //当前主轴电机状态 0-停止 1-正转 2-反转


/**
  * @brief  主轴电机状态设置
  * @param  uMotorStatus:
  *     @arg WB01_MOTOR_STATUS_STOP   停止
  *     @arg WB01_MOTOR_STATUS_CW     正转
  *     @arg WB01_MOTOR_STATUS_ACW    反转
  *     @arg WB01_MOTOR_STATUS_ESTOP  刹车
  * @retval None
  * @Note   禁止在同一时间内,出现A/B都为高的情况.由于IO操作的异步性(设置IO寄存器后,
  *         需要一定时间才会发生电平变化),所以在改变IO电平后,需要等待其电平发生实
  *         际改变后才执行一下步的操作,以避免这种情况出现;
  *         除此之外,必须先执行拉低电平的操作,再执行拉高电平的操作;
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

#define WB01_OUTGOODS_PROC_TIME     (100)           //出货流程监控时间(MS)
static SYS_TIME_DATA m_OutGoodsCtrlTimer = {1};     //出货流程控制定时器

Bit32 g_lMaxGridCount = 25;         //最大的格子数
Bit32 g_lCurGridNumber = 0;         //当前转过的格子
Bit32 g_lObjGridNumber = 0;         //要出货的格子数(目标格子数)

//bool g_bMainAxisMotorRunningFlag = false;   //主轴电机运行状态


#if 0
/**
  * @brief  目标柜号设置
  * @param  ulGridNumber 目标柜号
  * @retval 0-成功  1-正在运行,设置失败  2-设置指超过最大柜号
  */
uBit32 WB01_SetObjGridNumber(uBit32 ulGridNumber)
{
    uBit32 ulRet = 1;
    
    if (ulGridNumber >= g_lMaxGridCount)
    {
        return 2;
    }
    
    if (!g_bMainAxisMotorRunningFlag)
    {
        //设置主轴电机运行标志
        g_bMainAxisMotorRunningFlag = true;
        
        //设置目标柜号
        g_lObjGridNumber = ulGridNumber;
        
        ulRet = 0;
    }
    
    return ulRet;
}
#endif


/**
  * @brief  目标柜号获取
  * @param  None
  * @retval 目标柜号
  */
uBit32 WB01_GetObjGridNumber(void)
{
    
    return g_lObjGridNumber;
}


/**
  * @brief  总柜数获取
  * @param  None
  * @retval 总柜数
  */
uBit32 WB01_GetMaxGridCount(void)
{
    
    return g_lMaxGridCount;
}


#if 0
/**
  * @brief  出货流程处理
  * @param  None
  * @retval None
  */
void WB01_OutGoodsHandler(void)
{
    static uBit32 s_ulTmpValue = 0;
    
    if (SysTime_CheckExpiredState(&m_OutGoodsCtrlTimer))
    {
        SysTime_StartOneShot(&m_OutGoodsCtrlTimer, WB01_OUTGOODS_PROC_TIME); //设置下一次执行的时间
        
        if (g_bMainAxisMotorRunningFlag)
        {

            //假如当前柜号小于目标柜号,则电机正转
            if (g_lCurGridNumber < g_lObjGridNumber)
            {
                //获取主轴电机状态
                uBit8 uMotorStatus = WB01_GetMainAxisMotorStatus();
                
                switch (uMotorStatus)
                {
                case WB01_MOTOR_STATUS_STOP : 
                    //若停止电机2S以上,则开始正转
                    s_ulTmpValue++;
                    if (s_ulTmpValue >= 20)
                    {
                        DEBUF_PRINT("motor: current is stop , cw\r\n");
                        WB01_SetMainAxisMotorStatus(WB01_MOTOR_STATUS_CW);
                    }
                    break;
                case WB01_MOTOR_STATUS_CW   : break;
                case WB01_MOTOR_STATUS_ACW  : 
                    //假如当前在逆转,则急停电机
                    s_ulTmpValue = 0;
                    DEBUF_PRINT("motor: current is acw , estop\r\n");
                    WB01_SetMainAxisMotorStatus(WB01_MOTOR_STATUS_ESTOP);
                    break;
                case WB01_MOTOR_STATUS_ESTOP: 
                    //若急停电机2S以上,则开始正转
                    s_ulTmpValue++;
                    if (s_ulTmpValue >= 20)
                    {
                        DEBUF_PRINT("motor: current is estop , cw\r\n");
                        WB01_SetMainAxisMotorStatus(WB01_MOTOR_STATUS_CW);
                    }
                    break;
                }
                    
            }
            //假如当前柜号大于目标柜号,则电机逆转
            else if (g_lCurGridNumber > g_lObjGridNumber)
            {
                //获取主轴电机状态
                uBit8 uMotorStatus = WB01_GetMainAxisMotorStatus();
                
                switch (uMotorStatus)
                {
                case WB01_MOTOR_STATUS_STOP : 
                    //若停止电机2S以上,则开始逆转
                    s_ulTmpValue++;
                    if (s_ulTmpValue >= 20)
                    {
                        DEBUF_PRINT("motor: current is stop , acw\r\n");
                        WB01_SetMainAxisMotorStatus(WB01_MOTOR_STATUS_ACW);
                    }
                    break;
                case WB01_MOTOR_STATUS_CW   : 
                    //假如当前在正转,则急停电机
                    s_ulTmpValue = 0;
                    DEBUF_PRINT("motor: current is cw , estop\r\n");
                    WB01_SetMainAxisMotorStatus(WB01_MOTOR_STATUS_ESTOP);
                case WB01_MOTOR_STATUS_ACW  : 
                    break;
                case WB01_MOTOR_STATUS_ESTOP: 
                    //若急停电机2S以上,则开始逆转
                    s_ulTmpValue++;
                    if (s_ulTmpValue >= 20)
                    {
                        DEBUF_PRINT("motor: current is estop , acw\r\n");
                        WB01_SetMainAxisMotorStatus(WB01_MOTOR_STATUS_ACW);
                    }
                    break;
                }
                
            }
            //假如当前柜号和目标柜号相同,则停止电机
            else if (g_lCurGridNumber == g_lObjGridNumber)
            {
                DEBUF_PRINT("g_lCurGridNumber == g_lObjGridNumber, estop!\r\n");
                
                //刹车
                WB01_SetMainAxisMotorStatus(WB01_MOTOR_STATUS_ESTOP);
                
                //清除运行状态
                g_bMainAxisMotorRunningFlag = false;
            }

            
        }
    }

}
#else 


#define WB01_MAIN_START_SPEED               (200)   //启动速度
#define WB01_MAIN_FAST_SPEED                (1000)  //快进速度
#define WB01_MAIN_SLOW_SPEED                (500)   //慢进速度


typedef enum
{
    WB01_OUTGOODS_STEP_STOP = 0,        //停止
    WB01_OUTGOODS_STEP_START,           //启动
    WB01_OUTGOODS_STEP_SPEED_UP,        //加速
    WB01_OUTGOODS_STEP_FAST_KEEP,       //快进速度保持
    WB01_OUTGOODS_STEP_SPEED_DOWN,      //减速
    WB01_OUTGOODS_STEP_SLOW_KEEP,       //慢进速度保持
    WB01_OUTGOODS_STEP_FINISH,          //结束处理
    
}WB01_OUTGOODS_STEP;


static WB01_OUTGOODS_STEP m_OutGoodsStep = 0;


/**
  * @brief  目标柜号设置
  * @param  ulGridNumber 目标柜号
  * @retval 0-成功  1-正在运行,设置失败  2-设置指超过最大柜号
  */
uBit32 WB01_SetObjGridNumber(uBit32 ulGridNumber)
{
    uBit32 ulRet = 0;
    
    //校验柜号是否有效
    if (ulGridNumber >= g_lMaxGridCount)
    {
        return 2;
    }
    
    //校验当前设备是否在忙
    if (m_OutGoodsStep != WB01_OUTGOODS_STEP_STOP)
    {
        return 1;
    }
    
    //设置目标柜号
    g_lObjGridNumber = ulGridNumber;
    
    //设置当前工作步骤
    m_OutGoodsStep = WB01_OUTGOODS_STEP_START;
    
    return ulRet;
}


/**
  * @brief  出货流程处理
  * @param  None
  * @retval None
  */
void WB01_OutGoodsHandler(void)
{
    switch (m_OutGoodsStep)
    {
    case WB01_OUTGOODS_STEP_STOP       : //停止            
        break;
    case WB01_OUTGOODS_STEP_START      : //启动            
        break;
    case WB01_OUTGOODS_STEP_SPEED_UP   : //加速            
        break;
    case WB01_OUTGOODS_STEP_FAST_KEEP  : //快进速度保持    
        break;
    case WB01_OUTGOODS_STEP_SPEED_DOWN : //减速            
        break;
    case WB01_OUTGOODS_STEP_SLOW_KEEP  : //慢进速度保持    
        break;
    case WB01_OUTGOODS_STEP_FINISH     : //结束处理        
        break;
    default: break;
    }
    
}

#endif


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
            DEBUF_PRINT("Detect Hall Sensor Trigger\r\n");
            if (m_uCurMotorDir == WB01_MOTOR_DIR_CW)
            {
                g_lCurGridNumber++;
                
                if (g_lCurGridNumber >= g_lMaxGridCount)
                {
                    
                    //报警
                }
                
            }
            else if (m_uCurMotorDir == WB01_MOTOR_DIR_ACW)
            {
                g_lCurGridNumber--;
                
                if (g_lCurGridNumber < 0)
                {
                    g_lCurGridNumber = 0;
                    
                    //报警
                }
                
            }
            
        }
    }
      
}


/*****************************************************************************
 * 进货门出货门电机控制线程接口
 ****************************************************************************/

#define WB01_INDOOR_USAGE                       (1)
#define WB01_OUTDOOR_USAGE                      (1)

//电机运行状态定义
//#define WB01_MOTOR_STATUS_STOP                  (0)     //停止
//#define WB01_MOTOR_STATUS_CW                    (1)     //正转
//#define WB01_MOTOR_STATUS_ACW                   (2)     //反转

//门工作状态定义
#define DOOR_WORK_STATUS_STOP                   (0)     //空闲
#define DOOR_WORK_STATUS_OPEN                   (1)     //开门操作
#define DOOR_WORK_STATUS_CLOSE                  (2)     //关门操作

//电机当前运动状态定义
#define MOTOR_CUR_STATUS_UNKNOW                 (0)     //未知(中间位置/超时导致的位置异常/信号异常导致的位置异常等)
#define MOTOR_CUR_STATUS_RUNNING                (1)     //运行中
#define MOTOR_CUR_STATUS_OPEN_LIMIT             (2)     //在开门限位
#define MOTOR_CUR_STATUS_CLOSE_LIMIT            (3)     //在关门限位

#define WB01_DOOR_OVER_TIME                     (10*1000)   //超时时间

static uBit8 m_uCurIndoorMotorStatus = 0;                       //当前进货门电机状态
static uBit8 m_uCurOutdoorMotorStatus = 0;                      //当前出货门电机状态

static uBit8 m_uCurIndoorWorkStatus = DOOR_WORK_STATUS_STOP;    //当前进货门工作状态
static uBit8 m_uCurOutdoorWorkStatus = DOOR_WORK_STATUS_STOP;   //当前出货门工作状态

static uBit8 m_uCurIndoorStatus = MOTOR_CUR_STATUS_UNKNOW;      //当前进货门执行状态
static uBit8 m_uCurOutdoorStatus = MOTOR_CUR_STATUS_UNKNOW;     //当前出货门执行状态

static SYS_TIME_DATA m_IndoorOverlTimer  = {1};                 //入货门超时定时器
static SYS_TIME_DATA m_OutdoorOverlTimer  = {1};                //入货门超时定时器



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
        m_uCurIndoorMotorStatus = WB01_MOTOR_STATUS_STOP;
        GPIO_SetOutputState(OUTPUT_IO_IN_DOOR_EN, false); 
        while (GPIO_GetOutputState(OUTPUT_IO_IN_DOOR_EN) != false);
        GPIO_SetOutputState(OUTPUT_IO_IN_DOOR_DIR, false); 
        while (GPIO_GetOutputState(OUTPUT_IO_IN_DOOR_DIR) != false);
        break;
    case WB01_MOTOR_STATUS_CW: 
        m_uCurIndoorMotorStatus = WB01_MOTOR_STATUS_CW;
        GPIO_SetOutputState(OUTPUT_IO_IN_DOOR_DIR, false); 
        while (GPIO_GetOutputState(OUTPUT_IO_IN_DOOR_DIR) != false);
        GPIO_SetOutputState(OUTPUT_IO_IN_DOOR_EN, true); 
        while (GPIO_GetOutputState(OUTPUT_IO_IN_DOOR_EN) != true);
        break;
    case WB01_MOTOR_STATUS_ACW: 
        m_uCurIndoorMotorStatus = WB01_MOTOR_STATUS_ACW;
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
        m_uCurOutdoorMotorStatus = WB01_MOTOR_STATUS_STOP;
        GPIO_SetOutputState(OUTPUT_IO_OUT_DOOR_EN, false); 
        while (GPIO_GetOutputState(OUTPUT_IO_OUT_DOOR_EN) != false);
        GPIO_SetOutputState(OUTPUT_IO_OUT_DOOR_DIR, false); 
        while (GPIO_GetOutputState(OUTPUT_IO_OUT_DOOR_DIR) != false);
        break;
    case WB01_MOTOR_STATUS_CW: 
        m_uCurOutdoorMotorStatus = WB01_MOTOR_STATUS_CW;
        GPIO_SetOutputState(OUTPUT_IO_OUT_DOOR_DIR, false); 
        while (GPIO_GetOutputState(OUTPUT_IO_OUT_DOOR_DIR) != false);
        GPIO_SetOutputState(OUTPUT_IO_OUT_DOOR_EN, true); 
        while (GPIO_GetOutputState(OUTPUT_IO_OUT_DOOR_EN) != true);
        break;
    case WB01_MOTOR_STATUS_ACW: 
        m_uCurOutdoorMotorStatus = WB01_MOTOR_STATUS_ACW;
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
        m_uCurIndoorWorkStatus = DOOR_WORK_STATUS_OPEN;
    }
    else 
    {
        m_uCurIndoorWorkStatus = DOOR_WORK_STATUS_CLOSE;
    }
    
    //设置当前状态
    m_uCurIndoorStatus = MOTOR_CUR_STATUS_RUNNING;
    
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
    return m_uCurIndoorStatus;
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
    
    switch (m_uCurIndoorWorkStatus)
    {
    case DOOR_WORK_STATUS_STOP:
        
        break;
    case DOOR_WORK_STATUS_OPEN:
        
        //判断当前限位状态,若在限位处,则停止电机并退出
        if (GPIO_MAN_GetInputPinState(INPUT_IO_IN_DOOR_OPEN_LIMIT))
        {
            //设置当前状态
            m_uCurIndoorStatus = MOTOR_CUR_STATUS_OPEN_LIMIT;
            
            //设置当前工作步骤
            m_uCurIndoorWorkStatus = DOOR_WORK_STATUS_STOP;
            
            //停止电机
            WB01_SetIndoorMotorStatus(WB01_MOTOR_STATUS_STOP);
            
            break;
        }
        
        //判断当前电机工作状态
        switch (m_uCurIndoorMotorStatus)
        {
        case WB01_MOTOR_STATUS_STOP: 
            //若停止电机2S以上,则开始正转
            s_ulTmpValue++;
            if (s_ulTmpValue >= 20)
            {
                WB01_SetIndoorMotorStatus(WB01_MOTOR_STATUS_CW);
            }
            break;
            
            break;
        case WB01_MOTOR_STATUS_CW  :
            break;
        case WB01_MOTOR_STATUS_ACW :
            //如果当前电机是以相反的方向转动,则停止电机
            s_ulTmpValue = 0;
            WB01_SetIndoorMotorStatus(WB01_MOTOR_STATUS_STOP);
            break;
        }
        
        break;
    case DOOR_WORK_STATUS_CLOSE:
        
        //判断当前限位状态,若在限位处,则停止电机并退出
        if (GPIO_MAN_GetInputPinState(INPUT_IO_IN_DOOR_CLOSE_LIMIT))
        {
            //设置当前工作步骤
            m_uCurIndoorWorkStatus = DOOR_WORK_STATUS_STOP;
            
            //停止电机
            WB01_SetIndoorMotorStatus(WB01_MOTOR_STATUS_STOP);
            
            //设置当前状态
            m_uCurIndoorStatus = MOTOR_CUR_STATUS_CLOSE_LIMIT;
            
            break;
        }
        
        //判断当前电机工作状态
        switch (m_uCurIndoorMotorStatus)
        {
        case WB01_MOTOR_STATUS_STOP: 
            //若停止电机2S以上,则开始正转
            s_ulTmpValue++;
            if (s_ulTmpValue >= 20)
            {
                
                WB01_SetIndoorMotorStatus(WB01_MOTOR_STATUS_ACW);
            }
            break;
        case WB01_MOTOR_STATUS_CW  : 
            //如果当前电机是以相反的方向转动,则停止电机
            s_ulTmpValue = 0;
            WB01_SetIndoorMotorStatus(WB01_MOTOR_STATUS_STOP);
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
        m_uCurIndoorWorkStatus = DOOR_WORK_STATUS_STOP;
        m_uCurIndoorStatus = MOTOR_CUR_STATUS_UNKNOW;
        
        //停止电机
        WB01_SetIndoorMotorStatus(WB01_MOTOR_STATUS_STOP);
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
        m_uCurOutdoorWorkStatus = DOOR_WORK_STATUS_OPEN;
    }
    else 
    {
        m_uCurOutdoorWorkStatus = DOOR_WORK_STATUS_CLOSE;
    }
    
    //设置当前状态
    m_uCurOutdoorStatus = MOTOR_CUR_STATUS_RUNNING;
    
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
    return m_uCurOutdoorStatus;
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
    
    switch (m_uCurOutdoorWorkStatus)
    {
    case DOOR_WORK_STATUS_STOP:
        
        break;
    case DOOR_WORK_STATUS_OPEN:
        
        //判断当前限位状态,若在限位处,则停止电机并退出
        if (GPIO_MAN_GetInputPinState(INPUT_IO_OUT_DOOR_OPEN_LIMIT))
        {
            //设置当前状态
            m_uCurOutdoorStatus = MOTOR_CUR_STATUS_OPEN_LIMIT;
            
            //设置当前工作步骤
            m_uCurOutdoorWorkStatus = DOOR_WORK_STATUS_STOP;
            
            //停止电机
            WB01_SetOutdoorMotorStatus(WB01_MOTOR_STATUS_STOP);
            
            break;
        }
        
        //判断当前电机工作状态
        switch (m_uCurOutdoorMotorStatus)
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
            m_uCurOutdoorWorkStatus = DOOR_WORK_STATUS_STOP;
            
            //停止电机
            WB01_SetOutdoorMotorStatus(WB01_MOTOR_STATUS_STOP);
            
            //设置当前状态
            m_uCurOutdoorStatus = MOTOR_CUR_STATUS_CLOSE_LIMIT;
            
            break;
        }
        
        //判断红外检测信号,若在关门的过程中,检测到有货物,则停止关门并重新开门
        if (GPIO_MAN_GetInputPinState(INPUT_IO_GOODS_DECTECT))
        {
            m_uCurOutdoorWorkStatus = DOOR_WORK_STATUS_OPEN;
            
            //重新进行超时计时
            SysTime_StartOneShot(&m_OutdoorOverlTimer, WB01_DOOR_OVER_TIME);
            
            break;
        }
        
        //判断当前电机工作状态
        switch (m_uCurOutdoorMotorStatus)
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
        m_uCurOutdoorWorkStatus = DOOR_WORK_STATUS_STOP;
        m_uCurOutdoorStatus = MOTOR_CUR_STATUS_UNKNOW;
        
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

#define WB01_TEST_INTERVAL       (1000)     //按键扫描间隔(MS)
static  SYS_TIME_DATA m_TestTimer = {1};    //测试定时器


/**
  * @brief  测试处理
  * @param  None
  * @retval None
  */
void WB01_TestHandler(void)
{
    static uBit32 s_ulTempValue = 0;
    (void)s_ulTempValue;
    
    if (SysTime_CheckExpiredState(&m_TestTimer))
    {
        SysTime_StartOneShot(&m_TestTimer, WB01_TEST_INTERVAL);   //设置下一次执行的时间

#if WB01_TEST_05
        
        switch (s_ulTempValue%30)
        {
        case 0:
            WB01_SetMainAxisMotorStatus(WB01_MOTOR_STATUS_CW); 
            DEBUF_PRINT("WB01_MOTOR_STATUS_CW\r\n");
            break;
        case 10: 
            WB01_SetMainAxisMotorStatus(WB01_MOTOR_STATUS_STOP); 
            DEBUF_PRINT("WB01_MOTOR_STATUS_STOP\r\n");
            break;
        case 15: 
            WB01_SetMainAxisMotorStatus(WB01_MOTOR_STATUS_ACW);
            DEBUF_PRINT("WB01_MOTOR_STATUS_ACW\r\n");
            break;
        case 25:
            WB01_SetMainAxisMotorStatus(WB01_MOTOR_STATUS_STOP); 
            DEBUF_PRINT("WB01_MOTOR_STATUS_STOP\r\n");
        }
        
#endif
        
        s_ulTempValue++;
        
    }
    
}





