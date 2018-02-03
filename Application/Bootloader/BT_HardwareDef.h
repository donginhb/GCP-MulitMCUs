#ifndef __BT_HARDWARE_DEF_H
#define __BT_HARDWARE_DEF_H

#include "VM_IOInit.h"

/*****************************************************************************
 * 系统硬件资源参数
 ****************************************************************************/

#define BT_HwInit()                 VM_IOConfig()
#define BT_ShowMainWorkLed()        VM_ShowMainWorkLed()


#endif /* __BT_HARDWARE_DEF_H */
