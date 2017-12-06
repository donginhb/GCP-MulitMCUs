#ifndef  __MOTOR_CTRL_H
#define  __MOTOR_CTRL_H

#include "DataType/DataType.h"

#ifdef __cplusplus
extern "C"
{
#endif


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
               uBit8 uPwmCountNode, uBit32 ulCountChNum, uBit32 ulAxisDirPortNO);


void AXIS_CountCallBack(void);


int PWM_UpdateTimeCycle(int iTimeCycleUs);

/*
函 数 名：void PWM_SetOutput(void)
功    能：PWM输出
参    数：无
返 回 值：无
调用位置：内部调用
注意事项：如果PWM正在输出则无操作，如果PWM缓冲区无数据则无操作；PWM输出先停止PWM输出，再停止捕获寄存器，更改
*/
void PWM_SetOutput(void);


/*
函 数 名：int PWM_SendFeed(long lPinc)
功    能：将单周期脉冲进给量存入PWM缓冲区
参    数：lPinc --单周期脉冲进给量
返 回 值：成功返回0，否则返回非0值
调用位置：
注意事项：
*/
int PWM_SendFeed(long lPinc);


//获取当前PWM脉冲方向,1--正向 0--负向
int PWM_GetDir(void);

/*
函 数 名：int CheckPWMBusy()
功    能：检查PWM是否忙（正在输出脉冲）
参    数：无
返 回 值：PWM正在输出脉冲返回1，否则返回0
调用位置：
注意事项：
*/
int PWM_CheckBusy(void);


//获取当前指令脉冲位置
long PWM_GetCmdPulse(void);


//获取当前实际脉冲位置
long PWM_GetActPulse(void);

//获取脉冲速度pulse/ms
float PWM_GetPulseSpeed(void);


//设置当前指令脉冲位置
void PWM_SetCmdPulse(long lCmdPulse);


//设置当前实际脉冲位置
void PWM_SetActPulse(long lActPulse);



#ifdef __cplusplus
}
#endif

#endif /* __MOTOR_CTRL_H */
