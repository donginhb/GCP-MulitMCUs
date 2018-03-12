#ifndef __APPLICATION_H
#define __APPLICATION_H


#if defined(BOOTLOADER)
#include "Application/Bootloader/BT_MainProc.h"
#endif

#if defined(GCP_APP)
#include "Application/GCPTest/GCP_MainProc.h"
#include "Application/GCPTest/GCP_AppVersion.h"
#elif defined(CN_APP)
#include "Application/CNMachine/CN_MainProc.h"
#include "Application/CNMachine/CN_AppVersion.h"
#elif defined(WFC_APP)
#include "Application/WIFIClock/WFC_MainProc.h"
#include "Application/WIFIClock/WFC_AppVersion.h"
#elif defined(MD_APP)
#include "Application/MotorDriver/MD_MainProc.h"
#include "Application/MotorDriver/MD_AppVersion.h"
#elif defined(LC_APP)
#include "Application/LEDController/LC_MainProc.h"
#include "Application/LEDController/LC_AppVersion.h"
#elif defined(BMC_APP)
#include "Application/BLEMotorCtrl/BMC_MainProc.h"
#include "Application/BLEMotorCtrl/BMC_AppVersion.h"
#elif defined(IRB_APP)
#include "Application/IRDoorBell/IRB_MainProc.h"
#include "Application/IRDoorBell/IRB_AppVersion.h"
#elif defined(BS08M_APP)
#include "Application/BS08M/BS08M_MainProc.h"
#include "Application/BS08M/BS08M_AppVersion.h"
#elif defined(BS08S_APP)
#include "Application/BS08S/BS08S_MainProc.h"
#include "Application/BS08S/BS08S_AppVersion.h"
#elif defined(WB01_APP)
#include "Application/WB01/WB01_MainProc.h"
#include "Application/WB01/WB01_AppVersion.h"
#endif




#if defined(BOOTLOADER)
#define APP_Init()          BT_Init()
#define APP_MainProc()      BT_MainProc()

#elif defined(GCP_APP)
#define APP_Init()          GCP_Init()
#define APP_MainProc()      GCP_MainProc()

#elif defined(CN_APP)
#define APP_Init()          CN_Init()
#define APP_MainProc()      CN_MainProc()

#elif defined(WFC_APP)
#define APP_Init()          WFC_Init()
#define APP_MainProc()      WFC_MainProc()

#elif defined(MD_APP)
#define APP_Init()          MD_Init()
#define APP_MainProc()      MD_MainProc()

#elif defined(LC_APP)
#define APP_Init()          LC_Init()
#define APP_MainProc()      LC_MainProc()

#elif defined(BMC_APP)
#define APP_Init()          BMC_Init()
#define APP_MainProc()      BMC_MainProc()

#elif defined(IRB_APP)
#define APP_Init()          IRB_Init()
#define APP_MainProc()      IRB_MainProc()

#elif defined(BS08M_APP)
#define APP_Init()          BS08M_Init()
#define APP_MainProc()      BS08M_MainProc()

#elif defined(BS08S_APP)
#define APP_Init()          BS08S_Init()
#define APP_MainProc()      BS08S_MainProc()

#elif defined(WB01_APP)
#define APP_Init()          WB01_Init()
#define APP_MainProc()      WB01_MainProc()

#else 
#define APP_Init()   
#define APP_MainProc()

#endif


#endif /* __APPLICATION_H */
