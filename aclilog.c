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

#include <acli.h>
#include <ssys.h>
#include <uconio.h>

#if 0
static void
cli_print_collection(
        const struct CliCollection collection[1],
        const struct CliArgData arg_data[1]
){
        u16 s, o;

        print(b"\n\t");
        print(collection->name);
        println(b":");


        for(    s = 0;
                collection->start[s] != 0 && s < CLI_COLLECTION_MAX_STARTS;
                ++s
        ) {
                u32 s_offs = collection->start[s];
                for(    o = 0;
                        arg_data->args[s_offs+o][0]!='-';
                        ++o
                ) {
                        const u8 *buf = arg_data->args[s_offs+o];
                        print(b"\t\t");
                        println(buf);
                }
        }


}
#endif

#if 0
static void
cli_print_collections(
        const struct CliCollection *collections ,
        const umax                  collection_count,
        const struct CliArgData     arg_data[1]
) {
        u16 i = 0;
        {       u8 buffer[32];
                print(b"Collections parsed: ");
                assert(collection_count < SMAX_MAX);
                itoa((smax)collection_count, buffer);
                println(buffer);
        }
        

        print(b"Collections:");
        for(    i=0;
                i < collection_count;
                ++i
        ) {
                cli_print_collection(collections + i, arg_data);
        }
        println(b null);
}
#endif

#if 0
static void
cli_print_toggle(
        const struct CliToggle toggle[1]
) {
        print(b"\t");
        print(toggle->name);
        print(b":");
        print(b(toggle->value ? "true" : "false"));
}
#endif

#if 0
static void
cli_print_toggles(
        const struct CliToggle *toggles,
        const umax              toggle_count
) {
        u16 i = 0;
        println(b"Toggles:");
        for(    i=0;
                i < toggle_count;
                ++i
        ) {
                cli_print_toggle(toggles + i);
        }
        println(b null);
}
#endif
