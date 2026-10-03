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

#include <ssys.h>

#include <uconio.h>
#include <ustring.h>
#include <uassert.h>

static void
print_buffer(
        u16 handle,
        const u8 *buffer
) {
        enum status status;
        write(handle, buffer, strlen(buffer), &status);
        assert(status == OK);
}

void
print(
        const u8 *buffer
) {
        print_buffer(STDOUT, buffer);
}

void
println(
        const u8 *buffer
) {
        print(buffer);
        print(b"\n");
}

void
printerr(
        const u8 *buffer
) {
        print_buffer(STDERR, b"[ERROR] ");
        print_buffer(STDERR, buffer);
}

void
printerrln(
        const u8 *buffer
) {
        printerr(buffer);
        print_buffer(STDERR, b"\n");
}

#ifndef NDEBUG
#ifndef NDEBUG_CONIO
void
printdbg(
        const u8 *buffer
) {
        print(b"[DEBUG] ");
        print(buffer);
}

void
printdbgln(
        const u8 *buffer
) {
        print(b"[DEBUG] ");
        println(buffer);
}
#else
#define CONIO_PRINTDBG_STUB
#endif/*NDEBUG_CONIO*/
#else
#define CONIO_PRINTDBG_STUB
#endif/*NDEBUG*/
#ifdef CONIO_PRINTDBG_STUB
void
printdbg(
        const u8 *buffer
) {
        (void)buffer;
}

void
printdbgln(
        const u8 *buffer
) {
        (void)buffer;
}
#endif/*CONIO_PRINTDBG_STUB*/
