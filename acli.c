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

#include <acli.h>
#include <uassert.h>
#include <ssys.h>
#include <stypes.h>
#include <ustring.h>

void
cli_init_context(
        struct CliContext context[1]
) {
                /* Set to maximum, ready to overflow */
        context->registry->counters->toggle     = UMAX_MAX;
        context->registry->counters->collection = UMAX_MAX;
        context->registry->counters->action     = UMAX_MAX;
        context->registry->counters->resource   = UMAX_MAX;

                /* No other values require initialisation */
}

void
cli_register_toggle(
        struct CliContext context[1],
        const u8          name[],
        const u8          value
) {
        umax counter = ++ context->registry->counters->toggle;

        struct CliToggle *this;

        assert(context);
        assert(name);
        assert((0 == value || 1 == value));
        assert(counter < CLI_TOGGLE_COUNT);

        this = context->registry->toggle + counter;

        this->name  = name;
        this->value = value;
}

void
cli_register_collection(
        struct CliContext context[1],
        const u8          name[],
        const u8         *value[CLI_COLLECTION_MAX_CHILDREN]
) {
        umax counter = ++ context->registry->counters->collection;

        struct CliCollection *this;
        imax copy_index;

        assert(context);
        assert(name);
        assert(value);
        assert(counter < CLI_COLLECTION_COUNT);

        this = context->registry->collection + counter;

        this->name = name;

        for (copy_index = 0; 0 != value[copy_index]; ++ copy_index)
                this->value[copy_index] = value[copy_index];
}

void
cli_register_action(
        struct CliContext context[1],
        const u8          name[],
        const CliCallback callback
) {
        umax counter = ++ context->registry->counters->action;

        struct CliAction *this;

        assert(context);
        assert(name);
        assert(callback);
        assert(counter < CLI_ACTION_COUNT);

        this = context->registry->action + counter;

        this->name = name;
        this->callback = callback;
}

void
cli_register_resource(
        struct CliContext     context[1],
        const u8              name[],
        const enum CliGenericType   type,
        const union CliGenericValue value
) {
        umax counter = ++ context->registry->counters->resource;

        struct CliResource *this;

        assert(context);
        assert(name);
        assert(type >= 0);
        assert(type < CLI_GENERIC_MAX);
        assert(counter < CLI_RESOURCE_COUNT);

        this = context->registry->resource + counter;

        this->name = name;
        this->value->type = type;
        this->value->value = value;
}

/*
        * TODO: assert names are present!
*/
void
cli_register_verify(
        const struct CliContext context[1]
) {
        assert(context->registry->counters->action + 1
                        == CLI_ACTION_COUNT);
        assert(context->registry->counters->collection + 1
                        == CLI_COLLECTION_COUNT);
        assert(context->registry->counters->resource + 1
                        == CLI_RESOURCE_COUNT);
        assert(context->registry->counters->toggle + 1
                        == CLI_TOGGLE_COUNT);
        
        (void) context;
}

static void
cli_verify_action_tag(
        const struct CliRegistry registry[1],
        const u8               name        [] ,
        u8                     rc          [1]
) {
        const struct CliAction *actions = registry->action;
        umax action_index_max = registry->counters->action;        
        umax index;

        *rc = 0;

        for(
                index = 0;
                index <= action_index_max;
                ++index
        ) {
                *rc = streq(actions[index].name, name);
                if (*rc) { return; }
        }
}

static void
cli_verify_collection_tag(
        const struct CliRegistry   registry [1],
        const u8                   name     [] ,
        u8                         rc       [1]
) {
        const struct CliCollection *collections = registry->collection;
        umax collection_index_max = registry->counters->collection;
        umax index;

        *rc = 0;
        
        for (   index = 0;
                index <= collection_index_max;
                ++index
        ) {
                *rc = streq(collections[index].name, name);
                if (*rc) { return; }
        }
}

static void
cli_verify_toggle_tag(
        const struct CliRegistry registry    [1],
        const u8                 name        [] ,
        u8                       rc          [1]
) {
        const struct CliToggle *toggles = registry->toggle;
        umax toggle_index_max = registry->counters->toggle;
        umax index;

        *rc = 0;

        for(
                index = 0;
                index <= toggle_index_max;
                ++index
        ) {
                *rc = streq(toggles[index].name, name);
                if (*rc) { return; }
        }
}

static void
cli_verify_tag(
        const struct CliRegistry registry[1],
        const u8                 tag[] ,
        u8                       rc[1]
) {
        cli_verify_action_tag(
                registry,
                tag,
                rc);
        if(*rc) {
                return;
        }

        cli_verify_collection_tag(
                registry,
                tag,
                rc);
        if(*rc) {
                return;
        }

        cli_verify_toggle_tag(
                registry,
                tag,
                rc);
}

