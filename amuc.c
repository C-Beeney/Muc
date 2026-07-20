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

void prime_context(struct CliContext cli_context[1]);
void prime_context(struct CliContext cli_context[1])
{
        const u8 *collection_with[2] = {(u8*)"abc", (u8*)0};
        const u8 *collection_as[2] = {(u8*)"def", (u8*)0};
        union CliGenericValue cli_generic_value = {(u8*)"Hello, World!"};

        cli_register_collection (cli_context, (u8*)"--with" , collection_with);
        cli_register_collection (cli_context, (u8*)"--as"   , collection_as);
        cli_register_toggle     (cli_context, (u8*)"--quiet", 0);
        cli_register_action     (cli_context, (u8*)"--test" , muc_test);
        cli_register_resource   (cli_context, (u8*)"program:version",
                resource_string,
                cli_generic_value);
        cli_register_verify(cli_context);
}

i8 main(i32 argc, u8 *argv[], u8 *env[])
{

        struct CliContext cli_context[1];

        cli_init_context(cli_context);

        prime_context(cli_context);

        cli_parse_args(cli_context, argc, argv);

        (void) env;

        return 0;
}
