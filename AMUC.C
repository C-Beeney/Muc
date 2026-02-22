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

#include <AMUC.H>
#include <SSYS.H>
#include <UASSERT.H>
#include <USTRING.H>
#include <ACLI.H>

#define print(s) assert(strlen((const u8*)(s))==write(STDOUT,(const u8*)(s),(const imax)strlen((const u8*)(s))))

i8 main(i32 argc, u8 ** argv, u8 ** env)
{	

	struct CliContext cli_context[1];
        const u8 *values[3] = {(u8*)"Test string", (u8*)"another", (u8*)0};
        int offs = 3;

        cli_register_collection(
                (struct CliContext*)    cli_context,
                (u8*)                   "--with",
                (const u8 **)values
        );

        while (0 <-- offs) {
                print(
                        cli_context->registry->collection[0].value[2-offs]
                );
                print("\n");
        }

        (void) argc, (void) argv, (void) env;
	return 0;
}

/*
 * API for cli, register actions, collections & toggles.
 * first item will be the action
 * items beyond will be collections or toggles
 * 	memory will not be assigned. All memory needed SHOULD be prealocated by the kernel and stored under argv.
 * 
 * 						          {reg info}                      {name to match}      {ptr array for argv}
 * 	cli_register_collection((struct CliContext*) cli_context, (u8*) collection_name, (u8**) collection_size)
 * 
 * 	collection called -c which can hold 16 64-bit pointers to string values.
 * 	cli_register_collection(ctx, "-c", malloc(128));
 * 
 * 
 * 
*/
