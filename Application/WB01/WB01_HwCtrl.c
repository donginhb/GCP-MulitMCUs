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

#define WB01_DEBUG

#ifdef WB01_DEBUG
#define DEBUF_PRINT(x)      UART_SendStr(WB01_DEBUG_UART_NODE, x)
//#define DEBUF_PRINT(x)      UART_SendStr(WB01_COM_UART_NODE, x)
#else 
#define DEBUF_PRINT(x)   
#endif


#define WB01_TEST_01  (0)
#define WB01_TEST_02  (0)
#define WB01_TEST_03  (0)
#define WB01_TEST_04  (0)
#define WB01_TEST_05  (1)

#if ((WB01_TEST_01 + WB01_TEST_02 + WB01_TEST_03 + WB01_TEST_04 + WB01_TEST_05) > 1)
#error "错误"
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
    PWM_SetOutputPwmFrq(WB01_MOTOR_PWM_NODE, 1000);
    PWM_OutputEnable(WB01_MOTOR_PWM_NODE, true);
    
    //初始化HC595
    HC595_Init(OUTPUT_IO_HC595_SCK, OUTPUT_IO_HC595_RCK, OUTPUT_IO_HC595_SI, 16);
    
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
        
#if !WB01_TEST_02
        GPIO_ToggleOutputState(OUTPUT_IO_LED0);
#endif
    }

}


/*****************************************************************************
 * 主轴电机线程接口
 ****************************************************************************/

//主轴电机运行状态定义
#define WB01_MOTOR_STATUS_STOP      (0) //停止
#define WB01_MOTOR_STATUS_CW        (1) //正转
#define WB01_MOTOR_STATUS_ACW       (2) //反转
//#define WB01_MOTOR_STATUS_ESTOP     (3) //刹车

//主轴电机当前运动方向定义
#define WB01_MOTOR_DIR_CW           (0) //正转方向
#define WB01_MOTOR_DIR_ACW          (1) //正转方向

#define MAIN_AXIS_GRID_TIMEROVER    (3000)      //单格超时时间

static uBit8 m_uCurMotorDir = WB01_MOTOR_DIR_CW;    //当前电机运动方向 0-正转 1-逆转
static uBit8 m_uCurAxisMotorStatus = 0; //当前主轴电机状态


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

bool g_bMainAxisMotorRunningFlag = false;   //主轴电机运行状态


/**
  * @brief  目标柜号设置
  * @param  None
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

/**
  * @brief  出货流程处理
  * @param  None
  * @retval None
  */
void WB01_OutGoodsHandler(void)
{
    if (SysTime_CheckExpiredState(&m_OutGoodsCtrlTimer))
    {
        SysTime_StartOneShot(&m_OutGoodsCtrlTimer, WB01_OUTGOODS_PROC_TIME); //设置下一次执行的时间
        
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
            
            //打印当前柜号
            uBit8 uDisplayBuff[128] = {0};
            sprintf((char *)uDisplayBuff, "Cur Grid is %d\r\n", g_lCurGridNumber);
            DEBUF_PRINT(uDisplayBuff);
            
#if WB01_TEST_02
            GPIO_ToggleOutputState(OUTPUT_IO_LED0);
#endif
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

#if WB01_TEST_01
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
            WB01_SetMainAxisMotorStatus(WB01_MOTOR_STATUS_ESTOP); 
            DEBUF_PRINT("WB01_MOTOR_STATUS_ESTOP\r\n");
            break;
        }
#endif
        
#if WB01_TEST_03
        if (s_ulTempValue == 10)
        {
            WB01_SetObjGridNumber(5);
            DEBUF_PRINT("Start to 5 grid\r\n");
        }
#endif
        
#if WB01_TEST_04
        
        switch (s_ulTempValue%45)
        {
        case 10:
            WB01_SetObjGridNumber(10);
            DEBUF_PRINT("ID: 10 \r\n");
            break;
        case 20:
            WB01_SetObjGridNumber(15);
            DEBUF_PRINT("ID: 15 \r\n");
            break;
        case 30: 
            WB01_SetObjGridNumber(0);
            DEBUF_PRINT("Main Axis Motor Reset To ID:0\r\n");
            break;
        }
        
#endif
        
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


/*****************************************************************************
 * 进货门出货门电机控制线程接口
 ****************************************************************************/

//主轴电机运行状态定义
#define WB01_MOTOR_STATUS_STOP      (0)     //停止
#define WB01_MOTOR_STATUS_CW        (1)     //正转
#define WB01_MOTOR_STATUS_ACW       (2)     //反转


//入货门电机控制错误码定义
#define MOTOR_ERR_SUCCESS           (1)     //成功
#define MOTOR_ERR_TIMEOVER          (2)     //超时
#define MOTOR_ERR_DECT_OPEN         (3)     //关闭的过程之中检测到物体,已重新打开
#define MOTOR_ERR_DECT_OPEN_FAIL    (4)     //关闭的过程之中检测到物体,但是重新打开失败


//门工作状态定义
#define DOOR_WORK_STATUS_IDLE       (0)     //空闲
#define DOOR_WORK_STATUS_OPEN       (1)     //开门操作
#define DOOR_WORK_STATUS_CLOSE      (2)     //关门操作


static uBit8 m_uCurIndoorMotorStatus = 0;   //当前进货门电机状态
static uBit8 m_uCurOutdoorMotorStatus = 0;  //当前出货门电机状态

static uBit8 m_uCurIndoorWorkStatus = DOOR_WORK_STATUS_IDLE;    //当前进货门工作状态
static uBit8 m_uCurOutdoorWorkStatus = DOOR_WORK_STATUS_IDLE;   //当前出货门工作状态


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
    
}


