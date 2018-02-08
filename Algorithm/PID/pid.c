/******************************************************************************
 * @file     pid.c
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
#include <stdint.h>
#include "pid.h"

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
 *
 * @version  1.0
 * @date     2015-12-02
 * @author   sundy
 ******************************************************************************/
void pid_init(pid_object_t *pid, float kp, float ki, float kd, float dt)
{
	pid->error     = 0.0;
	pid->prevError = 0.0;
	pid->integ     = 0.0;
	pid->deriv     = 0.0;
	pid->desired = 0.0;
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;
	pid->iLimitMax = DEFAULT_PID_INTEGRATION_LIMIT;
	pid->iLimitLow = -DEFAULT_PID_INTEGRATION_LIMIT;
	pid->dt        = dt;
}

/******************************************************************************
 * @brief    pid_update
 *           
 * @note
 *
 * @param	 pid
 *			 desired
 *			 measured
 *			 is_yaw_error
 * @retval	 None
 *
 * @version  1.0
 * @date     2015-12-02
 * @author   sundy
 ******************************************************************************/
int16_t pid_update(pid_object_t *pid, float desired, float measured)
{
	int16_t output;

	pid->desired = desired;
	pid->error = pid->desired - measured;
	
	pid->integ += pid->error * pid->dt;
	if (pid->integ > pid->iLimitMax) {
		pid->integ = pid->iLimitMax;
	}
	else if (pid->integ < pid->iLimitLow) {
		pid->integ = pid->iLimitLow;
	}

	pid->deriv = (pid->error - pid->prevError) / pid->dt;

	pid->outP = pid->kp * pid->error;
	pid->outI = pid->ki * pid->integ;
	pid->outD = pid->kd * pid->deriv;

	output = (int16_t)(pid->outP + pid->outI + pid->outD);

	pid->prevError = pid->error;

	return output;
}

