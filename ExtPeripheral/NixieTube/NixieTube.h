#ifndef  __NIXIE_TUBE_H
#define  __NIXIE_TUBE_H

#include "DataType/DataType.h"

#ifdef __cplusplus
extern "C"
{
#endif

/*****************************************************************************
 * 数码管控制接口
 ****************************************************************************/

/**
  * @brief  数码管控制引脚编号设置
  * @param  ulDataPinNO  数据引脚编号(IO资源表中输出IO组的位置)
  * @param  ulClockPinNO 时钟引脚编号(IO资源表中输出IO组的位置)
  * @param  ulStbPinNO   片选引脚编号(IO资源表中输出IO组的位置)
  * @retval None
  */
void NixieTube_SetCtrlPinNO(uBit32 ulDataPinNO, uBit32 ulClockPinNO, uBit32 ulStbPinNO);


/**
  * @brief  数码管数据显示函数
  * @param  uHexData 要显示的数据
  * @retval 0-成功 非0-失败
  */
uBit32 NixieTube_DisNumber(uBit8 uHexData);


#ifdef __cplusplus
}
#endif

#endif /* __NIXIE_TUBE_H */
