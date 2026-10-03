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

#include <amuc.h>
#include <ssys.h>
#include <uassert.h>
#include <ustring.h>
#include <acli.h>
#include <mtest.h>
#include <uconio.h>

static void prime_context(struct CliContext cli_context[1])
{
        union CliResourceValue cli_generic_value;
        cli_generic_value.string = b"A very alpha version!";

        cli_register_collection (cli_context, b"with");
        cli_register_collection (cli_context, b"as");
        cli_register_toggle     (cli_context, b"quiet");
        cli_register_action     (cli_context, b"test" , muc_test);
        cli_register_resource   (cli_context, b"program:version",
                resource_string,
                cli_generic_value);
        cli_register_verify(cli_context);
}

s8 main(const s32 argc, const u8 *argv[], const u8 *env[])
{
        u8 rc[1] = {0};
        struct CliContext cli_context[1];
        const volatile u8 hang = 1;
        struct CliCallback callback;

        cli_init_context(cli_context);

        cli_register_args(cli_context, argv, (const s32)argc);

        prime_context(cli_context);

        printdbgln(b"Parsing initial arguments...");
        cli_parse_init(cli_context, rc);
        if(0 != *rc) {
                printerrln(b"Failed to parse initial arguments.");
                return 1;
        } else {
                printdbgln(b"Parsed initial arguments.");
        }

        printdbgln(b"Parsing callback arguments...");
        cli_parse_callback(cli_context, &callback, rc);
        if(0 != *rc) {
                printerrln(b"Failed to parse callback arguments.");
                return 1;
        } else {
                printdbgln(b"Parsed callback.");
        }

        printdbgln(b"Executing callbacks...");
        callback.callback(callback.context, rc);


        /*cli_execute_callback(cli_context, rc);*/
        if(0 != *rc) {
                printerrln(b"Failed to execute callback.");
                return 1;
        } else {
                printdbgln(b"Executed callbacks.");
        }

        printdbgln(b"Callback executed successfully.");

        (void) env;

        while(hang);

        return 0;
}
