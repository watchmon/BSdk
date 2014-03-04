#ifndef _COMMON_BASE_TYPE_H
#define _COMMON_BASE_TYPE_H

#include <stdint.h>
#include "common.h"

__COMMON_BEGIN_NAMESPACE

#ifndef TRUE
#define TRUE    (1)
#endif

#ifndef FALSE
#define FALSE   (0)
#endif

typedef bool     Bool;
typedef int8_t   Int8;
typedef uint8_t  Uint8;
typedef int16_t  Int16;
typedef uint16_t Uint16;
typedef int32_t  Int32;
typedef uint32_t Uint32;
typedef int64_t  Int64;
typedef uint64_t Uint64;

#define MAKEWORD(a, b)  ((Uint16)(((Uint8)(a)) | ((Uint16)((Uint8)(b))) << 8))

__COMMON_END_NAMESPACE

#endif
