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
#include <ssys.h>
void muc_test(const struct CliCallbackContext *ctx, u8 *rc)
{
        struct CliCollectionIterator it;
        u8 *buf;
        
        cli_collection_begin(&it, ctx, (u8*) "with");

        while((buf = cli_collection_next(&it))) {
                write(STDOUT, (u8*)"Received value: ", 16);
                write(STDOUT, buf, strlen(buf));
                write(STDOUT, (u8*)"\n", 1);
        }

        cli_collection_reset(&it);

        assert(5==write(STDOUT, (u8*)"abcd\n", 5)&&"Please work :)");
        CLI_ACTION_SET_EXIT_CODE(0);
}
