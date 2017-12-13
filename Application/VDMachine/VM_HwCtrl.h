#ifndef  __VM_HW_CFG_H
#define  __VM_HW_CFG_H

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
void VM_HwInit(void);


/*****************************************************************************
 * 主线程LED显示接口
 ****************************************************************************/

/**
  * @brief  LED 流水灯循环
  * @param  None
  * @retval None
  */
void VM_MainWorkLedShow(void);



/*****************************************************************************
 * 货道限位IO检测线程接口
 ****************************************************************************/
     
/**
  * @brief  VM 启动货道电机
  * @param  ulRow 行信号 [0-9]
  * @param  ulCol 列信号 [0-9]
  * @retval 0-成功 非0-失败
  */
uBit32 VM_EnableAisleMotor(uBit32 ulRow, uBit32 ulCol);
     

/**
  * @brief  VM限位信号检测
  * @param  None
  * @retval None
  */
void VM_MotorLimitDetectHandler(void);




#ifdef __cplusplus
}
#endif


#endif /* __VM_HW_CFG_H */
