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

/**
  * @brief  主轴电机状态设置
  * @param  uMotorStatus:
  *     @arg WB01_MOTOR_STATUS_STOP   停止
  *     @arg WB01_MOTOR_STATUS_CW     正转
  *     @arg WB01_MOTOR_STATUS_ACW    反转
  * @retval None
  */
void WB01_SetMainAxisMotorStatus(uBit8 uMotorStatus);


/**
  * @brief  主轴电机状态获取
  * @param  uMotorStatus: 0-停止 1-正转 2-反转
  * @retval 电机状态
  *     @arg WB01_MOTOR_STATUS_STOP   停止
  *     @arg WB01_MOTOR_STATUS_CW     正转
  *     @arg WB01_MOTOR_STATUS_ACW    反转
  */
uBit8 WB01_GetMainAxisMotorStatus(void);



/*****************************************************************************
 * 出货流程线程接口
 ****************************************************************************/

extern uBit32 WB01_OUTGOODS_START_SPEED;    //启动速度
extern uBit32 WB01_OUTGOODS_FAST_SPEED ;    //快进速度
extern uBit32 WB01_OUTGOODS_SLOW_SPEED ;    //慢进速度
extern uBit32 WB01_OUTGOODS_RESET_SPEED;    //复位速度


/**
  * @brief  目标柜号获取
  * @param  None
  * @retval 目标柜号
  */
uBit32 WB01_GetObjGridNumber(void);


/**
  * @brief  总柜数获取
  * @param  None
  * @retval 总柜数
  */
uBit32 WB01_GetMaxGridCount(void);


/**
  * @brief  自学习开始
  * @param  None
  * @retval 0-成功 1-忙
  */
uBit32 WB01_StartSelfLearn(void);


/**
  * @brief  自学习运行状态获取
  * @param  None
  * @retval 0-成功 1-忙
  */
uBit32 WB01_GetSelfLearnStep(void);


/**
  * @brief  自学习任务处理
  * @param  None
  * @retval None
  */
void WB01_SelfLearnHandler(void);


/**
  * @brief  出货复位任务开始
  * @param  None
  * @retval 0-成功 1-忙
  */
uBit32 WB01_StartOutGoodsResetTask(void);


/**
  * @brief  出货复位任务
  * @param  None
  * @retval None
  */
void WB01_OutGoodsResetHandler(void);


/**
  * @brief  目标柜号设置
  * @param  ulGridNumber 目标柜号
  * @retval 0-成功 1-正在运行,设置失败  2-设置指超过最大柜号 3-人体感应信号触发,指令无效
  */
uBit32 WB01_SetObjGridNumber(uBit32 ulGridNumber);


/**
  * @brief  出货流程步骤获取
  * @param  None
  * @retval 0-完成 1-进行中 2-暂停(因为人体红外检测的原因)
  */
uBit32 WB01_GetOutGoodsStep(void);


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


/*****************************************************************************
 * 进货门出货门电机控制线程接口
 ****************************************************************************/

/**
  * @brief  入货门状态控制
  * @param  bIsOpen 0-关门  1-开门
  * @retval None
  */
void WB01_SetIndoorStatus(bool bIsOpen);


/**
  * @brief  入货门状态获取
  * @param  None
  * @retval None
  */
uBit8 WB01_GetIndoorStatus(void);


/**
  * @brief  入货门电机控制
  * @param  None
  * @retval None
  */
void WB01_IndoorHandler(void);


/**
  * @brief  出货门状态控制
  * @param  bIsOpen 0-关门  1-开门
  * @retval None
  */
void WB01_SetOutdoorStatus(bool bIsOpen);


/**
  * @brief  入货门状态获取
  * @param  None
  * @retval None
  */
uBit8 WB01_GetOutdoorStatus(void);


/**
  * @brief  出货门电机控制
  * @param  None
  * @retval None
  */
void WB01_OutdoorHandler(void);


/*****************************************************************************
 * 货道限位IO检测线程接口
 ****************************************************************************/


//货道电机运行状态定义
typedef enum 
{
    WB01_AISLE_MOTOR_STATUS_IDLE     = 0,                       //货道电机空闲
    WB01_AISLE_MOTOR_STATUS_RUNNING  ,                          //货道电机运行中
    WB01_AISLE_MOTOR_STATUS_OVER     ,                          //超时
    WB01_AISLE_MOTOR_STATUS_POS_ERR  ,                          //限位信号异常
    
}WB01_AISLE_MOTOR_STATUS;


/**
  * @brief  货道电机设置
  * @param  ulRow 行号,从0算起
  * @param  bState 信号状态
  * @retval None
  */
void WB01_SetAsileMotor(uBit32 ulRow, uBit32 ulCol, bool bState);


/**
  * @brief  货道电机启动
  * @param  ulRow 行号,从0算起
  * @param  ulCol 列号,从0算起
  * @retval 0-成功 1-货道电机正在运行 2-参数范围错误
  */
uBit32 WB01_EnableAisleMotor(uBit32 ulRow, uBit32 ulCol);
     

/**
  * @brief  货道电机运行状态获取
  * @param  None
  * @retval 货道电机运行状态
  *   @arg  WB01_AISLE_MOTOR_STATUS_IDLE     货道电机空闲
  *         WB01_AISLE_MOTOR_STATUS_RUNNING  货道电机运行中
  *         WB01_AISLE_MOTOR_STATUS_OVER     超时
  *         WB01_AISLE_MOTOR_STATUS_POS_ERR  限位信号异常
  */
uBit32 WB01_GetAisleMotorRunningState(void);


/**
  * @brief  货道电机管理
  * @param  None
  * @retval None
  */
void WB01_AisleMotorHandler(void);



#ifdef __cplusplus
}
#endif


#endif /* __WB01_HW_CFG_H */
