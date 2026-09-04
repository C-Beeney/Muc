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

#include <uassert.h>
#include <ssys.h>
#include <ustring.h>

void
__assert_fail(
    const u8 *expr,
    const u8 *file,
    const s32 line,
    const u8 *func
) {

    u8 line_buf[64];
    enum status status[1];

    write(STDERR, (const u8*) "Assertion failed: ", 19, status);
    write(STDERR, (const u8*) expr, strlen((const u8*)expr), status);
    write(STDERR, (const u8*) "\n", 1, status);

    write(STDERR, (const u8*) file, strlen((const u8*)file), status);
    write(STDERR, (const u8*) " -> ", 4, status);
    write(STDERR, (const u8*) func, strlen((const u8*)func), status);
    write(STDERR, (const u8*) " at line ", 10, status);
    itoa((smax) line, line_buf);
    write(STDERR, (const u8*) line_buf, strlen(line_buf), status);
    write(STDERR, (const u8*) "\n", 1, status);

    abort();
}
