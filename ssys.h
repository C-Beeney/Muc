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
    #include <stypes.h>

    #define STDIN       ((const u32)0)
    #define STDOUT      ((const u32)1)
    #define STDERR      ((const u32)2)

    extern u32  read  (const u32 fh  , u8* buffer, const smax count);
    extern u32  write (const u32 fh  , const u8 *buffer, const smax count);
    extern u32  open  (const u8 *path, const u32 mode);
    extern void close (const u32 fh);
    extern void exit  (const s8 ec);
    extern void abort (void);
#endif/*SSYS_H*/
