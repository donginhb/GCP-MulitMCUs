#ifndef __CNC_ADDR_DEF_H                                
#define __CNC_ADDR_DEF_H

#include "CNC/CNCSYS/CNCSYS_Config.h"


#if (CNC_USING_EXT_SRAM)

#include "SDRAM_AddrDef.h"

//定义插补缓冲区地址段
#define IPO_BUF_START_ADDR                  (SDRAM_IPO_BUF_START_ADDR)          //插补缓冲区起始地址
#define IPO_BUF_TOTAL_LEN                   (SDRAM_IPO_BUF_TOTAL_LEN)           //定义插补缓冲区总长度(8bit)

//快移模式速度规划表地址
#define RAPID_SPEED_CTRL_TABLE_BUF_ADDR     (SDRAM_RAPID_SPEED_CTRL_TABLE_BUF_ADDR )  //快移模式速度规划表地址
#define RAPID_SPEED_CTRL_TABLE_BUF_LEN      (SDRAM_RAPID_SPEED_CTRL_TABLE_BUF_LEN  )  //快移模式速度规划表长度(8bit)

//手动位置控制速度规划表缓冲区地址
#define JOG_POSCTRL_TABLE_BUF_START_ADDR    (SDRAM_JOG_POSCTRL_TABLE_BUF_START_ADDR)  //手动位置控制速度规划表缓冲区起始地址
#define JOG_POSCTRL_TABLE_BUF_TOTAL_LEN     (SDRAM_JOG_POSCTRL_TABLE_BUF_TOTAL_LEN )  //手动位置控制速度规划表缓冲区总长度

//定义程序缓冲区地址段
#define PROG_BUF_START_ADDR                 (SDRAM_PROG_BUF_START_ADDR    )     //程序缓冲区起始地址
#define PROG_BUF_TOTAL_LEN                  (SDRAM_PROG_BUF_TOTAL_LEN     )     //程序缓冲区总长度

//定义用户自定义程序缓冲区地址段
#define USER_BUF_START_ADDR                 (SDRAM_USER_BUF_START_ADDR    )     //用户自定义数据缓冲区起始地址
#define USER_BUF_TOTAL_LEN                  (SDRAM_USER_BUF_TOTAL_LEN     )     //用户自定义数据缓冲区总长度

//定义解释器内部数据缓冲区地址段
#define IPR_BUF_START_ADDR                  (SDRAM_IPR_BUF_START_ADDR     )     //解释器内部数据缓冲区起始地址
#define IPR_BUF_TOTAL_LEN                   (SDRAM_IPR_BUF_TOTAL_LEN      )     //解释器内部数据缓冲区总长度

//系统参数内部数据缓冲区
#define AHB_BUF_START_ADDR                  (CNC_SRAM_BUFF_ADDR + 0xC00)        //系统参数数据缓冲区起始地址
#define AHB_BUF_TOTAL_LEN                   (4*1024)                            //系统参数数据缓冲区总长度

//通信模块内部数据缓冲区
#define CMU_BUF_START_ADDR                  (SDRAM_CMU_BUF_START_ADDR     )     //通信模块内部数据缓冲区起始地址
#define CMU_BUF_TOTAL_LEN                   (SDRAM_CMU_BUF_TOTAL_LEN      )     //通信模块内部数据缓冲区总长度

//以太网模块内部数据缓冲区
#define ENET_BUF_START_ADDR                 (SDRAM_ENET_BUF_START_ADDR    )     //以太网模块内部数据缓冲区起始地址
#define ENET_BUF_TOTAL_LEN                  (SDRAM_ENET_BUF_TOTAL_LEN     )     //以太网模块内部数据缓冲区总长度

//用户寄存器内部数据缓冲区
#define UREG_BUF_START_ADDR                 (SDRAM_UREG_BUF_START_ADDR    )     //用户寄存器数据区起始地址
#define UREG_BUF_TOTAL_LEN                  (SDRAM_UREG_BUF_TOTAL_LEN     )     //用户寄存器数据区总长度

#else 

//内部SRAM地址缓冲区基地址
#define CNC_START_ADDR                      (CNC_SRAM_BUFF_ADDR)                //CNC起始地址
#define CNC_TOTAL_LEN                       (CNC_SRAM_BUFF_LEN)                 //CNC缓冲区总共长度

//定义插补缓冲区地址段
#define IPO_BUF_START_ADDR                  (CNC_SRAM_BUFF_ADDR)                //插补缓冲区起始地址
#define IPO_BUF_TOTAL_LEN                   (1024)                              //定义插补缓冲区总长度(8bit)

//快移模式速度规划表地址
#define RAPID_SPEED_CTRL_TABLE_BUF_ADDR     (CNC_SRAM_BUFF_ADDR)                //快移模式速度规划表地址
#define RAPID_SPEED_CTRL_TABLE_BUF_LEN      (0)                                 //快移模式速度规划表长度(8bit)

//手动位置控制速度规划表缓冲区地址
#define JOG_POSCTRL_TABLE_BUF_START_ADDR    (CNC_SRAM_BUFF_ADDR)                //手动位置控制速度规划表缓冲区起始地址
#define JOG_POSCTRL_TABLE_BUF_TOTAL_LEN     (0)                                 //手动位置控制速度规划表缓冲区总长度

//定义程序缓冲区地址段            
#define PROG_BUF_START_ADDR                 (CNC_SRAM_BUFF_ADDR + 0x700)        //程序缓冲区起始地址
#define PROG_BUF_TOTAL_LEN                  (16)                                //程序缓冲区总长度

//定义用户自定义程序缓冲区地址段
#define USER_BUF_START_ADDR                 (CNC_SRAM_BUFF_ADDR + 0x800)        //用户自定义数据缓冲区起始地址
#define USER_BUF_TOTAL_LEN                  (16)                                //用户自定义数据缓冲区总长度

//定义解释器内部数据缓冲区地址段  ---没用到，可直接删除
#define IPR_BUF_START_ADDR                  (CNC_SRAM_BUFF_ADDR)                //解释器内部数据缓冲区起始地址
#define IPR_BUF_TOTAL_LEN                   (0)                                 //解释器内部数据缓冲区总长度

//系统参数内部数据缓冲区
#define AHB_BUF_START_ADDR                  (CNC_SRAM_BUFF_ADDR + 0xC00)        //系统参数数据缓冲区起始地址
#define AHB_BUF_TOTAL_LEN                   (4*1024)                            //系统参数数据缓冲区总长度

//通信模块内部数据缓冲区
#define CMU_BUF_START_ADDR                  (CNC_SRAM_BUFF_ADDR + 0x1C00)       //通信模块内部数据缓冲区起始地址
#define CMU_BUF_TOTAL_LEN                   (3*1024)                            //通信模块内部数据缓冲区总长度

//以太网模块内部数据缓冲区  --以太网没到就不用定位缓冲器，不要初始化以太网模块
#define ENET_BUF_START_ADDR                 (CNC_SRAM_BUFF_ADDR + 0x2600)       //以太网模块内部数据缓冲区起始地址
#define ENET_BUF_TOTAL_LEN                  (16*1024)                           //以太网模块内部数据缓冲区总长度

//用户寄存器内部数据缓冲区
#define UREG_BUF_START_ADDR                 (CNC_SRAM_BUFF_ADDR)                //用户寄存器数据区起始地址
#define UREG_BUF_TOTAL_LEN                  (0)                                 //用户寄存器数据区总长度

#endif /* CNC_USING_EXT_SRAM */


#endif /* __CNC_ADDR_DEF_H */
