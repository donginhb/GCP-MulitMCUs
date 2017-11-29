#ifndef  __SYS_CTRL_H
#define  __SYS_CTRL_H

#include "DataType/DataType.h"
#include "Version/Version.h"

#ifdef __cplusplus
extern "C"
{
#endif
    
    
/*****************************************************************************
 * 系统版本相关控制接口
 ****************************************************************************/

/**
  * @brief  系统逻辑控制版本获取
  * @param  pVersion 版本结构指针
  * @retval 0-成功 非0-失败
  */
uBit32 SYS_GetSLCVersion(SOFTWARE_VERSION* pVersion);


/**
  * @brief  通用控制平台版本获取
  * @param  pVersion 版本结构指针
  * @retval 0-成功 非0-失败
  */
uBit32 SYS_GetGCPVersion(SOFTWARE_VERSION* pVersion);


/*****************************************************************************
 * 系统时间相关控制接口
 ****************************************************************************/

/**
  * @brief  系统滴答时间获取
  * @param  None
  * @retval 滴答值
  * @note   在SysTimer之中已经有完善的接口以满足系统的需求,
  *         本函数的作用是兼容CNC及CMU中系统时基管理的接口;
  */
double SYS_GetTickCount(void);


#ifdef __cplusplus
}
#endif

#endif /* __SYS_CTRL_H */
