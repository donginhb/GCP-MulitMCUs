/**
  ******************************************************************************
  * @file    MotorCtrl.c
  * @author  Duhanfneg
  * @version V1.0
  * @date    2017.11.29
  * @brief   Motor place control application
  ******************************************************************************
  * @attention
  * 
  * 
  * 
  * 
  ******************************************************************************
  */
  
/***********************************<INCLUDES>**********************************/
#include "MotorCtrl.h"
#include "DataType/DataType.h"
#include "SysPeripheral/PWM/PWM_MotorCtrl.h"
#include "SysPeripheral/GPIO/GPIO_Man.h"
#include <stdlib.h>

/*****************************************************************************
 * 私有成员定义及实现
 ****************************************************************************/


/*****************************************************************************
 * MotorCtrl相关控制接口
 ****************************************************************************/

#define AXIS_MAX_COUNT      (3)
#define PWM_CTRL_BIT_STOP   (0X01)  //停止标志


typedef struct 
{
    unsigned char uCurDir;              //运动方向, 取值：1-正向 0-负向
    unsigned long ulPeriodValue;        //周期
    unsigned long ulCAPMRO;             //捕获值
}PWM_CTRL_DATA;

static unsigned char m_pwm_uPwmRunning = 0;                     //PWM正在输出标志  0-pwm输出停止 1-正在输出
static unsigned char m_pwm_uPwmBufferFullFlag = 0;              //缓冲区满标志1：满 0：空
static PWM_CTRL_DATA m_pwm_sPwmCtrlData;                        //脉冲数据缓冲区管理

static long m_pwm_lPulseInc = 0;//当前执行的脉冲增量    
static long m_pwm_lCmdPos = 0;  //指令位置，输出PWM时累加
static long m_pwm_lActPos = 0;  //实际位置，PWM输出结束时累加

//轴相关参数定义
static uBit8  m_uAxisPwmNode[AXIS_MAX_COUNT] = {0};             //轴对应的PWM节点号
static uBit32 m_ulAxisDirPort[AXIS_MAX_COUNT] = {0};            //轴对应的DIR控制脚,true为正转,false为反转


/**
  * @brief  轴相关参数配置
  * @param  uAxisNO 轴编号
  * @param  uPwmOutputNode PWM节点
  * @param  ulOutChNum 输出通道编号
  * @param  uPwmCountNode  PWM计数节点
  * @param  ulCountChNum 计数通道编号
  * @param  ulAxisDirPortNO 轴方向角在IO资源表中的编号
  * @retval 0-成功 非0-失败
  */
uBit32 AXIS_Init(uBit8 uAxisNO, uBit8 uPwmOutputNode, uBit32 ulOutChNum, 
               uBit8 uPwmCountNode, uBit32 ulCountChNum, uBit32 ulAxisDirPortNO)
{
    uBit32 ulRet = 0;
    
    //存储轴对应的PWM节点以及IO编号
    m_uAxisPwmNode[uAxisNO] = uPwmOutputNode;
    m_ulAxisDirPort[uAxisNO] = ulAxisDirPortNO;
    
    //初始化电机脉冲功能
    ulRet = PWM_MotorPulseInit(uPwmOutputNode, ulOutChNum, uPwmCountNode, ulCountChNum);
    
    return ulRet;
}



void AXIS_CountCallBack(void)
{
    m_pwm_lActPos += m_pwm_lPulseInc;
    m_pwm_uPwmRunning = 0;
    
    if (m_pwm_uPwmBufferFullFlag)
    {
        PWM_SetOutput();
    }
    
}

int PWM_UpdateTimeCycle(int iTimeCycleUs)
{
    if(m_pwm_uPwmRunning)//正在PWM输出
    {
      return 1;
    }
    
    return 0;
}


