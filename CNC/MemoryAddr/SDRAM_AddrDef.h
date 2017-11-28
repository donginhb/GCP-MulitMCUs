/*
Copyright (c) 2015, 东莞华科精机有限公司 All rights reserved.

文件名称：SDRAM_AddrDef.h

摘    要：SDRAM地址定义

运行环境：LPC43xx 

修改描述：

当前版本：1.0

修改作者：毛军

修改日期：2015年10月16日
///////////////////////////////////////
修改描述：
        1.SDRAM最后2M数据区以太网数据缓存区后定义 16*1024字节(共4000个4字节寄存器数据区)

修改作者：Alan

修改日期：2016-4-14
*/

#ifndef SDRAM_ADDR_DEF_H
#define SDRAM_ADDR_DEF_H


//定义SDRAM地址段
#define SDRAM_START_ADDR                    (0x28000000)            //SDRAM起始地址
#define SDRAM_TOTAL_LEN                        (0x02000000)            //SDRAM总长度(32M 8bit)



//定义插补缓冲区地址段
#define IPO_BUF_START_ADDR                    (0x28000000)            //插补缓冲区起始地址
#define IPO_BUF_TOTAL_LEN                    (2*1024*1024)            //定义插补缓冲区总长度(8bit)


#define RAPID_SPEED_CTRL_TABLE_BUF_ADDR        (0x28200000)            //快移模式速度规划表地址
#define RAPID_SPEED_CTRL_TABLE_BUF_LEN        (2*1024*1024)            //快移模式速度规划表长度(8bit)


#define    JOG_POSCTRL_TABLE_BUF_START_ADDR    (0x28400000)            //手动位置控制速度规划表缓冲区起始地址
#define    JOG_POSCTRL_TABLE_BUF_TOTAL_LEN        (2*1024*1024)            //手动位置控制速度规划表缓冲区总长度


//定义程序缓冲区地址段
#define    PROG_BUF_START_ADDR                    (0x28600000)            //程序缓冲区起始地址
#define    PROG_BUF_TOTAL_LEN                    (10*1024*1024)                //程序缓冲区总长度


//定义用户自定义程序缓冲区地址段
#define    USER_BUF_START_ADDR                    (0x29000000)            //用户自定义数据缓冲区起始地址
#define    USER_BUF_TOTAL_LEN                    (12*1024*1024)            //用户自定义数据缓冲区总长度


//定义解释器内部数据缓冲区地址段
#define    IPR_BUF_START_ADDR                    (0x29C00000)            //解释器内部数据缓冲区起始地址
#define    IPR_BUF_TOTAL_LEN                    (2*1024*1024)            //解释器内部数据缓冲区总长度


//(剩余2M)

//通信模块内部数据缓冲区
#define    CMU_BUF_START_ADDR                    (0x29E00000)            //通信模块内部数据缓冲区起始地址
#define    CMU_BUF_TOTAL_LEN                    (3*1024)                //通信模块内部数据缓冲区总长度

//以太网模块内部数据缓冲区
#define    ENET_BUF_START_ADDR                    (0x29E00C00)            //以太网模块内部数据缓冲区起始地址
#define    ENET_BUF_TOTAL_LEN                    (16*1024)                //以太网模块内部数据缓冲区总长度

#define UREG_BUF_START_ADDR                 (0x29E04C00)            //用户寄存器数据区
#define UREG_BUF_TOTAL_LEN                  (16*1024)


#ifdef __cplusplus
extern "C" {
#endif


    int SDRAM_Init();



#ifdef __cplusplus
}
#endif

#endif

