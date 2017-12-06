#ifndef _BRD_AXISDEV_H_
#define _BRD_AXISDEV_H_

#include "../../DataStructDef/SYS_DataStructDef.h"
#include "../../DataStructDef/AXIS_DataStructDef.h"

//板载轴设备初始化，总线初始化时调用
void BRD_AxisInit(void);    

//获取轴状态数据
void BRD_AxisGetAxisState(AXIS_STATE_DATA *pAxisStateData);

//设置轴控参数
void BRD_AxisSetAxisParm(const AXIS_PARM *pAxisParm);

//设置系统控制参数
uBit32 BRD_AxisSetSysCtrlParm(const SYS_CTRL_PARM *pSysCtrlParm);

//设置进给
uBit32 BRD_AxisSetFeed(Bit32 lPulseInc);

//轴指令处理
uBit32 BRD_AxisSetCmd(uBit32 ulCmdType, Bit32 lCmdPara1, Bit32 lCmdPara2);

#endif

