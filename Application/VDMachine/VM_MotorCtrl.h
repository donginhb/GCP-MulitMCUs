#ifndef  __VM_MOTOR_CTRL_H
#define  __VM_MOTOR_CTRL_H

#include "DataType/DataType.h"

#ifdef __cplusplus
extern "C"
{
#endif

void VM_SetConfig(void);
    
//升降电机运动控制 fPos mm fSpeed mm/s,bCheckLimit-是否检测极限
uBit16 VM_MoveLiftMotor(float32 fPos, float32 fSpeed, uBit8 bCheckLimit);

void VM_MoveLiftMotorTest(void);

#ifdef __cplusplus
}
#endif

#endif /* __VM_MOTOR_CTRL_H */
