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

        /* Basic Architecture Detection */

        #if     defined(__LP64__)   || \
                defined(_LP64)      || \
                defined(__x86_64__) || \
                defined(_M_X64)     || \
                defined(__aarch64__)

                #define ARCH_64

        #elif   defined(__i386__)    || \
                defined(__i386)      || \
                defined(_M_IX86)     || \
                defined(__arm__)     || \
		defined(__powerpc__) || \
                (defined(_WIN32) && !defined(_WIN64))

                #define ARCH_32

        #elif   defined(__I86__) || \
                defined(_M_I86)  || \
                defined(__MSDOS__)

                #define ARCH_16

        #endif /* ARCH_CHECK */

        /* Ensure only one architecture is defined */
        #if defined(ARCH_16) && (defined(ARCH_32) || defined(ARCH_64))
                #define ARCH_MULTI
        #elif defined(ARCH_32) && defined(ARCH_64)
                #define ARCH_MULTI
        #endif

        #if defined(ARCH_MULTI)
                typedef char multiple_architectures_detected[-1];
        #endif

        /* Ensure at least one architecture is defined */
        #if !defined(ARCH_16) && !defined(ARCH_32) && !defined(ARCH_64)
                typedef char no_architecture_detected[-1];
        #endif

        /* All applicable platforms support 8-bit types */
        typedef unsigned char u8;
        typedef signed char   s8;

        #define U8_MAX  ((u8) 0xFF)
        #define U8_MIN  ((u8) 0x00)
        #define S8_MAX  ((s8) 0x7F)
        #define S8_MIN  ((s8)-0x80)

        #ifdef ARCH_16
                
                typedef signed   int   s16;
                typedef unsigned int   u16;

                #define U16_MAX ((u16) 0xFFFF   )
                #define U16_MIN ((u16) 0x0000   )
                #define S16_MAX ((s16) 0x7FFF   )
                #define S16_MIN ((s16)-S16_MAX-1)

                typedef s16 snat;
                typedef u16 unat;

                #define SNAT_MIN ((snat) S16_MIN)
                #define SNAT_MAX ((snat) S16_MAX)
                #define UNAT_MIN ((unat) U16_MIN)
                #define UNAT_MAX ((unat) U16_MAX)

                typedef signed   long s32;
                typedef unsigned long u32;

                #define U32_MAX ((u32) 0xFFFFFFFF)
                #define U32_MIN ((u32) 0x00000000)
                #define S32_MAX ((s32) 0x7FFFFFFF)
                #define S32_MIN ((s32) -S32_MAX-1)

                typedef s32 smax;
                typedef u32 umax;

                #define SMAX_MAX S32_MAX
                #define SMAX_MIN S32_MIN
                #define UMAX_MAX U32_MAX
                #define UMAX_MIN U32_MIN
                
        #endif /* ARCH_16 */

        #if defined(ARCH_32) || defined(ARCH_64)
                typedef unsigned short u16;
                typedef signed short   s16;
                typedef unsigned int   u32;
                typedef signed int     s32;

                #define U16_MAX ((u16) 0xFFFF    )
                #define U16_MIN ((u16) 0x0000    )
                #define S16_MAX ((s16) 0x7FFF    )
                #define S16_MIN ((s16) -S16_MAX-1)

                #define U32_MAX ((u32) 0xFFFFFFFF)
                #define U32_MIN ((u32) 0x00000000)
                #define S32_MAX ((s32) 0x7FFFFFFF)
                #define S32_MIN ((s32) -S32_MAX-1)
        #endif /* ARCH_32/64 */

        #ifdef ARCH_32
                typedef s32 smax;
                typedef u32 umax;
                typedef s32 snat;
                typedef u32 unat;

                #define SNAT_MIN ((snat) S32_MIN)
                #define SNAT_MAX ((snat) S32_MAX)
                #define UNAT_MIN ((unat) U32_MIN)
                #define UNAT_MAX ((unat) U32_MAX)

                #define SMAX_MAX S32_MAX
                #define SMAX_MIN S32_MIN
                #define UMAX_MAX U32_MAX
                #define UMAX_MIN U32_MIN
        #endif /* ARCH_32 */

        #ifdef ARCH_64
                /* Use compiler-specific extensions for 64-bit in C89 */
                #if defined(_MSC_VER) /* ARCH_64_ACTIVE */
                        #define ARCH_64_ACTIVE

                        typedef unsigned __int64 u64;
                        typedef signed __int64   s64;
                        #define U64_MAX ((u64) 0xFFFFFFFFFFFFFFFFULL)
                        #define U64_MIN ((u64) 0x0000000000000000ULL)
                        #define S64_MAX ((s64) 0x7FFFFFFFFFFFFFFFLL )
                        #define S64_MIN ((s64) -S64_MAX-1           )
                #elif !defined(NO_COMPILER_EXTENSIONS) /* ARCH_64_ACTIVE */
                        #define ARCH_64_ACTIVE

                        __extension__ typedef unsigned long long u64;
                        __extension__ typedef signed long long   s64;
                        #define U64_MAX ((u64) __extension__ \
                                0xFFFFFFFFFFFFFFFFULL)
                        #define U64_MIN ((u64) __extension__ \
                                0x0000000000000000ULL)
                        #define S64_MAX ((s64) __extension__ \
                                0x7FFFFFFFFFFFFFFFLL )
                        #define S64_MIN ((s64) __extension__ \
                                -S64_MAX-1           )
                #else /* ARCH_64_ACTIVE */
                        #undef ARCH_64_ACTIVE
                        #define u64 no_u64_implementation
                        #define s64 no_s64_implementation
                #endif /* ARCH_64_ACTIVE */

                #if !defined(NO_COMPILER_EXTENSIONS)
                        typedef s64 smax;
                        typedef u64 umax;

                        #define SMAX_MAX (__extension__ S64_MAX)
                        #define SMAX_MIN (__extension__ S64_MIN)
                        #define UMAX_MAX (__extension__ U64_MAX)
                        #define UMAX_MIN (__extension__ U64_MIN)

                #else /* NO_COMPILER_EXTENSIONS */
                        typedef s32 smax;
                        typedef u32 umax;

                        #define SMAX_MAX S32_MAX
                        #define SMAX_MIN S32_MIN

                        #define UMAX_MAX U32_MAX
                        #define UMAX_MIN U32_MIN
                #endif /* NO_COMPILER_EXTENSIONS */


        #endif /* ARCH_64 */

        #ifndef NO_STATIC_TYPE_ASSERTIONS
                typedef char u8_is_size_1 [(sizeof(u8) ==1)?1:-1];
                typedef char s8_is_size_1 [(sizeof(s8) ==1)?1:-1];

                #if defined(ARCH_16) || \
                    defined(ARCH_32)

                        typedef char u16_is_size_2[(sizeof(u16)==2)?1:-1];
                        typedef char s16_is_size_2[(sizeof(s16)==2)?1:-1];
                        typedef char u32_is_size_4[(sizeof(u32)==4)?1:-1];
                        typedef char s32_is_size_4[(sizeof(s32)==4)?1:-1];

                #endif

                #if defined(U64_MAX) && \
                    defined(U64_MIN) && \
                    defined(S64_MAX) && \
                    defined(S64_MIN)

                        typedef char u64_is_size_8[(sizeof(u64)==8)?1:-1];
                        typedef char s64_is_size_8[(sizeof(s64)==8)?1:-1];

                #endif/* 64-bit types are defined */

                #if !defined(U8_MAX) || !defined(U8_MIN) ||\
                    !defined(S8_MAX) || !defined(S8_MIN)
                        typedef char missing_min_max_defined8[-1];
                #endif

                #if ( defined(ARCH_16) ||  defined(ARCH_32)) && \
                    (!defined(U16_MAX) || !defined(U16_MIN)  || \
                     !defined(S16_MAX) || !defined(S16_MIN)  || \
                     !defined(U32_MAX) || !defined(U32_MIN)  || \
                     !defined(S32_MAX) || !defined(S32_MIN))

                        typedef char missing_min_max_defined16_32[-1];
                #endif

                #if (defined(ARCH_64)  && \
                    (defined(_MSC_VER) || \
                    !defined(NO_COMPILER_EXTENSIONS))) && \
                   (!defined(U64_MAX) || !defined(U64_MIN) || \
                    !defined(S64_MAX) || !defined(S64_MIN))

                        typedef char missing_min_max_defined64[-1];
                #endif

        #endif /* NO_STATIC_TYPE_ASSERTIONS */

        /* Only one PAD* statement may be used per line unless compiler
         * extensions are available and enabled!                             */

        #ifndef NO_PADDING
                #define PADDED(x) PAD16(x) PAD32(x) PAD64(x)

                #define _PAD_CONCAT_INTERNAL(a, b) a##b
                #define _PAD_CONCAT(a, b) _PAD_CONCAT_INTERNAL(a, b)
                
                #if !defined (NO_COMPILER_EXTENSIONS) && defined(__COUNTER__)
                        #define _PAD_CONCAT_NOISE __COUNTER__
                #else
                        #define _PAD_CONCAT_NOISE __LINE__
                #endif     

                /* Padding on any supported architecture width */
                
                #define PAD(_16, _32, _64) PAD16(_16) PAD32(_32) PAD64(_64)

                /* Padding for 16-bit architecture */
                #ifdef ARCH_16
                        #define PAD16(x) \
                                union { u8 _pad[x]; } \
                                _PAD_CONCAT(_pad, _PAD_CONCAT_NOISE);
                #else
                        #define PAD16(x)
                #endif /* ARCH_16 */
                
                /* Padding for 32-bit architecture */
                #ifdef ARCH_32
                        #define PAD32(x) \
                                union { u8 _pad[x]; } \
                                _PAD_CONCAT(_pad, _PAD_CONCAT_NOISE);
                #else
                        #define PAD32(x)
                #endif /* ARCH_32 */

                /* Padding for 64-bit architecture */
                #ifdef ARCH_64
                        #define PAD64(x) \
                                union { u8 _pad[x]; } \
                                _PAD_CONCAT(_pad, _PAD_CONCAT_NOISE);
                #else
                        #define PAD64(x)
                #endif/*ARCH_64*/
        #else  /* NO_PADDING */
                #define PADDED(x)
                #define PAD(_16, _32, _64)
                #define PAD16(x)
                #define PAD32(x)
                #define PAD64(x)
        #endif /* NO_PADDING */

#endif /* PORTABLE_TYPES_H */
