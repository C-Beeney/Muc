#include <stddef.h>

#ifndef CLI_COLLECTION_REGISTER_COUNT
#define CLI_COLLECTION_REGISTER_COUNT 16
#endif/*CLI_COLLECTION_REGISTER_COUNT*/

#ifndef CLI_COLLECTION_CHILD_COUNT
#define CLI_COLLECTION_CHILD_COUNT 16
#endif

#ifndef CLI_TOGGLE_REGISTER_COUNT
#define CLI_TOGGLE_REGISTER_COUNT 16
#endif/*CLI_TOGGLE_REGISTER_COUNT*/

#ifndef CLI_ACTIONS_REGISTER_COUNT
#define CLI_ACTIONS_REGISTER_COUNT 16
#endif/*CLI_ACTIONS_REGISTER_COUNT*/

#ifndef CLI_REGISTRY_NAME_MAX_LENGTH
#define CLI_REGISTRY_NAME_MAX_LENGTH 16
#endif

struct CliContext;

struct CliContextCollection {
    char name[CLI_REGISTRY_NAME_MAX_LENGTH];
    char values[CLI_COLLECTION_CHILD_COUNT][CLI_REGISTRY_NAME_MAX_LENGTH];
};

struct CliContextToggle {
    char name[CLI_REGISTRY_NAME_MAX_LENGTH];
    char is_true;
};


struct CliContextAction {
    char name[CLI_REGISTRY_NAME_MAX_LENGTH];
    void (*action) (struct CliContext *context);
};

struct CliContextRegistry {
    struct CliContextCollection collections[CLI_COLLECTION_REGISTER_COUNT];
    struct CliContextToggle toggles[CLI_TOGGLE_REGISTER_COUNT];
    struct CliContextAction actions[CLI_ACTIONS_REGISTER_COUNT];
};

struct CliContext {
    struct CliContextRegistry registry;
};


void cli_register_collection( struct CliContext *cli_context,
                              char *name,
                              char **default_values
                            );

void cli_register_toggle( struct CliContext *cli_context,
                          char *name,
                          char default_is_true
                        );

void cli_reigster_action( struct CliContext *cli_context,
                          char *name,
                          void(*action)(void)
                        );
