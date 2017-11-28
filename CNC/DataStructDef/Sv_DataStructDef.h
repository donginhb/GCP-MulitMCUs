
/*
Copyright (c) 2013, 东莞华科精机有限公司 All rights reserved.

文件名称：Sv_DataStructDef.h 

摘    要：定义伺服数据类型

运行环境：Windows Xp / WINCE / LPC43xx / LPC1751

修改描述：

当前版本：2.0

修改作者：Alan

修改日期：2016-3-7

//-----------------------------------------------------------------------------------------------
*/

#ifndef    SV_DATA_STRUCT_DEF_H
#define SV_DATA_STRUCT_DEF_H

#include "DataType.h"

#pragma pack(push)
#pragma pack(8)        

//------------------------------------------------------------------------------
//CAN总线伺服参数定义
typedef struct can_sv_parm{
    uBit16 Rn_Pos_Prop_Gain;              //par000 位置比例增益                                          {0 - 30000}Def - 2000'
    uBit16 Rn_Pos_Ff_Gain;                //par001 位置前馈增益                                           {0 - 30000}}Def - 0'
    uBit16 Rn_Pos_Ff_Filter_Pre;          //par002 位置前馈滤波常数                                       {0 - 100}Def - 80' 
    uBit16 Rn_Pos_Cmd_Filter_Per;         //par003 位置指令滤波时间常数                                   {0 - 100}Def - 80 - NN'
    uBit16 Rn_Pos_Reach_Range;            //par004 定位完成范围                                           {0 - 30000}Def - 10'
    uBit16 Rn_Pos_Toler_Range;            //par005 位置超差检测范围                                       {0 - 30000}Def - 5000'
    uBit16 Re_Pos_Dynamic_Gear_En;        //par006 动态电子齿轮有效                                       {0 - 1}Def - 0' 0 - no， 1 - yes 
    uBit16 Re_Pos_Cmd_Num;                //par007 位置指令分频分子                                       {1 - 32767}Def - 1'
    uBit16 Re_Pos_Cmd_Den;                //par008 位置指令分频分母                                       {1 - 32767}Def - 1'
    uBit16 Re_Pos_Cmd_Sign;               //par009 位置指令脉冲取反                                       {0 - 1}Def - 0' 0 - no， 1 - yes      
    uBit16 Re_Pos_Fb_Num;                 //par010 位置反馈分频分子                                        {1 - 32767}Def - 1'
    uBit16 Re_Pos_Fb_Den;                 //par011 位置反馈分频分母                                        {1 - 32767}Def - 1'
    uBit16 Re_Pos_Err_En;                 //par012 位置超差错误有效                                       {0 - 1}Def - 1'  
    uBit16 Reserve0;                      //par013                                                      {0 - 32767}Def - 0 - NN'     
    uBit16 Reserve1;                      //par014                                                       {0 - 32767}Def - 0 - NN' 

    uBit16 Rn_Speed_Prop_Gain1;           //par100  第一速度比例增益                                      {0 - 30000}Def - 2000'
    uBit16 Rn_Speed_Inte_Gain1;           //par101  第一速度积分增益                                      {0 - 1000}Def - 10' 
    uBit16 Rn_Speed_Prop_Gain2;           //par102 第二速度比例增益                                      {0 - 30000}Def - 4000'
    uBit16 Rn_Speed_Inte_Gain2;           //ppar103  第二速度积分增益                                      {0 - 1000}Def - 15' 
    uBit16 Rn_Speed_PG_1_2_Change;        //par104 第一第二速度比例切换点                                  {0 - 100}Def - 10'
    uBit16 Rn_Speed_IG_1_2_Change;        //par105 第一第二速度积分切换点                                   0 - 100}Def - 10'
    uBit16 Rn_Speed_Kc_Gain;              //par106 速度抗饱和补偿因子                                      {0 - 10000}Def - 1000 - NN'
    uBit16 Rn_Speed_Kfr_Pdff;             //par107 PDFF前馈因子                                           {0 - 32767}Def - 0 - NN'          
    uBit16 Rn_Speed_Ff_Gain;              //par108 加速度前馈增益                                         {0 - 32767} Def - 2000'
    uBit16 Rn_Speed_Kd_Gain;              //par109 速度微分增益                                           {0 - 5000} Def - 20'
    uBit16 Rn_Speed_Fb_Filt_Per;          //par110 速度反馈滤波因子                                       {0 - 100}Def - 100'
    uBit16 Rn_Speed_Max_Limit;            //par111 用户设定最大速度限制                                   {0 - 10000} Def - 5000'      
    uBit16 Rn_Speed_Jog_Set;              //par112 Jog速度                                                {0 - 3000} Def - 100'
    uBit16 Rn_Speed_Reach_Range;          //par113 到达速度                                               {0 - 5000}Def - 100'
    uBit16 Rn_Speed_Inter_Cmd;            //par114 内部速度                                              {0 - 3000}Def - 500 - NN'
    uBit16 Rn_Speed_An_Cmd_Filter_Per;    //par115 模拟量速度指令滤波时间常数                             {0 - 100}Def - 100 - NN'  
    uBit16 Rn_Speed_Zero_Comp;            //par116  速度指令零漂补偿                                       {-8000 - +8000}Def - 0 - NN'
    uBit16 Re_Speed_Ad10_Gain;            //par117  速度指令输入增益, 10V电压对应的转速值 r/min            {0 - 3000}Def - 3000 - NN'  
    uBit16 Re_Speed_An_Cmd_Sign;          //par118 模拟量速度指令取反                                     {0 - 1}Def - 0'              0 - no， 1 - yes                          
    uBit16 Rn_Acc_Time_Const;             //par119  加速时间常数                                           {0 - 1000}Def - 10'      
    uBit16 Rn_Dec_Time_Const;             //par120  减速时间常速                                           {0 - 1000}Def - 10'
    uBit16 Rn_Curr_OLC_Limit_Per;         //par200   过载电流百分常数                                       {0 - 100} Def - 100'   
    uBit16 Rn_Curr_Max_Limit_Per;         //par201  最大电流限制百分常数                                  {0 - 100}Def - 100'  
    uBit16 Rn_Curr_Twice_Over_Time;       //par202 均值过载滤波时间                                         {0 - 100}Def - 50'
    uBit16 Rn_Curr_Peak_Over_Time;        //par203 峰值过载滤波时间                                          {0 - 100}Def - 100'
    uBit16 Rn_Torq_Cmd_Filter_Pre;        //par204 力矩指令滤波常数                                       {0 - 100} Def - 50'
    uBit16 Reserve2;                      ///par205   
    uBit16 Ro_Curr_Prop_Gain;             //par206 电流控制增益                                           {0 - 30000}Def - 2000'
    uBit16 Ro_Curr_Inte_Gain;             //par207 电流控制积分                                           {0 - 2000}Def - 20'
    uBit16 Ro_Curr_Kcp_Const;             //par208 电流环解耦补偿系数                                     {0 - 100} Def - 100 - NN'   
    uBit16 Ro_Volt_Kv_Const;              //par209 反电动势时间常数                                       {0 - 100} Def - 100 - NN' 
    uBit16 Reserve3;                      //par210                                                         {0 - 32767}Def - 0 - NN' 

    uBit16 Re_Sv_Ctrl_Mode_Change_En;     //par300 控制方式切换允许                                       {0 - 1} Def - 0'  0 - no， 1 - yes 
    uBit16 Re_Sv_Ctrl_Mode;               //par301 控制模式                                               {0 - 10} Def - 0'
    uBit16 Re_Speed_Cmd_Mode;             //par302 速度指令选择                                           {0 - 4} Def - 0'   0-内部速度，1-外部模拟量速度
    uBit16 Re_Pos_Ctrl_Mode;              //par303 位置指令输入方式                                       {0 - 1}Def - 0' 0 - no， 1 - yes
    uBit16 Rn_Ccw_Limit_En;               //par304 驱动禁止输入有效                                       {0 - 1}Def - 0' 0 - no， 1 - yes
    uBit16 Rn_Cw_Limit_En;                //par305 驱动禁止输入有效                                       {0 - 1}Def - 0' 0 - no， 1 - yes
    uBit16 Rn_Inertia_Ratio;              //par306 惯量比                                                    {0 - 1000}Def - 5'

    uBit16 Re_Io_In8_Force_On;            //par400 8位输入端子强制ON控制字                                   {0 - 255}Def - 0'
    uBit16 Re_Io_In8_Sign;                //par401 8位输入端子取反控制字                                     {0 - 255}Def - 0'
    uBit16 Re_Io_Out8_Sign;               //par402 8位输出端子取反控制字                                     {0 - 255}Def - 0'
    uBit16 Re_Io_Filter_Time;             //par403 IO端子去抖时间常数                                     {0 - 30000}Def - 100 - NN' 
    uBit16 Re_InPut0_Plan;                //par404 输入端子0规划                                     {0 - 10}Def - 0 - NN' 
    uBit16 Re_InPut1_Plan;                //par405 输入端子1规划                                    {0 - 10}Def - 1 - NN' 
    uBit16 Re_InPut2_Plan;                //par406 输入端子2规划                                   {0 - 10}Def - 2 - NN' 
    uBit16 Re_InPut3_Plan;                //par407 输入端子3规划                                   {0 - 10}Def - 3 - NN' 
    uBit16 Re_InPut4_Plan;                //par408 输入端子4规划                                   {0 - 10}Def - 4 - NN' 
    uBit16 Re_InPut5_Plan;                //par409 输入端子5规划                                    {0 - 10}Def - 5 - NN' 
    uBit16 Re_OutPut0_Plan;               //par410 输出端子0规划                                    {0 - 10}Def - 0 - NN' 
    uBit16 Re_OutPut1_Plan;               //par411 输出端子1规划                                   {0 - 10}Def - 1 - NN' 
    uBit16 Re_OutPut2_Plan;               //par412 输出端子2规划                                   {0 - 10}Def - 2 - NN' 
    uBit16 Re_OutPut3_Plan;               //par413 输出端子3规划                                  {0 - 10}Def - 3 - NN' 
    uBit16 Rn_Force_Enable;               //par414 强制使能                                               {0 - 1}Def - 0' 0 - no， 1 - yes
    uBit16 Re_Enable_Delay_Time;          //par415 使能延时时间                                               {0 - 30000}Def - 200
    uBit16 Rn_Break_Enable;               //par416 抱闸使能                                               {0 - 1}Def - 0
    uBit16 Rn_Break_Enable_Delay;         //par417 抱闸使能延时                                               {0 - 30000}Def - 200
    uBit16 Rn_Break_Disable_Delay;        //par418 抱闸断使能延时                                               {0 - 30000}Def - 200

    uBit16 Rn_Vis_Fri_Coeff;              //par500 粘滞摩擦系数                                               {0 - 10000}Def - 0 - NN'     
    uBit16 Rn_Shock_Res_Rate;             //par501 停止时振动抑制衰减比                                   {0 - 100}Def - 0 - NN'     
    uBit16 Rn_Shock_Res_Time;             //par502 停止时振动抑制开始时间                                 {0 - 100}Def - 0 - NN' 
    uBit16 Rn_P_Pi_Change_Point;          //par503 P/PI切换点                                             {0 - 100}Def - 30 - NN'    
    uBit16 Re_Uvw_Delay_Time;             //par504 省线式编码器UVW调整时间                                {0 - 30000}Def - 20000 - NN' 
    uBit16 Re_Led_Init_Status;            //par505 LED初始显示状态                                        {0 - 40}Def -10 - NN'
    uBit16 Re_Para_Auto_Adjust_En;        //par506 PID参数自调整使能                                        {0 - 1}Def -0 - NN'
    uBit16 Re_Para_Auto_Adjust_Mode;      //par507 PID参数自调整模式                                        {0 - 6}Def -0 - NN'
    uBit16 Re_Restore_Parameter;          //par508 恢复出厂参数设置                                         {0 - 19999}Def - 0'

    uBit16 nSpeedSineFreq;                //par600 Test01                                 {0 - 32767}Def - 10'      
    uBit16 nSpeedSquareFreq;              //par601 Test02                                 {0 - 32767}Def - 10'     
    uBit16 nSpeedAmpPre;                  //par602 速度幅度百分比                                 {0 - 100}Def - 10' 
    uBit16 Test04;                        //par603 Test04                                  {0 - 32767}Def - 0 - NN' 
    uBit16 Test05;                        //par604 Test05                                {0 - 32767}Def - 0 - NN' 
    uBit16 Test06;                        //par605 Test06                                 {0 - 32767}Def - 0 - NN' 
    uBit16 Test07;                        //par606 Test07                                  {0 - 32767}Def - 0 - NN' 

    uBit16 Rnn_Password1;                 //par700 参数修改密码1                       {0 - 32767}Def - 0 - NN' 
    uBit16 Rnn_Password2;                 //par701 参数修改密码2                         {0 - 32767}Def - 0 - NN' 
    uBit16 Ro_HardWareVersion;            //par702 硬件版本                         {0 - 32767}Def - 0 - NN' 
    uBit16 Ro_SoftWareVersion;            //par703 软件版本                         {0 - 32767}Def - 0 - NN' 

    uBit16 Para_Crcdata;                  // 校验位         
}CAN_SV_PARM;


