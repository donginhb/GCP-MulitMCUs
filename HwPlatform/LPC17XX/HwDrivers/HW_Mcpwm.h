#ifndef  __HW_MCPWM_H
#define  __HW_MCPWM_H

#include "chip.h"

#ifdef __cplusplus
extern "C"
{
#endif
    
//MCPWM节点定义
#define HW_MCPWM_NODE_0             (0)
#define HW_MCPWM_NODE_1             (1)
#define HW_MCPWM_NODE_2             (2)


/*****************************************************************************
 * MCPWM 输出相关控制接口
 ****************************************************************************/

/**
  * @brief  MCPWM初始化
  * @param  uMcpwmNode MCPWM节点
  * @retval None
  */
void HW_MCPWM_InitOutput(uint8_t uMcpwmNode, bool bIsEnableMCOB);


#ifdef __cplusplus
}
#endif

#endif /* __HW_MCPWM_H */
