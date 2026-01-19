#include "muc.h"

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "platform-dependent/itoa.h"
#include "cli.h"

#define print(x) write(1, x, strlen(x))

void print_argc(int const argc) {

    char argcbuf[4] = {0};

    char *argcstr = itoa(argc, argcbuf);

    print("Value of argc is : ");
    print(argcstr);
    print(".\n");


}

int main(int argc, char ** argv, char ** env) {

    print_argc(argc);

    print("Printing all values of argv below:\n\n");
    for (int i = 0; i < argc; ++i) {

        char *_;

        print("Value of argv[");
        char ibuf[4] = {0};
        char *istr = itoa(i, ibuf);
        print(istr);
        print("] is ");
        print(argv[i]);
        print("\n");

    }

    struct CliArgument cli_args = {0};

    strcpy(cli_args.argument, "--with-files");

    cli_argument_init_children(2, &cli_args);

    strcpy(cli_args.children[0].argument, "somefile.txt");
    strcpy(cli_args.children[1].argument, "someotherfile.bin");

    return 0;

}
