#include "cli.h"

#include <assert.h>

static const size_t EXPECTED_CLI_ARGUMENT_SIZE = 1024;

static void arena_allocate(
    const size_t size,
    void **ptr /* void *ptr will be overwritten on success */
) {

    /* Define static variables */

    static const size_t ARENA_SIZE = 65536;
    static char arena[ARENA_SIZE]; /* Memory block for all arena allocations */
    size_t static offset_last = 0; /* Start of the arena, update on alloc for next free block */

    /* Main function body */
    
    /* Handle case of not enough remaining space */
    if(ARENA_SIZE < offset_last + size) {
        *ptr = NULL;

        return;
    }

    /* Consider memory used */
    offset_last += size;

    /* Set *ptr to requested block */
    *ptr = (void *)&arena[offset_last]; 

}

void cli_argument_init_children(
    const size_t child_count,
    struct CliArgument *cli_argument
) {
    assert(sizeof(struct CliArgument) == EXPECTED_CLI_ARGUMENT_SIZE && "CliArgument should be expected size.");
    assert(cli_argument->children == NULL && "cli_argument should have no children pre-children-init.");

    /* Allocate the size of a child * the amount of children wanted from arena */
    arena_allocate(sizeof(struct CliArgument) * child_count, &cli_argument->children);

    assert(cli_argument->children != NULL && "cli_argument should have children set post-children-init.");
}