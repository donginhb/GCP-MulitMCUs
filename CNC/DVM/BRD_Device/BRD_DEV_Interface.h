/*    
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：

摘    要：

运行环境：IAR6.6

修改描述：

当前版本：1.0 

修改作者： 

修改了内容：

修改日期： 

原 作 者：Alan

完成日期：2015-11-5        
*/
#ifndef _BRD_DEV_INTERFACE_H_
#define _BRD_DEV_INTERFACE_H_

#include "../DEV_DriveStructDef.h"

//板载总线驱动输出接口--------------------------
void BRD_DEV_GetBusDriver(BUS_DEVICE_DRIVE *pDeviceDrive);

#endif