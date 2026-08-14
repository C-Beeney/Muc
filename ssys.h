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

        
        enum status {
            #ifndef SCOMPAT_IMPL
            OK = 0,

            GENERIC_ERR,
            EOF,
            STATUS_COUNT
            #else
            GX_OK = 0,
            GX_GENERIC_ERR,
            GX_EOF,
            GX_STATUS_COUNT
            #endif/*SCOMPAT_H*/
        };


        #ifndef SSYS_H_NODECL
                #include <stypes.h>

                #define STDIN       ((const u32)0)
                #define STDOUT      ((const u32)1)
                #define STDERR      ((const u32)2)

                extern void
                read(
                        const u16 fh,
                        u8* buffer,
                        const umax count,
                        enum status status[1]
                );

                extern void
                write(
                        const u16 fh,
                        const u8 *buffer,
                        const umax count,
                        enum status status[1]
                );

                extern u16
                open(
                        const u8 *path,
                        enum status status[1]
                );

                extern u16
                open_readonly(
                        const u8 *path,
                        enum status status[1]
                );

                extern u16
                create(
                        const u8 *path,
                        enum status status[1]
                );

                extern void
                create_passive(
                        const u8 *path,
                        enum status status[1]
                );

                extern void
                close(
                        const u16 fh,
                        enum status status[1]
                );

                extern void
                exit(
                    const s8 ec
                );

                extern void
                abort(void);
        #else
                extern void
                gx_read(
                        const u16 fh,
                        u8* buffer,
                        const umax count,
                        enum status status[1]
                );

                extern void
                gx_write(
                        const u16 fh,
                        const u8 *buffer,
                        const umax count,
                        enum status status[1]
                );

                extern u16
                gx_open(
                        const u8 *path,
                        enum status status[1]
                );

                extern u16
                gx_open_readonly(
                        const u8 *path,
                        enum status status[1]
                );

                extern u16
                gx_create(
                        const u8 *path,
                        enum status status[1]
                );

                extern void
                gx_create_passive(
                        const u8 *path,
                        enum status status[1]
                );

                extern void
                gx_close(
                        const u16 fh,
                        enum status status[1]
                );

                extern void
                gx_exit(
                    const s8 ec
                );

                extern u8*
                gx_itoa(
                    smax i,
                    u8 *buffer
                );

                int gx__assert_fail(
                    const u8 *expr,
                    const u8 *file,
                    const s32 line,
                    const u8 *func
                );

                smax
                gx_strlen(
                        const u8 *buffer
                );

                u8
                gx_streq(
                        const u8 *buffer0,
                        const u8 *buffer1
                );

                extern void
                gx_abort(void);
        #endif /*SSYS_H_NODECL*/
#endif/*SSYS_H*/
