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

#ifndef UCONIO_H
#define UCONIO_H
#include <stypes.h>

/* Prints [buffer] to STDOUT.
   assumes [buffer] is NULL-terminated.
   asserts success.
*/
void print(const u8 *buffer);

/* Prints [buffer] to STDOUT with newline.
   assumes [buffer] is NULL-terminated.
   asserts success.
*/
void println(const u8 *buffer);

/* Prints [buffer] to STDERR.
   assumes [buffer] is NULL-terminated.
   asserts success.
*/
void printerr(const u8 *buffer);

/* Prints [buffer] to STDERR with newline.
   assumes [buffer] is NULL-terminated.
   asserts success.
*/
void printerrln(const u8 *buffer);

/* Prints [buffer] to STDOUT unless NDEBUG_CONIO or NDEBUG is defined.
   assumes [buffer] is NULL-terminated.
   asserts success.
*/
void printdbg(const u8 *buffer);

/* Prints [buffer] to STDOUT with newline unless NDEBUG/_CONIO defined.
   assumes [buffer] is NULL-terminated.
   asserts success.
*/
void printdbgln(const u8 *buffer);


#endif/*UCONIO_H*/
