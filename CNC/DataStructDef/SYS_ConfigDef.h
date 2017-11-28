
/*
Copyright (c) 2013, 东莞华科精机有限公司 All rights reserved.

文件名称：SYS_DataConfigDef.h 

摘    要：关于系统配置的宏定义文件，主要定义系统支持的一些主要参数，如：最大坐标系，轴数等等

运行环境：Windows Xp / WINCE / LPC43xx / LPC1751

修改描述：

当前版本：2.0

修改作者：毛军

修改日期：2013年12月07日

//-----------------------------------------------------------------------------------------------
修改描述：

当前版本：2.1

修改作者：毛军

修改日期：2014年6月21日

*/

#ifndef _SYS_DATA_CONFING_DEF_
#define _SYS_DATA_CONFING_DEF_

//----------------------------------------------系统运行能力配置------------------------------------------------------

#define ENABLE_CAN_BUS  (1)    //使能CAN总线设备
#define ENABLE_MIII_BUS (0)    //使能MIII总线设备

//#define ENABLE_AXIS_PITCH_CMP_PARM  （1）     //使能轴螺距补偿参数




//----------------------------------------------------------------------------------------------------


//
#define DEV_MAX_COUNT                    (8)//(20)    //系统可连接的轴及IO模块最大数量
#define AXIS_MAX_COUNT                    (8) //(16)    //系统最大支持的轴数量
#define CRDSYS_AXIS_MAX_COUNT            (8)        //单个坐标系支持的最大轴数
#define CRDSYS_MAX_COUNT                (1) //(4)        //最大坐标系数量(最大通道数量)
#define AXIE_MAP_MOTOR_MAX_COUNT        (2)        //一个坐标轴所能映射的电机最大数量


//定义总线类型
#define DEV_BUS_TYPE_NULL                (0)            //未定义
#define DEV_BUS_TYPE_CAN                (1)            //CAN总线
#define DEV_BUS_TYPE_MECHALINK_III        (2)            //M-III总线
#define DEV_BUS_TYPE_ONBOARD            (3)            //板载设备

/*
//定义可用的总线编号
#define DEV_BUS_NO_CAN0                    (0)            //CAN0
#define DEV_BUS_NO_CAN1                    (1)            //CAN1
*/

//定义设备类型
#define DEV_TYPE_UNDEF                    (-1)        //未定义
#define DEV_TYPE_COUNT                    (6)        //设备种类数量
#define DEV_TYPE_AXIS                    (0)            //轴卡
#define DEV_TYPE_OUTPUT                    (1)            //输出卡
#define DEV_TYPE_INPUT                    (2)            //输入卡
#define DEV_TYPE_ADDA                    (3)            //AD/DA卡
#define DEV_TYPE_IO                        (4)            //输入+输出卡
#define DEV_TYPE_SV                        (5)            //伺服


//----------------------------------------------------------------------------------------------------
#define DEV_INVALID_NO                    (0xFF)        //设备无效编号
#define DEV_INVALID_ID                    (0x00)        //设备无效ID


//----------------------------------------------------------------------------------------------------
#define CRD_MASK_01   (0x01)
#define CRD_MASK_02   (0x02)
#define CRD_MASK_03   (0x04)
#define CRD_MASK_04   (0x08)

#define CRD_MASK_ALL  (0x03)

#define AXIS_MASK_01   (0x01)
#define AXIS_MASK_02   (0x02)
#define AXIS_MASK_03   (0x04)
#define AXIS_MASK_04   (0x08)
#define AXIS_MASK_05   (0x10)
#define AXIS_MASK_06   (0x20)
#define AXIS_MASK_07   (0x40)
#define AXIS_MASK_08   (0x80)

#define AXIS_MASK_ALL  (0xff)

#define AXIS_CMP_DIR_COUNT    (2)
#define AXIS_CMP_MAX_POINT    (128)    //最大补偿点数

//定义轴类型
#define AXIS_TYPE_NULL            (0)            //未安装
#define AXIS_TYPE_LINE            (1)            //直线轴
#define AXIS_TYPE_ROTATE        (2)            //旋转轴(角度值任意)
#define AXIS_TYPE_ROTATE_360    (3)            //旋转轴(角度值为0-360)

#define HAND_WHELL_DISABLE        (0)
#define HAND_WHELL_ENABLE        (1)

//------------------------------------------------IO板硬件配置数据定义---------------------------------------------------
#define IO_MODULE_MAX_COUNT        (AXIS_MAX_COUNT)    //系统最大IO模块数量
#define IO_MODULE_POINT_COUNT        (16)            //单个IO模块输入或输出点数量

//定义每个ADDA模块中AD输入的通道数量
#define ADDA_MODULE_MAX_COUNT    (AXIS_MAX_COUNT)    //系统最大ADDA模块数量
#define AD_CHANNEL_COUNT        (6)
#define DA_CHANNEL_COUNT        (4)

#define AD_CHANNEL_01    (0x01)
#define AD_CHANNEL_02    (0x02)
#define AD_CHANNEL_03    (0x04)
#define AD_CHANNEL_04    (0x08)
#define AD_CHANNEL_05    (0x10)
#define AD_CHANNEL_06    (0x20)

#define AD_CHANNEL_ALL    (0x3F)

#define DA_CHANNEL_01    (0x01)
#define DA_CHANNEL_02    (0x02)
#define DA_CHANNEL_03    (0x04)
#define DA_CHANNEL_04    (0x08)
         
#define DA_CHANNEL_ALL    (0x0F)

#define HIGH_SPEED_DA_CHANNEL_COUNT        (2)    //系统支持2路高速DA输出

#define HIGH_SPEED_IO_INPUT            (0)     //高速IO输入
#define HIGH_SPEED_IO_OUTPUT            (1)     //高速IO输出

#define VERSION_MAX_LEN        (8)            //版本查询信息的最大长度

#define SPINDLE_CW            (0)        //主轴正转
#define SPINDLE_CCW            (1)        //主轴反转

#define PROG_BUFFER_CLOSE    (0)            //程序缓存关闭
#define PROG_BUFFER_OPEN    (1)            //程序缓存打开

#define USER_BUFFER_CLOSE    (0)            //用户缓存关闭
#define USER_BUFFER_OPEN    (1)            //用户缓存打开

//
#define SYS_UPDATE_DATA_BUF_LEN     (520)       //系统程序升级数据缓冲区长度


//------------------------------------------------------------------------------------

#define SYS_IPO_TIME_CYCLE_DEFAULT        (1000)        //定义默认的系统插补周期（us）



#endif //#ifndef _SYS_DATA_CONFING_DEF_