//MIII总线伺服参数定义
typedef struct miii_sv_parm
{
    uBit32 ulEncoderType;    //编码器类型 0-绝对编码器 1-增量编码器(范围0-1)，只读
    uBit32 ulMotorType;        //电机类型    ，0-旋转电机 1-线性电机(范围0-1)，只读
    uBit32 ulCloseLoop;        //电机类型    ，0-半闭环 1-全闭环(范围0-1)，只读
    uBit32 ulRatedSpeed;    //额定转速    ，单位:rev/min 或mm/s 只读
    uBit32 ulMaxSpeed;        //最大转速，单位:rev/min 或mm/s 只读
    Bit32 lSpeedMultiplier;    //速度幂N(额定转速对最大转速的10的N次幂) 只读
    uBit32 ulRatedTorque;    //额定转矩    ，单位:N或N.M 只读
    uBit32 ulMaxTorque;        //最大转矩，单位:N或N.M 只读
    Bit32 lTorqueMultiplier;    //转矩幂N(额定转矩对最大转矩的10的N次幂) 只读
    uBit32 ulResolution;        //分辨率(旋转)，单位pulse/rev; 读写
    uBit32 ulLinearScalePitch;        //直线标尺节距，单位nm, 读写
    uBit32 ulPulsePerScalePitch;    //每标尺节距对应的脉冲数，单位pulse/pitch, 读写
    uBit32 ulNumratorGear;            //电子齿轮比分母，读写
    uBit32 ulDenominatorGear;        //电子齿轮比分子，读写
    Bit32 lAEncoderOriOffset;        //绝对式编码器原点偏置，单位：指令单位，读写
    uBit32 ulMultiturnLimit;            //多旋转圈数上下值设定，单位：rev，读写
    uBit32 ulLimitSetting;            //限制使能设定，读写
                                            //bit0-正硬限位(0-屏蔽 1-不屏蔽)
                                            //bit1-负硬限位(0-屏蔽 1-不屏蔽)
                                            //bit4-正软限位(0-无效 1-有效)
                                            //bit5-负硬限位(0-无效 1-有效)
    Bit32 lPSoftLimit;                    //正侧软限位值，单位：指令单位，读写
    Bit32 lNSoftLimit;                    //负侧软限位值，单位：指令单位，读写

    //单位系参数------------------------
    uBit32 ulSpeedUnit;                //速度单位选择,范围（0-4）
                                            //0--指令单位/s
                                            //1--指令单位/min
                                            //2--相应额定速度的%
                                            //3--rad/min或mm/s
                                            //4--电机最高速度/0x40000000
    Bit32 lSpeedBaseUnit;            //速度基本单位选择，设定为速度单位的10的n次幂，读写

    uBit32 ulPosUnit;                    //位置单位选择：固定为0，表示指令单位，读写
    Bit32 lPosBaseUnit;                //位置基本单位选择，设定为位置单位的10的n次幂，读写

    uBit32 ulAccUnit;                    //加速度单位选择，范围(0-1)    ，读写            
                                            //0--指令单位/s-2;
                                            //1--ms(加速时间)，到达额定转速时间
    Bit32 lAccBaseUnit;                //加速度基本单位选择，设定为加速度单位的10的n次幂，读写

    uBit32 ulTorqueUnit;                //转矩单位选择，范围(0-2)    ，读写            
                                            //0--N.M;
                                            //1--相应额定转速的%
                                            //2--最大转矩/0x40000000
    Bit32 lTorqueBaseUnit;            //转矩基本单位选择，设定为转矩单位的10的n次幂，读写
    
    uBit32 ulSupportedUnit;            //支持单位系设置，只读
                                            //bit0--bit7;速度单位系支持设置
                                            //bit8--bit15,位置单位系支持设置
                                            //bit16--bit23,加速度单位系支持设置
                                            //bit24--bit31,转矩单位系支持设置

    //调整参数----------------------------
    uBit32 ulSpeedLoopGain;            //速度环增益，单位0.001HZ，读写
    uBit32 ulSpeedITimeConstant;    //速度环积分时间常数，单位us，读写
    uBit32 ulPosLoopGain;                //位置环增益，单位0.001/s，读写
    uBit32 ulFeedForwardComp;        //前馈补偿，单位%，读写
    uBit32 ulPosITimeConstant;        //位置环积分时间常数，单位us，读写
    uBit32 ulInPosRang;                    //定位误差，单位：指令单位，读写
    uBit32 ulNearPosRang;                //定位接近误差，单位：指令单位，读写
    
    //命令相关参数--------------------------
    uBit32 ulExFunAcceDeceTConstant;//指数函数加减速时间常数，单位us，读写
    uBit32 ulMoveAverageTime;            //移动平均时间，单位us，读写
    Bit32 lExPosingFinalPos;                    //外部信号最终移动距离，单位：指令单位，读写
    uBit32 ulHomeApproachSpeed;        //回零接近速度，单位：0.001/min或0.001mm/s，读写
    uBit32 ulHomeCreepSpeed;                //回零蠕变速度，单位：0.001/min或0.001mm/s，读写
    Bit32 lHomeFinalTravel;                    //原点复归最终移动距离，单位：指令单位，读写
    uBit32 ulMonitorSelect1;                //固定监视1，范围0-F，读写
    uBit32 ulMonitorSelect2;                //固定监视2，范围0-F，读写
    uBit32 ulSelMon1;                            //选择监视1，范围0-9，读写
    uBit32 ulSelMon2;                            //选择监视2，范围0-9，读写
    uBit32 ulSelMon3;                            //选择监视3，范围0-9，读写
    uBit32 ulZPointDetctionRang;            //原点检出幅度，单位：指令单位，读写
    uBit32 ulPTorqueLimit;                    //正侧转矩限制，单位：参数设定，读写
    uBit32 ulNTorqueLimit;                    //反侧转矩限制，单位：参数设定，读写
    uBit32 ulZSpeedDetectionRang;        //零速信号检出范围，单位：0.001/min或0.001mm/s，读写
    uBit32 ulSpeedMatchDetectionRang;//速度一致信号检出范围，单位：0.001/min或0.001mm/s，读写
    uBit32 ulSvCtrlEnableSetting;            //伺服命令控制字段有效/无效设置，只读
    uBit32 ulSvStatEnableSetting;        //伺服命令伺服状态字有效/无效设置，只读
    uBit32 ulOutEnableSetting;                //伺服命令IO控制字段(输出)有效/无效设置，只读
    uBit32 ulInEnableSetting;                //伺服命令IO控制字段(输入)有效/无效设置，只读
}MIII_SV_PARM;

#pragma pack(pop)

#endif // #ifndef AXIS_DATA_STRUCT_DEF_H

