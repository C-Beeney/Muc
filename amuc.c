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

static void prime_context(struct CliContext cli_context[1])
{
        union CliResourceValue cli_generic_value;
        cli_generic_value.string = (u8*)"A very alpha version!";

        cli_register_collection (cli_context, (u8*)"with");
        cli_register_collection (cli_context, (u8*)"as");
        cli_register_toggle     (cli_context, (u8*)"quiet");
        cli_register_action     (cli_context, (u8*)"test" , muc_test);
        cli_register_resource   (cli_context, (u8*)"program:version",
                resource_string,
                cli_generic_value);
        cli_register_verify(cli_context);
}

s8 main(const s32 argc, const u8 *argv[], const u8 *env[])
{
        u8 rc[1] = {0};
        struct CliContext cli_context[1];
        const volatile u8 hang = 1;
        enum status status[1];

        cli_init_context(cli_context);

        cli_register_args(cli_context, argv, (const s32)argc);

        prime_context(cli_context);

        cli_parse_init(cli_context, rc);
        if(0 != *rc) {
                write(STDERR, (u8*)"Error: Failed to parse initial arguments.\n", 43, status);
                return 1;
        }

        cli_parse_callback(cli_context, rc);
        if(0 != *rc) {
                write(STDERR, (u8*)"Error: Failed to parse callback arguments.\n", 44, status);
                return 1;
        }

        cli_execute_callback(cli_context, rc);
        if(0 != *rc) {
                write(STDERR, (u8*)"Error: Failed to execute callback.\n", 36, status);
                return 1;
        }

        write(STDOUT, (u8*)"Success: Callback executed successfully.\n", 42, status);

        (void) env; (void) argc; (void) argv;

        while(hang);

        return 0;
}
