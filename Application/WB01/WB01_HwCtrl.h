#ifndef  __WB01_HW_CFG_H
#define  __WB01_HW_CFG_H

#include "DataType/DataType.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
 * 硬件配置接口
 ****************************************************************************/

/**
  * @brief  系统硬件初始化
  * @param  None
  * @retval None
  */
void WB01_HwInit(void);


/*****************************************************************************
 * LED显示线程接口
 ****************************************************************************/

/**
  * @brief  LED 流水灯循环
  * @param  None
  * @retval None
  */
void WB01_MainWorkLedShow(void);


/*****************************************************************************
 * 主轴电机线程接口
 ****************************************************************************/

//主轴电机运行状态定义
#define WB01_MOTOR_STATUS_STOP      (0) //停止
#define WB01_MOTOR_STATUS_CW        (1) //正转
#define WB01_MOTOR_STATUS_ACW       (2) //反转
#define MAIN_AXIS_GRID_TIMEROVER    (3000)      //单格超时时间


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
void WB01_SetMainAxisMotorStatus(uBit8 uMotorStatus);


/**
  * @brief  主轴电机状态获取
  * @param  uMotorStatus: 0-停止 1-正转 2-反转 3-刹车
  * @retval 电机状态
  *     @arg WB01_MOTOR_STATUS_STOP   停止
  *     @arg WB01_MOTOR_STATUS_CW     正转
  *     @arg WB01_MOTOR_STATUS_ACW    反转
  *     @arg WB01_MOTOR_STATUS_ESTOP  刹车
  */
uBit8 WB01_GetMainAxisMotorStatus(void);


/*****************************************************************************
 * 出货流程线程接口
 ****************************************************************************/

/**
  * @brief  目标柜号设置
  * @param  None
  * @retval 0-成功  1-正在运行,设置失败
  */
uBit32 WB01_SetObjGridNumber(uBit32 ulGridNumber);


/**
  * @brief  出货流程处理
  * @param  None
  * @retval None
  */
void WB01_OutGoodsHandler(void);


/*****************************************************************************
 * 霍尔传感器监控线程接口
 ****************************************************************************/

/**
  * @brief  霍尔传感器处理
  * @param  None
  * @retval None
  */
void WB01_HallSensorProc(void);



/*****************************************************************************
 * 测试线程接口
 ****************************************************************************/


/**
  * @brief  测试处理
  * @param  None
  * @retval None
  */
void WB01_TestHandler(void);


#ifdef __cplusplus
}
#endif


#endif /* __WB01_HW_CFG_H */
