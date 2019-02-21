#ifndef ATCAFX_H
#define ATCAFX_H

#include <iostream>
#include <list>
#include <string>
#include <numeric>
#include <vector>


//
// Unix/GCC/Clang
//
typedef signed char            Int8;
typedef unsigned char          UInt8;
typedef signed short           Int16;
typedef unsigned short         UInt16;
typedef signed int             Int32;
typedef unsigned int           UInt32;
typedef signed long            IntPtr;
typedef unsigned long          UIntPtr;
typedef float                  fp32;
#if defined(__LP64__)
    #define POCO_PTR_IS_64_BIT 1
    #define POCO_LONG_IS_64_BIT 1
    typedef signed long        Int64;
    typedef unsigned long      UInt64;
#else
    typedef signed long long   Int64;
    typedef unsigned long long UInt64;
#endif



#endif // ATCAFX_H