/**
  * @brief  入货门状态控制
  * @param  bIsOpen 0-关门  1-开门
  * @retval None
  */
void WB01_SetIndoorStatus(bool bIsOpen)
{
    if (bIsOpen)
    {
        m_uCurIndoorWorkStatus = DOOR_WORK_STATUS_OPEN;
    }
    else 
    {
        m_uCurIndoorWorkStatus = DOOR_WORK_STATUS_CLOSE;
    }
    
}


/**
  * @brief  入货门电机控制
  * @param  None
  * @retval None
  */
void WB01_IndoorHandler(void)
{
    static uBit32 s_ulTmpValue = 0;
    
    switch (m_uCurIndoorWorkStatus)
    {
    case DOOR_WORK_STATUS_IDLE:
        
        break;
    case DOOR_WORK_STATUS_OPEN:
        
        //判断当前限位状态,若在限位处,则停止电机并退出
        if (GPIO_MAN_GetInputPinState(INPUT_IO_IN_DOOR_OPEN_LIMIT))
        {
            //设置当前工作步骤
            m_uCurIndoorWorkStatus = DOOR_WORK_STATUS_IDLE;
            
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
            m_uCurIndoorWorkStatus = DOOR_WORK_STATUS_IDLE;
            
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
    
}


/**
  * @brief  出货门状态控制
  * @param  bIsOpen 0-关门  1-开门
  * @retval None
  */
void WB01_SetOutdoorStatus(bool bIsOpen)
{
    if (bIsOpen)
    {
        m_uCurOutdoorWorkStatus = DOOR_WORK_STATUS_OPEN;
    }
    else 
    {
        m_uCurOutdoorWorkStatus = DOOR_WORK_STATUS_CLOSE;
    }
    
}


/**
  * @brief  出货门电机控制
  * @param  None
  * @retval None
  */
void WB01_OutdoorHandler(void)
{
    static uBit32 s_ulTmpValue = 0;
    
    switch (m_uCurOutdoorWorkStatus)
    {
    case DOOR_WORK_STATUS_IDLE:
        
        break;
    case DOOR_WORK_STATUS_OPEN:
        
        //判断当前限位状态,若在限位处,则停止电机并退出
        if (GPIO_MAN_GetInputPinState(INPUT_IO_OUT_DOOR_OPEN_LIMIT))
        {
            //设置当前工作步骤
            m_uCurOutdoorWorkStatus = DOOR_WORK_STATUS_IDLE;
            
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
            m_uCurOutdoorWorkStatus = DOOR_WORK_STATUS_IDLE;
            
            //停止电机
            WB01_SetOutdoorMotorStatus(WB01_MOTOR_STATUS_STOP);
            
            break;
        }
        
        //判断红外检测信号,若在关门的过程中,检测到有货物,则停止关门并重新开门
        if (GPIO_MAN_GetInputPinState(INPUT_IO_GOODS_DECTECT))
        {
            m_uCurOutdoorWorkStatus = DOOR_WORK_STATUS_OPEN;
            
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
    
}


/*****************************************************************************
 * 货道限位IO检测线程接口
 ****************************************************************************/

//定义货道电机资源
#define WB01_AISLE_MAX_ROW                      (10)            //货道最大行数
#define WB01_AISLE_MAX_COL                      (10)            //货道最大列数

//货道电机限位信号电平状态
#define WB01_AISLE_INDEX_SIGNAL_VALID           (true)          //限位信号有效信号
#define WB01_AISLE_INDEX_SIGNAL_INVALID         (false)         //限位信号无效信号

//货道电机时间参数定义
#define WB01_AISLE_MOTOR_DELAY_TIME             (500)           //货道电机检测延时时间(MS)
#define WB01_AISLE_MOTOR_OVER_TIME              (4000)          //货道电机超时时间

//货道电机运行状态定义
typedef enum 
{
    WB01_AISLE_MOTOR_STATUS_IDLE     = 0,                       //货道电机空闲
    WB01_AISLE_MOTOR_STATUS_RUNNING  ,                          //货道电机运行中
    WB01_AISLE_MOTOR_STATUS_OVER     ,                          //超时
    WB01_AISLE_MOTOR_STATUS_POS_ERR  ,                          //限位信号异常
    
}WB01_AISLE_MOTOR_STATUS;



//货道电机运行步骤定义
typedef enum 
{
    WB01_AISLE_MOTOR_WORK_IDLE = 0,                             //空闲
    WB01_AISLE_MOTOR_WORK_FAST,                                 //快进
    WB01_AISLE_MOTOR_WORK_FAST_STOP,                            //快进停止
    WB01_AISLE_MOTOR_WORK_SLOW_STOP,                            //慢进缓冲
    WB01_AISLE_MOTOR_WORK_SLOW,                                 //慢进
    WB01_AISLE_MOTOR_WORK_SIGNAL_VERIFY,                        //信号确认
    WB01_AISLE_MOTOR_WORK_FINISH,                               //结束处理
    
}WB01_AISLE_MOTOR_WORK_STEP;


static SYS_TIME_DATA m_AisleDelayTimer = {0};                   //货道电机检测延时定时器
static SYS_TIME_DATA m_AisleFastTimer = {0};                    //货道电机快进计时定时器
static SYS_TIME_DATA m_AisleSlowTimer = {0};                    //货道电机慢进计时定时器
static SYS_TIME_DATA m_AisleSlowStopTimer = {0};                //货道电机慢进缓冲计时定时器
static SYS_TIME_DATA m_AisleOverTimer = {0};                    //货道电机超时计时定时器
static SYS_TIME_DATA m_AisleVerifyTimer = {0};                  //货道电机信号确认计时定时器

static uBit32 m_ulCurRow = 0xFF;                                //当前行
static uBit32 m_ulCurCol = 0xFF;                                //当前列
static uBit32 m_ulCurOutputValue = 0;                           //当前输出值(高6位是行信号,低10位是列信号.而行信号为低电平,而列信号为高电平时货道电机旋转)

static WB01_AISLE_MOTOR_STATUS m_vm_AisleMotorRunningStatus = WB01_AISLE_MOTOR_STATUS_IDLE; //货道电机运行状态
static WB01_AISLE_MOTOR_WORK_STEP m_vm_CurAisleMotorStep = WB01_AISLE_MOTOR_WORK_IDLE; //当前货道电机工作步骤


static uBit16 SaleAinStart[6] = {0x003e, 0x003d, 0x003b, 0x0037, 0x002f, 0x001f}; //层数 L0-L5
static uBit16 MotorNumStart[10] = {0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000, 0x0040, 0x0080}; //电机m0-m9

static uBit16 SaleAinStop[6] = {0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020};  //层数 L0-L5
static uBit16 MotorNumStop[10] = {0xfe00, 0xfd00, 0xfb00, 0xf700, 0xef00, 0xdf00, 0xbf00, 0x7f00, 0x00bf, 0x007f}; //电机m0-m9


/**
  * @brief  货道电机设置
  * @param  ulRow 行号,从0算起
  * @param  bState 信号状态
  * @retval None
  */
static void WB01_SetAsileMotor(uBit32 ulRow, uBit32 ulCol, bool bState)
{
    uBit16 nTmpVlue = 0;
    
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
  * @retval 0-成功 1-参数范围错误 2-货道电机正在运行
  */
uBit32 WB01_EnableAisleMotor(uBit32 ulRow, uBit32 ulCol)
{
    //校验入参参数
    if ((ulRow >= WB01_AISLE_MAX_ROW) || (ulCol >= WB01_AISLE_MAX_COL))
    {
        return 1;
    }
    
    //判断电机是否在运行
    if (m_vm_AisleMotorRunningStatus == WB01_AISLE_MOTOR_STATUS_RUNNING)
    {
        return 2;
    }
    
    m_ulCurRow = ulRow;  //行
    m_ulCurCol = ulCol;  //列
    
    //使能货道电机行信号
    WB01_SetAsileMotor(ulRow, ulCol, true);
    
    //设置电机运行标志位
    m_vm_AisleMotorRunningStatus = WB01_AISLE_MOTOR_STATUS_RUNNING;
    
    //启动检测延时定时器
    SysTime_StartOneShot(&m_AisleDelayTimer, WB01_AISLE_MOTOR_DELAY_TIME);
    
    //设置工作步骤
    m_vm_CurAisleMotorStep = WB01_AISLE_MOTOR_WORK_FAST;

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
        //等待延时事件到达
        if (SysTime_CheckExpiredState(&m_AisleDelayTimer))
        {
            SysTime_StartOneShot(&m_AisleDelayTimer, WB01_AISLE_MOTOR_DELAY_TIME);
            
            //检测到限位信号
            if (WB01_GetAisleIndexSignal(m_ulCurRow) == WB01_AISLE_INDEX_SIGNAL_VALID)
            {
                
            }
            
            m_vm_CurAisleMotorStep++;
        }
        
        
    }
    
    
    
    
}








