#ifndef  __HW_SYS_INIT_H
#define  __HW_SYS_INIT_H

#ifdef __cplusplus
extern "C" {
#endif
    
/**
  * @brief  系统初始化(在main函数之前中执行).
  * @param  None
  * @retval None
  * @note   由于库函数提供的系统初始化函数中,会将芯片内核频率设置为120M,而
  *         LPC1752最高内核频率是100M,直接调用库函数进行系统初始化会导致系
  *         统不稳定,所以在这里重新实现其内核的初始化.
  */
void Board_SystemInit(void);

#ifdef __cplusplus
}
#endif


#endif /* __HW_SYS_INIT_H */
