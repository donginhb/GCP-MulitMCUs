#ifndef _IPR_ERROR_CODE_H_
#define _IPR_ERROR_CODE_H_

//第1部分-程序调用 返回 错误代码信息--(待完善)------------------------------------------------------------------
#define IPR_CMD_IPR_ENABLE            (1) //解释器使能 IPR_SetPpiEnable
#define IPR_CMD_IPR_INIT            (2) //解释器初始化 IPR_CncModuleInit
#define IPR_CMD_IPR_CANCEL            (3) //解释器取消程序 IPR_Cancel
#define IPR_CMD_IPR_PAUSE            (4) //解释器暂停程序 IPR_Pause
#define IPR_CMD_IPR_CYCLE            (5) //解释器程序循环执行 IPR_Cycle
#define IPR_CMD_IPR_PROG_RESET        (6) //解释器重新开始 IPR_ProgReset
#define IPR_CMD_IPR_PUT_GCODE        (7) //发送加工G代码到解释器IPR_PutGCodeToIprBuf
#define IPR_CMD_IPR_PUT_CCGCODE        (8) //发送固定循环G代码到解释器IPR_PutCannedCycleToIprBuf
#define IPR_CMD_IPR_RND_START        (9) //解释器任意行开始解释IPR_RndStart
#define IPR_CMD_IPR_SET_COORD        (10) //设置坐标系IPR_SetCoord(G92 G54~G59?)

#define IPR_CMD_SET_TOOL_DATA        (11) //设置刀具数据IPR_SetToolData
#define IPR_CMD_GET_TOOL_DATA        (12) //获取刀具数据IPR_GetToolData
#define IPR_CMD_SET_FLASH_CONF        (13) //设置Flash存储其实地址和大小IPR_SetFlashConf
#define IPR_CMD_SET_IPR_USER_BASE    (14) //设置用户缓冲区的地址和大小IPR_SetIprUserBufBase
#define IPR_CMD_SET_IPR_VAR_BASE    (15) //设置变量缓冲区的地址和大小IPR_SetIprVarBufBase
#define IPR_CMD_GET_BP_DATA_LEN        (16) //获取断点数据的长度IPR_GetBpDataLen
#define IPR_CMD_GET_BP_DATA            (17) //获取断点数据IPR_GetBpData
#define IPR_CMD_SET_BP_DATA            (18) //设置（恢复）断点数据IPR_SetBpData
#define IPR_CMD_SET_TOOL            (19) //设置当前使用的刀具号IPR_SetTool
#define IPR_CMD_SET_BASE_TOOL        (20) //设置标刀的刀具号IPR_SetBaseTool
#define IPR_CMD_UPDATE_CRD_DATA        (21) //更新通道数据 IPR_UpdateCrdData
#define IPR_CMD_IPR_WORKING            (22) //行解释


//-------------------------------------------------------------------------------
#define IPR_ERR_VAR_MEM_INIT        (1) //解释器变量内存初始化错误
#define IPR_ERR_VAR_MEM_MALLOC        (2) //解释器变量内存分配错误
#define IPR_ERR_PPI_INIT_FAIL        (3) //初始化错误
#define IPR_ERR_IPR_PAUSE_FAIL        (4) //解释器停止错误
#define IPR_ERR_IPR_CANCEL_FAIL        (5) //解释器取消错误



//--------------------------------------------------------------------------------
//第2部分-解释过程的错误信息------------------------------------------------------

#define    E_FORMAT            (51)        /*"程序格式错"*/
#define    E_PROCNO            (52)        /*"程序号错"*/
#define    E_BADADS            (53)        /*"非法地址字"*/
#define E_SYNTAX            (54)        /*"程序语法错"*/
#define    E_SUBOVER            (55)        /*"子程序嵌套太多"*/
#define E_DATATYPE            (56)        /*"数据类型错"*/
#define    E_CTRLOVER            (57)        /*"分支太多"*/
#define    E_PLANEALT            (58)        /*"平面不能更改"*/
#define E_SETUPTOOL            (59)        /*"刀补建立出错"*/
#define E_CANCELTOOL        (60)    /*"刀补撤销出错"*/

