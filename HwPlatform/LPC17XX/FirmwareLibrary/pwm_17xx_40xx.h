/*
 * @brief LPC17xx_40xx PMU chip driver
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

#ifndef __PWM_17XX_40XX_H_
#define __PWM_17XX_40XX_H_

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief LPC17xx_40xx Power Management Unit register block structure
 */
typedef struct {
    __IO uint32_t IR;
    __IO uint32_t TCR;
    __IO uint32_t TC;
    __IO uint32_t PR;
    __IO uint32_t PC;
    __IO uint32_t MCR;
    __IO uint32_t MR0;
    __IO uint32_t MR1;
    __IO uint32_t MR2;
    __IO uint32_t MR3;
    __IO uint32_t CCR;
    __I  uint32_t CR0;
    __I  uint32_t CR1;
    __I  uint32_t CR2;
    __I  uint32_t CR3;
    uint32_t RESERVED0;
    __IO uint32_t MR4;
    __IO uint32_t MR5;
    __IO uint32_t MR6;
    __IO uint32_t PCR;
    __IO uint32_t LER;
    uint32_t RESERVED1[7];
    __IO uint32_t CTCR;
} LPC_PWM_T;


/* --------------------- BIT DEFINITIONS -------------------------------------- */
/**********************************************************************
* IR register definitions
**********************************************************************/
/** Interrupt flag for PWM match channel for 6 channel */
#define PWM_IR_PWMMRn(n)        ((uint32_t)((n<4)?(1<<n):(1<<(n+4))))
/** Interrupt flag for capture input */
#define PWM_IR_PWMCAPn(n)        ((uint32_t)(1<<(n+4)))
/**  IR register mask */
#define PWM_IR_BITMASK            ((uint32_t)(0x0000073F))

/**********************************************************************
* TCR register definitions
**********************************************************************/
/** TCR register mask */
#define PWM_TCR_BITMASK                ((uint32_t)(0x0000000B))
#define PWM_TCR_COUNTER_ENABLE      ((uint32_t)(1<<0)) /*!< PWM Counter Enable */
#define PWM_TCR_COUNTER_RESET       ((uint32_t)(1<<1)) /*!< PWM Counter Reset */
#define PWM_TCR_PWM_ENABLE          ((uint32_t)(1<<3)) /*!< PWM Enable */

/**********************************************************************
* CTCR register definitions
**********************************************************************/
/** CTCR register mask */
#define PWM_CTCR_BITMASK            ((uint32_t)(0x0000000F))
/** PWM Counter-Timer Mode */
#define PWM_CTCR_MODE(n)            ((uint32_t)(n&0x03))
/** PWM Capture input select */
#define PWM_CTCR_SELECT_INPUT(n)    ((uint32_t)((n&0x03)<<2))

/**********************************************************************
* MCR register definitions
**********************************************************************/
/** MCR register mask */
#define PWM_MCR_BITMASK                ((uint32_t)(0x001FFFFF))
/** generate a PWM interrupt when a MATCHn occurs */
#define PWM_MCR_INT_ON_MATCH(n)     ((uint32_t)(1<<(((n&0x7)<<1)+(n&0x07))))
/** reset the PWM when a MATCHn occurs */
#define PWM_MCR_RESET_ON_MATCH(n)   ((uint32_t)(1<<(((n&0x7)<<1)+(n&0x07)+1)))
/** stop the PWM when a MATCHn occurs */
#define PWM_MCR_STOP_ON_MATCH(n)    ((uint32_t)(1<<(((n&0x7)<<1)+(n&0x07)+2)))

/**********************************************************************
* CCR register definitions
**********************************************************************/
/** CCR register mask */
#define PWM_CCR_BITMASK                ((uint32_t)(0x0000003F))
/** PCAPn is rising edge sensitive */
#define PWM_CCR_CAP_RISING(n)          ((uint32_t)(1<<(((n&0x2)<<1)+(n&0x1))))
/** PCAPn is falling edge sensitive */
#define PWM_CCR_CAP_FALLING(n)         ((uint32_t)(1<<(((n&0x2)<<1)+(n&0x1)+1)))
/** PWM interrupt is generated on a PCAP event */
#define PWM_CCR_INT_ON_CAP(n)          ((uint32_t)(1<<(((n&0x2)<<1)+(n&0x1)+2)))

/**********************************************************************
* PCR register definitions
**********************************************************************/
/** PCR register mask */
#define PWM_PCR_BITMASK            (uint32_t)0x00007E7C
/** PWM output n is a single edge controlled output */
#define PWM_PCR_PWMSELn(n)       ((uint32_t)(((n&0x7)<2) ? 0 : (1<<n)))
/** enable PWM output n */
#define PWM_PCR_PWMENAn(n)       ((uint32_t)(((n&0x7)<1) ? 0 : (1<<(n+8))))

/**********************************************************************
* LER register definitions
**********************************************************************/
/** LER register mask*/
#define PWM_LER_BITMASK                ((uint32_t)(0x0000007F))
/** PWM MATCHn register update control */
#define PWM_LER_EN_MATCHn_LATCH(n)   ((uint32_t)((n<7) ? (1<<n) : 0))


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
void Chip_PWM_Init(LPC_PWM_T *pPWM);


/*********************************************************************//**
 * @brief        De-initializes the PWM peripheral registers to their
*                  default reset values.
 * @param[in]    pPWM    PWM peripheral selected, should be LPC_PWM1
 * @return         None
 **********************************************************************/
void Chip_PWM_DeInit (LPC_PWM_T *pPWM);


/*********************************************************************//**
 * @brief        Get the PWM peripheral clock
 * @param[in]    pPWM    PWM peripheral selected, should be LPC_PWM1
 * @return         None
 **********************************************************************/
uint32_t Chip_PWM_GetClock(LPC_PWM_T *pPWM);


#ifdef __cplusplus
}
#endif

#endif /* __PMU_17XX_40XX_H_ */
