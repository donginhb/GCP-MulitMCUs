/******************************************************************************
 * @file     pid.h
 * @brief    pid control
 *           
 *
 * @version  1.0
 * @date     2015-12-10
 *
 * @author   Sundy
 *
 * @note
 * @Copyright (C) 2015 Shanghai Eastsoft Microelectronics C0., Ltd.
 ******************************************************************************/
#ifndef __PID_H
#define __PID_H

#include <stdint.h>

#define DEFAULT_PID_INTEGRATION_LIMIT  50.0

typedef struct
{
	float desired;      //< set point
	float error;        //< error  Îó²î
	float prevError;    //< previous error
	float integ;        //< integral
	float deriv;        //< derivative
	float kp;           //< proportional gain
	float ki;           //< integral gain
	float kd;           //< derivative gain
	float outP;         //< proportional output (debugging)
	float outI;         //< integral output (debugging)
	float outD;         //< derivative output (debugging)
	float iLimitMax;    //< integral limit
	float iLimitLow;    //< integral limit
	float dt;           //< delta-time dt
} pid_object_t;

/******************************************************************************
 * @brief    pid_init
 *           
 * @note
 *
 * @param	 pid
 *			 kp
 *			 kd
 *			 dt
 * @retval	 None
 ******************************************************************************/
void pid_init(pid_object_t *pid, float kp, float ki, float kd, float dt);

/******************************************************************************
 * @brief    pid_update
 *           
 * @note
 *
 * @param	 pid
 *			 desired
 *			 measured
 * @retval	 None
 ******************************************************************************/
int16_t pid_update(pid_object_t *pid, float desired, float measured);

#endif /* __PID_H */

