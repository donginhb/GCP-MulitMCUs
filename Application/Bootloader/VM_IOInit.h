#ifndef  __VM_IO_INIT_H
#define  __VM_IO_INIT_H


#ifdef __cplusplus
extern "C"
{
#endif


/*****************************************************************************
 * 水果机IO资源控制接口
 ****************************************************************************/

/**
  * @brief  IO初始化
  * @param  None
  * @retval None
  */
void VM_IOConfig(void);


/**
  * @brief  LED 流水灯循环
  * @param  None
  * @retval None
  */
void VM_ShowMainWorkLed(void);

    

#ifdef __cplusplus
}
#endif


#endif /* __VM_IO_DEF_H */
