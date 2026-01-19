#include <stddef.h>

static const size_t MAX_ARGUMENT_LENGTH = (1024 - sizeof(void *) *2); /* CliArgument will be 1k */

struct CliArgument {

    struct CliArgument *children;       /* Variable number of children */

    char argument[MAX_ARGUMENT_LENGTH]; /* Used to store cli args e.g. filepaths or --new-muc etc */

};

/* Argument utility functions */
void cli_argument_init_children(
    const size_t child_count,
    struct CliArgument *cli_argument
);

/* Argument parsing */

void cli_argument_parse_argv(
    int argc,
    char ** argv,
    struct CliArgument *cli_argument
);

/* Argument handling */

void cli_register_argument_handler(
    const char *const argument,
    void (*handler) (const struct CliArgument *const)
);

void cli_handle_arguments(void);