#ifndef  __HAL_CONFIG_H
#define  __HAL_CONFIG_H

#include "DataType/DataType.h"

//芯片资源定义
#if define(LPC17XX)

#define HW_UART_NODE_COUNT      (4)
#define HW_SPI_NODE_COUNT       (2)
#define HW_I2S_NODE_COUNT       (1)
#define HW_CAN_NODE_COUNT       (2)

#elif define(LPC43XX)


#elif define(STM32F10X)


#elif define(STM32F0XX)


#endif



#endif /* __HAL_CONFIG_H */
