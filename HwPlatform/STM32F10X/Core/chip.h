#ifndef __CHIP_H_
#define __CHIP_H_

#if defined(STM32F030)
#include "stm32f0xx.h"
#endif

#if defined(STM32F10X_HD)
#include "stm32f10x.h"
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef __cplusplus

#ifndef bool
#define bool _Bool
#endif

#ifndef false
#define false 0
#endif

#ifndef true
#define true 1
#endif

#endif /* !__cplusplus */


#ifdef __CC_ARM
#define INLINE  __inline
#else
#define INLINE inline
#endif

#endif /* __CHIP_H_ */
