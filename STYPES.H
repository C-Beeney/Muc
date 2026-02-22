/*---------------------------------------------------------------------------|
|                                                                            |
|   Copyright 2026 Connor R Beeney                                           |
|                                                                            |
|   Licensed under the Apache License, Version 2.0 (the "License");          |
|   you may not use this file except in compliance with the License.         |
|   You may obtain a copy of the License at                                  |
|                                                                            |
|       http://www.apache.org/licenses/LICENSE-2.0                           |
|                                                                            |
|   Unless required by applicable law or agreed to in writing, software      |
|   distributed under the License is distributed on an "AS IS" BASIS,        |
|   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. |
|   See the License for the specific language governing permissions and      |
|   limitations under the License.                                           |
|                                                                            |
|---------------------------------------------------------------------------*/

#ifndef PORTABLE_TYPES_H
#define PORTABLE_TYPES_H

    /* --- 1. Basic Architecture Detection --- */

    /* Detect 64-bit: Look for LP64 (Unix/Mac) or LLP64 (Windows x64) indicators */
    #if defined(__LP64__) || defined(_LP64) || defined(__x86_64__) || defined(_M_X64) || defined(__aarch64__)

        #define ARCH_64

    #/* Detect 32-bit: Check common 32-bit CPU macros or Windows 32-bit markers */
    #elif defined(__i386__) || defined(_M_IX86) || defined(__arm__) || (defined(_WIN32) && !defined(_WIN64))

        #define ARCH_32
    #/* Detect 16-bit: Look for 8086/OpenWatcom style markers */
    #elif defined(__I86__) || defined(_M_I86) || defined(__MSDOS__)

        #define ARCH_16
        
    #endif

    /* --- 2. Type Mapping based on Detection --- */

    typedef unsigned char u8;
    typedef signed char   i8;

    #define U8_MAX   0xFF
    #define U8_MIN   0x00
    #define I8_MAX   0x7F
    #define I8_MIN (-0x80)

    #ifdef ARCH_16

        typedef unsigned int   u16;
        typedef signed int     i16;
        typedef unsigned long  u32;
        typedef signed long    i32;
        /* C89 has no standard 64-bit type; many 16-bit compilers don't support it */

        #define U16_MAX   0xFFFF
        #define U16_MIN   0x0000
        #define I16_MAX   0x7FFF
        #define I16_MIN (-0x8000)

    #endif

    #if defined(ARCH_32) || defined(ARCH_64)

        typedef unsigned short u16;
        typedef signed short   i16;
        typedef unsigned int   u32;
        typedef signed int     i32;

        #define U16_MAX   0xFFFF
        #define U16_MIN   0x0000
        #define I16_MAX   0x7FFF
        #define I16_MIN (-0x8000)

        #define U32_MAX   0xFFFFFFFF
        #define U32_MIN   0x00000000
        #define I32_MAX   0x7FFFFFFF
        #define I32_MIN (-0x80000000)

    #endif

    #ifdef ARCH_64
        /* Use compiler-specific extensions for 64-bit in C89 */
        #if defined(_MSC_VER)

            typedef unsigned __int64 u64;
            typedef signed __int64   i64;
            #define U64_MAX   0xFFFFFFFFFFFFFFFFUI64
            #define U64_MIN   0x0000000000000000UI64
            #define I64_MAX   0x7FFFFFFFFFFFFFFFI64
            #define I64_MIN (-(I64_MAX)-1I64)
        #else

            __extension__ typedef unsigned long long u64;
            __extension__ typedef signed long long   i64;
            #define U64_MAX   0xFFFFFFFFFFFFFFFFULL
            #define U64_MIN   0x0000000000000000ULL
            #define I64_MAX   0x7FFFFFFFFFFFFFFFLL
            #define I64_MIN (-(I64_MAX)-1LL)
        
        #endif


        typedef i64 imax;
        typedef u64 umax;

    #endif

    #ifndef NO_STATIC_TYPE_ASSERTIONS

        typedef char u8_is_size_1 [(sizeof(u8) ==1)?1:-1];
        typedef char i8_is_size_1 [(sizeof(i8) ==1)?1:-1];
        typedef char u16_is_size_2[(sizeof(u16)==2)?1:-1];
        typedef char i16_is_size_2[(sizeof(i16)==2)?1:-1];
        typedef char u32_is_size_4[(sizeof(u32)==4)?1:-1];
        typedef char i32_is_size_4[(sizeof(i32)==4)?1:-1];
        typedef char u64_is_size_8[(sizeof(u64)==8)?1:-1];
        typedef char i64_is_size_8[(sizeof(i64)==8)?1:-1];

        #if !defined(U8_MAX ) ||! defined(U8_MIN ) \
        ||! defined(I8_MAX ) ||! defined(I8_MIN ) \
        ||! defined(U16_MAX) ||! defined(U16_MIN) \
        ||! defined(I16_MAX) ||! defined(I16_MIN) \
        ||! defined(U32_MAX) ||! defined(U32_MIN) \
        ||! defined(I32_MAX) ||! defined(I32_MIN) \
        ||! defined(U64_MAX) ||! defined(U64_MIN) \
        ||! defined(I64_MAX) ||! defined(I64_MIN)
            char missing_min_max_defined[-1];
        
        #endif

    #endif

#endif /* PORTABLE_TYPES_H */
