#include "muc.h"

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "platform-dependent/itoa.h"
#include "cli.h"

static void print(char *x)
{
	size_t x_length;
	
	assert(x != NULL);

	x_length = strlen(x);


	if (x_length != (size_t)write(1, x, x_length)) {
		assert(0 && "Failed to print full message to stdout");
	}
}

static void print_argc(int const argc) {

	char argcbuf[4] = {0};

	char *argcstr = itoa(argc, argcbuf);

	print("Value of argc is : ");
	print(argcstr);
	print(".\n");

}

int main(int argc, char ** argv, __attribute_maybe_unused__ char ** env)
{

	int i;
	struct CliContext ctx = {0};
	char *cli_collection_with_defaults[] = {"Hello", "World"};

	memset(&ctx, 0, sizeof(ctx));
	
	print_argc(argc);
	
	print("Printing all values of argv below:\n\n");
	
	for (i = 0; i < argc; ++i) {
		
		char ibuf[4] = {0};
		char *istr = itoa(i, ibuf);
		
		print("Value of argv[");
		print(istr);
		print("] is ");
		print(argv[i]);
		print("\n");
		
	}
	
	cli_register_collection(&ctx, "--as", NULL);
	cli_register_collection(&ctx, "--with", cli_collection_with_defaults);

	print("Printing args.\n");
	for (i = 0; i < 2; ++i) {
		int j = -1;

		print(ctx.registry.collections[i].name);
		print("\n");

		while(*ctx.registry.collections[i].values[++j]) {
			print("value -> ");
			print(ctx.registry.collections[i].values[j]);
			print("\n");
		}
	}
	print("Printed args.\n");

	return 0;

}
