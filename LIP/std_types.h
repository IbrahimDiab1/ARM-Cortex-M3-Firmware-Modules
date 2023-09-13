/************************************************/
/* Author      : Ibrahim Diab                   */
/* File Name   : Std_Types.h                    */
/* Description : Common Standard types file     */
/************************************************/

#ifndef STD_TYPES_H
#define STD_TYPES_H

typedef unsigned char boolean;

#ifndef FALSE
#define FALSE       (0u)
#endif

#ifndef TRUE
#define TRUE        (1u)
#endif

#ifndef DISABLED
#define DISABLED    (0u)
#endif

#ifndef ENABLED
#define ENABLED     (1u)
#endif

#ifndef NULL
#define NULL        ((void*)0)
#endif

typedef unsigned char       uint8   ;       /*                    0 .. 255                   */
typedef signed char         int8    ;       /*                 -128 .. +127                  */
typedef unsigned short      uint16  ;       /*                    0 .. 65535                 */
typedef signed short        int16   ;       /*               -32768 .. +32767                */
typedef unsigned long       uint32  ;       /*                    0 .. 4294967295            */
typedef signed long         int32   ;       /*          -2147483648 .. +2147483647           */
typedef unsigned long long  uint64  ;       /*                    0 .. 18446744073709551615  */
typedef signed long long    int64   ;       /* -9223372036854775808 .. 9223372036854775807   */
typedef float               float32 ;
typedef double              float64 ;   

#endif  // STD_TYPES_H
