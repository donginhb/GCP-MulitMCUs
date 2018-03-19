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

/**
  * @brief  目标柜号设置
  * @param  ulGridNumber 目标柜号
  * @retval 0-成功  1-正在运行,设置失败  2-设置指超过最大柜号
  */
uBit32 WB01_SetObjGridNumber(uBit32 ulGridNumber);


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
#define DOOR_WORK_STATUS_STOP       (0)     //空闲
#define DOOR_WORK_STATUS_OPEN       (1)     //开门操作
#define DOOR_WORK_STATUS_CLOSE      (2)     //关门操作


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

//定义货道电机资源
#define WB01_AISLE_MAX_ROW                      (1)             //货道最大行数
#define WB01_AISLE_MAX_COL                      (3)             //货道最大列数

//货道电机运行状态定义
typedef enum 
{
    WB01_AISLE_MOTOR_STATUS_IDLE     = 0,                       //货道电机空闲
    WB01_AISLE_MOTOR_STATUS_RUNNING  ,                          //货道电机运行中
    WB01_AISLE_MOTOR_STATUS_OVER     ,                          //超时
    WB01_AISLE_MOTOR_STATUS_POS_ERR  ,                          //限位信号异常
    
}WB01_AISLE_MOTOR_STATUS;



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