static void
cli_parse_collection(
        const struct CliRegistry registry  [1],
        struct CliCollection     collection[1],
        u8                      *argv[]       ,
        umax                     argv_index[1],
        const i32                argc
) {
        umax index = 0;
        u8 rc[1];

        collection->name = argv[*argv_index];

        for(;;++index) {
                
                *argv_index += 1;

                assert(argc >= 0);
                if(*argv_index >= (umax)argc) { return; }

                cli_verify_tag(registry, argv[*argv_index], rc);

                if(*rc) { break; }
                
                assert(index >= 0 && (index) < CLI_COLLECTION_MAX_CHILDREN);
                collection->value[index] = argv[*argv_index];
                assert(argc >= 0);
                if(*argv_index >= (umax) argc) {
                        assert(0 && "Invalid syntax!"); /* TODO: Change to proper error reporting */
                }
        }

        return;
}

static void
cli_handle_toggle(
        struct CliToggle *this
) {
        if(this->value != 0) {
                assert(0 && "Toggled 2x! -- REPLACE WITH ERROR!!!");
                exit(127);
        }
}


struct InitialStateCounters {
        umax collection;
        umax toggle;
};

struct InitialState {
        struct CliCollection collection[CLI_COLLECTION_COUNT];
        struct CliToggle     toggle    [CLI_TOGGLE_COUNT];

        struct InitialStateCounters counters;
        /*MAYBE PADDING*/
};

static void
cli_parse_initial_state(
        struct InitialState initial_state[1],
        struct CliRegistry  registry     [1],
        u8                 *argv         [] ,
        umax                argv_index   [1],
        const i32           argc            ,
        u8                  rc[1]


) {
        assert(argc >= 0);

        for(
                *rc = 0;
                *argv_index < (umax) argc;
                *argv_index += 1
        ) {

                cli_verify_toggle_tag(
                        registry,
                        argv[*argv_index],
                        rc);
                if(*rc) {
                        umax toggle_count =
                                initial_state->counters.toggle ++;
                        struct CliToggle *this =
                                initial_state->toggle + toggle_count;
                        cli_handle_toggle(
                                this);
                        continue;
                }
                cli_verify_collection_tag(
                        registry,
                        argv[*argv_index],
                        rc);
                if(*rc) {

                        umax collection_count = 
                                initial_state->counters.collection ++;
                        struct CliCollection *collections =
                                initial_state->collection;
                        cli_parse_collection(
                                registry,
                                collections + collection_count,
                                argv,
                                argv_index,
                                argc);
                        *argv_index -=1;
                        continue;
                }

                cli_verify_action_tag(
                        registry,
                        argv[*argv_index],
                        rc);

                if(*rc) {
                        break;
                }
        }

        --*argv_index;
        return;
}

static void
cli_print_collection(
        const struct CliCollection collection[1]
){
        umax j = 0;

        write(STDOUT, (u8*)"\n\t", 2);
        write(STDOUT, collection->name, strlen(collection->name));
        write(STDOUT, (u8*)":\n", 2);

        while(collection->value[j]) {
                imax value_length = strlen(collection->value[j]);
                write(STDOUT, (u8*)"\t\t", 2);
                write(STDOUT, collection->value[j], value_length);
                write(STDOUT, (u8*)"\n", 1);
                j += 1;
        }
}

static void
cli_print_collections(
        const struct CliCollection *collections ,
        const umax                  collection_count
) {
        umax i = 0;
        write(STDOUT, (u8*)"Collections:", 13);
        for (i=0; i < collection_count; ++i) {
                cli_print_collection(collections + i);
        }
        write(STDOUT, (u8*)"\n", 1);
}

void
cli_parse_args(
        struct CliContext context[1],
        const i32 argc,
        u8 *argv[]
) {
        /* Initial state declared before any actions;
         * Applied across all children.
         */

        struct CliContextualisedCallback *callback = context->callback;
        struct CliRegistry *registry = context->registry;

        struct InitialState initial_state[1] = {0};

        umax argv_index = 1;
        u8 next_is_tag = 1;
        u8 rc[1];

        (void)callback;
        cli_parse_initial_state(
                initial_state,
                registry,
                argv,
                &argv_index,
                argc,
                rc);
                
        assert(argc >= 0);
        if(argv_index >= (umax)argc) {
                assert(0&& "There is no action in cli args!");
        }

        cli_print_collections(
                initial_state->collection,
                initial_state->counters.collection);


        write(STDOUT, (u8*)"WeeWee?!\n", 9);
        for(;
                0 != next_is_tag;
                ++ argv_index
        ) {
                /*write(STDOUT, (u8*)"I AM GEOFF!!!\n", 14); *//*                                      TODO TODO TODO */
                /* Code goes here (todo) */




                /* Check for toggles and collections. */

                        /* Gather context for this action, combine with common context. */
                        /* Work out which action this is. Assign that function pointer. */
                        /* Ensure we don't have too many collections or toggles in context. */
                        /* Ensure we don't surpass maximum actions per program invocation. */

                /* Else, add to current CliContextualisedCallback */


                /* Ensure we update next_is_tag */
                assert(argc >= 0);
                if(argv_index+1 < (umax) argc)
                        cli_verify_tag(
                                registry,
                                argv[argv_index+1],
                                &next_is_tag);
        }
}
