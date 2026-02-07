#include "cli.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "util/muc_copystring.h"

#define MUC_CLI_DBG

#define TRACE(MESSAGE)

#ifdef MUC_CLI_DBG
        #include <unistd.h>
        #include <string.h>

        #define MUC_CLI_DBG_PRINTCONST_STDOUT 1

        #define MUC_CLI_DBG_STACK_PRINT(STRING)                                             \
                assert(STRING && "MUC_CLI_DBG_STACK_PRINT : May not be called upon NULL."); \
                (void)write(MUC_CLI_DBG_PRINTCONST_STDOUT, STRING, sizeof(STRING))

        #define MUC_CLI_DBG_HEAPV_PRINT(STRING)                                             \
                assert(STRING && "MUC_CLI_DBG_HEAPV_PRINT : May not be called upon NULL."); \
                (void)write(MUC_CLI_DBG_PRINTCONST_STDOUT, STRING, strlen(STRING))
        

        #ifdef TRACE
        #undef TRACE
        #endif

        #define TRACE(MESSAGE)                            \
                MUC_CLI_DBG_STACK_PRINT("[TRACE] :: "); \
                MUC_CLI_DBG_HEAPV_PRINT(MESSAGE);       \
                MUC_CLI_DBG_STACK_PRINT("\n")
#endif /*MUC_CLI_DBG*/

void cli_register_collection( struct CliContext *cli_context, char *name, char **default_values) {
        static short collectionsCount = -1; /*Will be set to 0 by first use.*/

        struct CliContextCollection *collection;

        collectionsCount += 1;

        assert(cli_context != NULL);
        assert(name != NULL);
        assert(strlen(name) < CLI_REGISTRY_NAME_MAX_LENGTH);
        assert(collectionsCount < CLI_COLLECTION_REGISTER_COUNT);
        
        collection = cli_context->registry.collections + collectionsCount;
        TRACE("Copying collection name.");

        muc_copystring(collection->name, CLI_REGISTRY_NAME_MAX_LENGTH, name);
        
        if (! default_values) {
                
                /* Indicates end of values */
                collection->values[0][0]='\0';
                
                TRACE ("No default values, returning.");
                return;
        }
        
        {
                short default_values_index;
                
                TRACE("Copying over default values.");
                for(default_values_index = 0; default_values[default_values_index] != NULL; ++default_values_index) {
                        char *dest = collection->values[default_values_index];
                        char *source = default_values[default_values_index];

                        TRACE("Copying...");

                        assert(dest && "Destination must not be NULL");
                        assert(source && "Source must not be NULL");

                        muc_copystring(dest, CLI_REGISTRY_NAME_MAX_LENGTH, source);
                }

                TRACE("Default values copied, setting null terminal.");
                collection->values[default_values_index][0]='\0';

        }

}

void cli_register_toggle( struct CliContext *cli_context,
                          char *name,
                          char default_is_true
                        );

void cli_reigster_action( struct CliContext *cli_context,
                          char *name,
                          void(*action)(void)
                        );
