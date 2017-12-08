//定义数据类型 (说明：请根据不同的开发平台修改该数据类型定义)
#ifndef __DATATYPE_H
#define __DATATYPE_H

typedef unsigned char      uBit8;
typedef   signed char      Bit8; 

typedef unsigned short int uBit16;
typedef   signed short int Bit16;  

typedef unsigned long  int uBit32; 
typedef signed   long  int Bit32;  

typedef signed long long   Bit64;

typedef unsigned char BooLean;

typedef float float32;
typedef double double64;

#ifndef NULL
#define NULL 0
#endif

#ifndef __cplusplus

#ifndef bool
#define bool _Bool
#endif

#ifndef false
#define false 0
#endif

#ifndef true
#define true 1
#endif

#endif /* !__cplusplus */


#ifdef __CC_ARM
#define INLINE  __inline
#else
#define INLINE inline
#endif



#define        ZERO    (1e-6)

#ifndef PI
    #define PI    (3.14159265358979f)    //圆周率
#endif

#define equal(a, b)  (fabs((a)-(b)) < ZERO)     //浮点数是否相等判断“==”宏定义

#endif
