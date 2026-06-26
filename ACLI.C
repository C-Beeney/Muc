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

        assert(context                    && "Toggle.");
        assert(name                       && "Toggle.");
        assert((0 == value || 1 == value) && "Toggle.");
        assert(counter < CLI_TOGGLE_COUNT && "Toggle.");

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

        assert(context                        && "Collection.");
        assert(name                           && "Collection.");
        assert(value                          && "Collection.");
        assert(counter < CLI_COLLECTION_COUNT && "Collection.");

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

        assert(context                    && "Action.");
        assert(name                       && "Action.");
        assert(callback                   && "Action.");
        assert(counter < CLI_ACTION_COUNT && "Action.");

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

        assert(context                      && "Resource.");
        assert(name                         && "Resource.");
        assert(type >= 0                    && "Resource.");
        assert(type < CLI_GENERIC_MAX       && "Resource.");
        assert(counter < CLI_RESOURCE_COUNT && "Resource.");

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
cli_parse_args_verify_action_tag(
        const struct CliRegistry registry[1],
        const u8               name        [] ,
        u8                     rc          [1]
) {

        const struct CliAction *actions = registry->action;
        umax action_index_max = registry->counters->action;

        umax index = UMAX_MAX;

        for(;
                ++index < action_index_max;
                *rc = streq(actions[index].name, name)
        ) if (*rc) return;
}

static void
cli_parse_args_verify_collection_tag(
        const struct CliRegistry   registry [1],
        const u8                   name     [] ,
        u8                         rc       [1]
) {
        const struct CliCollection *collections = registry->collection;
        umax collection_index_max = registry->counters->collection;

        umax index = UMAX_MAX;

        for(;;) {
                index += 1;
                if(index > collection_index_max) break;
                *rc = streq(collections[index].name, name);
                if (*rc) { return; }
        }
}

static void
cli_parse_args_verify_toggle_tag(
        const struct CliRegistry registry    [1],
        const u8                 name        [] ,
        u8                       rc          [1]
) {
        const struct CliToggle *toggles = registry->toggle;
        umax toggle_index_max = registry->counters->toggle;
        umax index = UMAX_MAX;
        *rc = 0;
        for(;
                ++index < toggle_index_max;
                *rc = streq(toggles[index].name, name)
        ) if (*rc) return;
}

static void
cli_parse_args_verify_tag(
        const struct CliRegistry registry[1],
        const u8                 tag[] ,
        u8                       rc[1]
) {
        cli_parse_args_verify_action_tag(
                registry,
                tag,
                rc);
        if(*rc) {
                return;
        }

        cli_parse_args_verify_collection_tag(
                registry,
                tag,
                rc);
        if(*rc) {
                return;
        }

        cli_parse_args_verify_toggle_tag(
                registry,
                tag,
                rc);
}

static void
cli_parse_args_parse_collection(
        const struct CliRegistry registry  [1],
        struct CliCollection     collection[1],
        u8                      *argv[]       ,
        umax                     argv_index[1],
        const i32                argc
) {
        umax index = UMAX_MAX;
        u8 rc[1];

        for(;;) {
                index += 1; 
                *argv_index += 1;

                assert(argc >= 0);
                if(*argv_index >= (umax)argc) { return; }

                cli_parse_args_verify_tag(registry, argv[*argv_index], rc);

                if(*rc) { break; }
                assert(index >= 0 && (index) < CLI_COLLECTION_MAX_CHILDREN);
                collection->value[index] = argv[*argv_index];
                assert(argc >= 0);
                if(*argv_index >= (umax) argc) {
                        assert(0 && "Invalid syntax!");
                }
        }

        return;
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
cli_parse_args_parse_initial_state(
        struct InitialState initial_state[1],
        struct CliRegistry  registry     [1],
        u8                 *argv         [] ,
        umax                argv_index   [1],
        i32                 argc            ,
        u8                  rc[1]


) {
        for(;;) {
                assert(argc >= 0);
                if(*argv_index >= (umax) argc) break;
                cli_parse_args_verify_toggle_tag(
                        registry,
                        argv[*argv_index],
                        rc);
                if(*rc) {
                        umax toggle_count =
                                initial_state->counters.toggle ++;
                        struct CliToggle *toggles =
                                initial_state->toggle;
                        if(toggles[toggle_count].value != 0) {
                                assert(0 && "Toggled 2x!");
                        }
                        continue;
                }
                cli_parse_args_verify_collection_tag(
                        registry,
                        argv[*argv_index],
                        rc);
                if(*rc) {

                        umax collection_count = 
                                initial_state->counters.collection ++;
                        struct CliCollection *collections =
                                initial_state->collection;
                        cli_parse_args_parse_collection(
                                registry,
                                collections + collection_count,
                                argv,
                                argv_index,
                                argc);
                        continue;
                }

                cli_parse_args_verify_action_tag(
                        registry,
                        argv[*argv_index],
                        rc);

                if(*rc) {
                        break;
                }
                *argv_index = *argv_index + 1;
        }

        *argv_index -= 1;
        return;
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
        cli_parse_args_parse_initial_state(
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

        {
                umax i = 0;
                write(STDOUT, (u8*)"weewoo\n", 7);
                for (i=0; i < initial_state->counters.collection; ++i) {
                        umax j = 0;
                        write(STDOUT, (u8*)"hoy\n", 4);

                        write(STDOUT, initial_state->collection[i].name, (imax)strlen(initial_state->collection[i].name));
                        while(initial_state->collection[i].value[j]) {
                                write(STDOUT, (u8*)"hey\n", 4);
                                write(STDOUT, initial_state->collection[i].value[j], (imax)strlen(initial_state->collection[i].value[j]));
                                j += 1;
                        }
                }
                write(STDOUT, (u8*)"woowee?\n", 8);
        }
        for(;
                0 != next_is_tag;
                ++ argv_index
        ) {
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
                        cli_parse_args_verify_tag(
                                registry,
                                argv[argv_index+1],
                                &next_is_tag);
        }
}
