






/***********************************<INCLUDES>**********************************/
#include "../../PeriDrivers/W5500/w5500_utility.h"
#include "../../DataType/DataType.h"

/**
*@brief	 	16位字符高8位低8位转换
*@param		i:要转换的数据
*@return	转换后的数据
*/
uBit16 swaps(uBit16 i)
{
  uBit16 ret=0;

  ret = (i & 0xFF) << 8;
  ret |= ((i >> 8)& 0xFF);

  return ret;
}

/**
*@brief	 	32字符高低位变换
*@param		i:要转换的数据
*@return	转换后的数据
*/
uBit32 swapl(uBit32 l)
{
  uBit32 ret=0;

  ret = (l & 0xFF) << 24;
  ret |= ((l >> 8) & 0xFF) << 16;
  ret |= ((l >> 16) & 0xFF) << 8;
  ret |= ((l >> 24) & 0xFF);

  return ret;
}

/**
*@brief		将一个主机模式的unsigned long型数据转换到大端模式的TCP/IP网络字节格式的数据
*@param		要转换的数据
*@return 	大端模式的数据
*/
unsigned long htonl(
	unsigned long hostlong		/**< hostshort  - A 32-bit number in host byte order.  */
	)
{
#if ( SYSTEM_ENDIAN == _ENDIAN_LITTLE_ )
	return swapl(hostlong);
#else
	return hostlong;
#endif
}

/**
*@brief		将一个主机模式的unsigned short型数据转换到大端模式的TCP/IP网络字节格式的数据
*@param		要转换的数据
*@return 	大端模式的数据
*/
uBit16 htons(
	uBit16 hostshort	/**< A 16-bit number in host byte order.  */
	)
{
#if ( SYSTEM_ENDIAN == _ENDIAN_LITTLE_ )
	return swaps(hostshort);
#else
	return hostshort;
#endif
}

/**
*@brief		将一个大端模式的TCP/IP网络字节格式的数据转换到主机模式的unsigned long型数据
*@param		要转换的数据
*@return 	unsigned long模式的数据
*/
unsigned long ntohl(unsigned long netlong)
{
#if ( SYSTEM_ENDIAN == _ENDIAN_LITTLE_ )
	return htonl(netlong);
#else
	return netlong;
#endif
}
