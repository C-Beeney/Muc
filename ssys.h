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

#ifndef SSYS_H
#define SSYS_H

        #define SSYS_CAT_(a, b) a##b
        #define SSYS_CAT(a, b)  SSYS_CAT_(a, b)

        #ifndef SSYS_PREFIX
                #define SSYS_PREFIXED(s) s
        #else/*SSYS_PREFIX*/
                #define SSYS_PREFIXED(s) SSYS_CAT(SSYS_PREFIX, s)
        #endif/*SSYS_PREFIX*/

        enum status {
            SSYS_PREFIXED(OK                  ) = 0,
            SSYS_PREFIXED(GENERIC_ERR         )    ,
            SSYS_PREFIXED(EOF                 )    ,
            SSYS_PREFIXED(COLLECTION_NOT_FOUND)    ,
            SSYS_PREFIXED(STATUS_COUNT)
        };

        #include <stypes.h>

        #ifndef SSYS_H_NODECL
                #define STDIN       ((const u32)0)
                #define STDOUT      ((const u32)1)
                #define STDERR      ((const u32)2)
        #endif
        
        extern void
        SSYS_PREFIXED(read)(
                const u16 fh,
                u8* buffer,
                const umax count,
                enum status status[1]
        );

        extern void
        SSYS_PREFIXED(write)(
                const u16 fh,
                const u8 *buffer,
                const umax count,
                enum status status[1]
        );

        extern u16
        SSYS_PREFIXED(open)(
                const u8 *path,
                enum status status[1]
        );

        extern u16
        SSYS_PREFIXED(open_readonly)(
                const u8 *path,
                enum status status[1]
        );

        extern u16
        SSYS_PREFIXED(create)(
                const u8 *path,
                enum status status[1]
        );

        extern void
        SSYS_PREFIXED(create_passive)(
                const u8 *path,
                enum status status[1]
        );

        extern void
        SSYS_PREFIXED(close)(
                const u16 fh,
                enum status status[1]
        );

        extern void
        SSYS_PREFIXED(exit)(
                const s8 ec
        );

        extern void
        SSYS_PREFIXED(abort)(void);

        #undef SSYS_CAT_
        #undef SSYS_CAT
        #undef SSYS_PREFIXED

#endif/*SSYS_H*/
