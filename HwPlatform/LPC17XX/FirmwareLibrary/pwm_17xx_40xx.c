/*
 * @brief LPC15xx PMU chip driver
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2014
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#include "chip.h"

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

#ifndef CHECK_PARAM
#define CHECK_PARAM(x)
#endif

/*****************************************************************************
 * Public types/enumerations/variables
 ****************************************************************************/

/*****************************************************************************
 * Private functions
 ****************************************************************************/

/* Returns clock for the peripheral block */
STATIC CHIP_SYSCTL_CLOCK_T Chip_PWM_GetClockIndex(LPC_PWM_T *pPWM)
{
    CHIP_SYSCTL_CLOCK_T clkPWM;

    if (pPWM == LPC_PWM1) {
        clkPWM = SYSCTL_CLOCK_PWM1;
    }
#if defined(CHIP_LPC177X_8X)
    else if (pPWM == LPC_PWM0) {
        clkPWM = SYSCTL_CLOCK_PWM0;
    }
#endif
    else
    {
        clkPWM = SYSCTL_CLOCK_PWM1;
    }

    return clkPWM;
}

/*****************************************************************************
 * Public functions
 ****************************************************************************/

/*********************************************************************//**
 * @brief         Initializes the pPWM peripheral corresponding to the specified
 *               parameters in the PWM_ConfigStruct.
 * @param[in]    pPWM PWM peripheral, should be LPC_PWM1
 * @param[in]    PWMTimerCounterMode Timer or Counter mode, should be:
 *                 - PWM_MODE_TIMER: Counter of PWM peripheral is in Timer mode
 *                 - PWM_MODE_COUNTER: Counter of PWM peripheral is in Counter mode
 * @param[in]    PWM_ConfigStruct Pointer to structure (PWM_TIMERCFG_Type or
 *                  PWM_COUNTERCFG_Type) which will be initialized.
 * @return         None
 * Note: PWM_ConfigStruct pointer will be assigned to corresponding structure
 *         (PWM_TIMERCFG_Type or PWM_COUNTERCFG_Type) due to PWMTimerCounterMode.
 **********************************************************************/
void Chip_PWM_Init(LPC_PWM_T *pPWM)
{
    Chip_Clock_EnablePeriphClock(Chip_PWM_GetClockIndex(pPWM));
    
}


/*********************************************************************//**
 * @brief        De-initializes the PWM peripheral registers to their
*                  default reset values.
 * @param[in]    pPWM    PWM peripheral selected, should be LPC_PWM1
 * @return         None
 **********************************************************************/
void Chip_PWM_DeInit (LPC_PWM_T *pPWM)
{
    Chip_Clock_DisablePeriphClock(Chip_PWM_GetClockIndex(pPWM));

}


uint32_t Chip_PWM_GetClock(LPC_PWM_T *pPWM)
{
    
    return Chip_Clock_GetPeripheralClockRate(SYSCTL_PCLK_PWM1);
}

