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

#include <mtest.h>
#include <uassert.h>
#include <uconio.h>
#include <ssys.h>

void muc_test(const struct CliCallbackContext *ctx, u8 *rc)
{
        struct CliCollectionIterator it[1];
        const u8 *buf;

        cli_collection_begin(it, ctx, b"with");

        while((buf = cli_collection_next(it))) {
                println(buf);
        }

        cli_collection_change(it, b"as");

        while((buf = cli_collection_next(it))) {
                println(buf);
        }
        println(b(cli_toggle_get(ctx, b"quiet") ? "YES" : "NO"));

        *rc = OK;
}