#define E_ARCDATA            (61)    /*"圆弧数据错"*/
#define    E_INTERFERENCE        (62)    /*"刀具干涉"*/
#define    E_ROUNDING            (63)    /*"倒角值太大"*/
#define    E_THREAD            (64)    /*"螺纹数据错"*/
#define E_TAP                (65)    /*"攻丝深度错"*/
#define    E_THREAD_NO_PPR        (66)    /*"无螺纹主轴编码器"*/
#define E_G29                (67)    /*"G29无中间点"*/
#define    E_G54_G59            (68)    /*"G54位置无效,请重新设置"*/
#define    E_G92                (69)    /*"G92零点位置无效,请重新对刀"*/
#define    E_MIRROR            (70)    /*"镜像错"*/

#define    E_SCALE                (71)    /*"缩放错"*/
#define    E_ROTATE            (72)    /*"旋转错"*/
#define    E_POLAR                (73)    /*"极坐标定义错"*/
#define    E_CYLINDER            (74)    /*"圆柱坐标定义错"*/
#define    E_LOADBP            (75)    /*"恢复断点错"*/
#define E_BPAWAY            (76)    /*"机床不在断点位置"*/
#define E_RESOUSE            (77)    /*"内部资源不够"*/
#define E_MSTK                (78)    /*"解释器内部堆栈错"*/
#define E_ENDIF                (79)    /*"endif语句错"*/
#define E_ELSE                (80)    /*"else语句错"*/

#define E_NOENDIF            (81)    /*"无endif"*/
#define    E_ENDW                (82)    /*"endw语句错"*/
#define E_NOENDW            (83)    /*"无endw"*/
#define    E_CONDITION            (84)    /*"条件错"*/
#define    E_LVALUE            (85)    /*"变量名错"*/
#define    E_ASSIGN            (86)    /*"不能赋值"*/
#define    E_VAR                (87)    /*"宏变量名错"*/
#define    E_WORKCOORD            (88)    /*"位置指令值错"*/
#define E_TOOCOMPLEX        (89)    /*"表达式太复杂"*/
#define    E_OVERFLOW            (90)    /*"运算溢出"*/

#define    E_DOMAIN            (91)    /*"函数自变量错"*/
#define E_FP                (92)    /*"浮点运算错"*/
#define    E_DIV0                (93)    /*"被零除"*/
#define E_G92_CSTUP            (94)    /*"G92零点位置无效,请重新对刀"*/
#define E_NEED_HOME            (95)    /*"未回参考点或必须重新回参考点"*/
#define E_CUT_CSTUP            (96)    /*"刀偏置无效,请重新设置"*/
#define E_CUT_CSTUP_NORC    (97)    /*"未选择标刀，刀偏值无效,请重新设置"*/
#define E_COORD_ALT            (98)    /*"不能更改坐标系"*/
#define E_COORD_NSTUP        (99)    /*"未建立工件坐标系"*/
#define E_COMPC_ARGS        (100)    /*"复合循环参数错"*/

#define E_COMPC_CC            (101)    /*"进入复合循环前应取消刀尖补偿"*/
#define E_COMPC_BODY        (102)    /*"找不到复合循环体"*/
#define E_COMPC_RESCUE        (103)    /*"复合循环中不能调用复合循环"*/
#define E_COMPC_END            (104)    /*"复合循环未结束"*/
#define E_NO_IIPBUF            (105)    /*"无足够插补缓冲区"*/
#define E_NO_POBUF            (106)    /*"无足够后置缓冲区"*/
#define E_OPT_FUNC            (107)    /*"使用了未经授权的选件功能"*/
#define E_IPO_AXES            (108)    /*"联动轴数超过最大值"*/
#define E_FC_ARGS            (109)    /*"固定循环参数错"*/
#define E_MOVE                (110)    /*"平移指令错"*/
#endif
