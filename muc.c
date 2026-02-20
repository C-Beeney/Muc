#include <muc.h>
#include <util/sys.h>
#include <util/assert.h>
#include <cli.h>

i32 main(i32 argc, u8 ** argv, u8 ** env)
{	

	char *names[2] = {"abcdef", "ghijkl"};

	write((u32) STDOUT, (u8*) names[0], (imax) 1);
	write((u32) STDOUT, (u8*) names[1], (imax) 1);

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
