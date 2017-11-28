/*    
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：

摘    要：定义CAN总线设备驱动层对外接口

运行环境：IAR6.6

修改描述：

当前版本：1.0 

修改作者： 

修改了内容：

修改日期： 

原 作 者：Alan

完成日期：2015-11-5        
*/
#ifndef _CAN_DEV_INTERFACE_H_
#define _CAN_DEV_INTERFACE_H_

#include "CAN_DEV_DataStructDef.h"

#include "..\\..\\..\\SysPeripheral\\CAN\\CanDataStructDef.h"
#include "..\\DEV_DriveStructDef.h"

//CAN总线驱动输出接口--------------------------
void CAN_DEV_GetBusDriver(BUS_DEVICE_DRIVE *pDeviceDrive);

//外部依赖接口挂接----------------------------
void CAN_DEV_SetComInterface(CAN_INTERFACE sCanInterface);
void CAN_DEV_SetExternalApi(CAN_DEV_EX_INTERFACE sCanDevExapiTable);
#endif