/*
函 数 名：void PWM_SetOutput(void)
功    能：PWM输出
参    数：无
返 回 值：无
调用位置：内部调用
注意事项：如果PWM正在输出则无操作，如果PWM缓冲区无数据则无操作；PWM输出先停止PWM输出，再停止捕获寄存器，更改
*/
void PWM_SetOutput(void)
{
    if(m_pwm_uPwmRunning)//正在PWM输出
    {
        return;
    }
    
    if(m_pwm_uPwmBufferFullFlag==0)
    {
        return;
    }
    
    //设置运动方向
    if(m_pwm_sPwmCtrlData.uCurDir)  //正向
    {   
        GPIO_MAN_SetOutputPinState(m_ulAxisDirPort[0], true);
        m_pwm_lPulseInc = m_pwm_sPwmCtrlData.ulCAPMRO;
        m_pwm_lCmdPos += m_pwm_lPulseInc;
    }
    else    //负向
    {   
        GPIO_MAN_SetOutputPinState(m_ulAxisDirPort[0], false);
        m_pwm_lPulseInc = -m_pwm_sPwmCtrlData.ulCAPMRO;
        m_pwm_lCmdPos -= m_pwm_sPwmCtrlData.ulCAPMRO;
    }
    
    //设置PWM输出
    PWM_SendPulseLimitMs(m_uAxisPwmNode[0], m_pwm_sPwmCtrlData.ulCAPMRO);
    
    m_pwm_uPwmRunning = 1; 
    m_pwm_uPwmBufferFullFlag = 0;
}


/*
函 数 名：int PWM_SendFeed(long lPinc)
功    能：将单周期脉冲进给量存入PWM缓冲区
参    数：lPinc --单周期脉冲进给量
返 回 值：成功返回0，否则返回非0值
调用位置：
注意事项：
*/
int PWM_SendFeed(long lPinc)
{
    if (m_pwm_uPwmBufferFullFlag)
    {
        return 1;
    }
    
    if(lPinc==0)
    {
        return 0;
    }
    
    m_pwm_sPwmCtrlData.uCurDir = (lPinc > 0) ? 1 : 0;
    lPinc = abs(lPinc);
    
    //m_pwm_sPwmCtrlData.ulPeriodValue = m_pwm_ulPwmFrq/lPinc;               //周期   
    m_pwm_sPwmCtrlData.ulCAPMRO = lPinc;                                   //PWM捕获计数匹配值
    m_pwm_uPwmBufferFullFlag = 1;
    
    if(m_pwm_uPwmRunning == 0)//未启动
    {
        PWM_SetOutput();      
    }
    
    return 0;
}


//获取当前PWM脉冲方向,1--正向 0--负向
int PWM_GetDir(void)
{
    if(m_pwm_lPulseInc < 0)
    {
        return 0;
    }
    
    return 1;
}


/*
函 数 名：int CheckPWMBusy()
功    能：检查PWM是否忙（正在输出脉冲）
参    数：无
返 回 值：PWM正在输出脉冲返回1，否则返回0
调用位置：
注意事项：
*/
int PWM_CheckBusy(void)
{
    
    return m_pwm_uPwmRunning;
}



//获取当前指令脉冲位置
long PWM_GetCmdPulse(void)
{
    return m_pwm_lCmdPos;
}


//获取当前实际脉冲位置
long PWM_GetActPulse(void)
{
    if(m_pwm_uPwmRunning)//正在运行
    {
        if(m_pwm_lPulseInc > 0)
        {
            return m_pwm_lActPos + PWM_GetCount(m_uAxisPwmNode[0]);
        }
        else
        {
            return m_pwm_lActPos - PWM_GetCount(m_uAxisPwmNode[0]);
        }
    }
    else
    {
        return m_pwm_lActPos;
    }
}

//获取脉冲速度pulse/ms
float PWM_GetPulseSpeed(void)
{
    float fSpeed=0.0;
    
    if(m_pwm_uPwmRunning==0)
    {
        return 0.0;
    }
    
#if 0
    fSpeed = PWM_FREQUECY;
    fSpeed *= (float)m_pwm_lPulseInc/(float)m_pwm_ulPwmFrq;
#else 
    fSpeed = (float)m_pwm_lPulseInc;
#endif
    
    return fSpeed;
}


//设置当前指令脉冲位置
void PWM_SetCmdPulse(long lCmdPulse)
{
    m_pwm_lCmdPos = lCmdPulse;
}


//设置当前实际脉冲位置
void PWM_SetActPulse(long lActPulse)
{
    m_pwm_lActPos = lActPulse;  
}
