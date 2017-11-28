/*
Copyright (c) 2013, 东莞华科精机有限公司 All rights reserved.

文件名称：Version.h

摘    要：IPO版本声明

运行环境：IAR6.6

修改描述：

当前版本：1.0

修改作者：

修改了内容：

修改日期：

原 作 者：Alan

完成日期：2015-12-14
*/

/*******************************版本说明***********************************
例如：NO20103A[NO20-10-3-A]
NO20 :产品型号, NO20标示通用数据系统2.0，ST01表示花样机数控系统1.0
10        :软件版本
3        :组件标示
            (0)            //上位机程序
            (1)            //逻辑控制（M0）
            (2)            //插补运算（M4）
            (3)            //轴设备
            (4)            //OUTPUT设备
            (5)            //INPUT设备
            (6)            //ADDA设备
A        :特殊标示，A标示APP程序 B标示BOOTLOADER程序


NO20102A---初始版本
*************************************************************************/
#ifndef VERSION_H_
#define VERSION_H_

//#define APP_VER_STR                "NO20102A"


//

#define APP_VER_STR                "NO20XXXA"
#define APP_VER_PART            SYS_PART_SLC   //组件号
#define APP_VER_NO0             (2)
#define APP_VER_NO1             (11)


#define MAINCTRL_TYPE_BOARD10        (0) //主控卡
#define MAINCTRL_TYPE_BOARD20        (1) //主控板(2.0,CAN硬件接口跟主控卡不一样)
#define MAINCTRL_TYPE_BOARD21        (2) //主控板(2.1,CAN硬件接口跟主控卡一样，高速IO跟主控版2.0不一样)
#define MAINCTRL_TYPE               (MAINCTRL_TYPE_BOARD10)




#endif
