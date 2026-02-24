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

#include <ACLI.H>
#include <UASSERT.H>
#include <SSYS.H>
#include <STYPES.H>
#include <USTRING.H>

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
        assert(0 == value || 1 == value   && "Toggle.");
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
        enum CliGenericType   type,
        union CliGenericValue value
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

void
cli_register_verify(
        struct CliContext context[1]
) {
        assert(context->registry->counters->action + 1
                        == CLI_ACTION_COUNT);
        assert(context->registry->counters->collection + 1
                        == CLI_COLLECTION_COUNT);
        assert(context->registry->counters->resource + 1
                        == CLI_RESOURCE_COUNT);
        assert(context->registry->counters->toggle + 1
                        == CLI_TOGGLE_COUNT);
}

static void
cli_parse_args_verify_action_tag(
        struct CliRegistry registry[1],
        u8                 tag     [] ,
        u8                 rc      [1]
) {
        umax index;

        for(
                index = UMAX_MAX;
                ++index < registry->counters->action;
                *rc = streq(registry->action[index].name, tag)
        ) if (*rc) return;
}

static void
cli_parse_args_verify_collection_tag(
        struct CliRegistry registry[1],
        u8                 tag     [] ,
        u8                 rc      [1]
) {
        umax index;

        for(
                index = UMAX_MAX;
                ++index < registry->counters->collection;
                *rc = streq(registry->collection[index].name, tag)
        ) if (*rc) return;
}

static void
cli_parse_args_verify_toggle_tag(
        struct CliRegistry registry[1],
        u8                 tag     [] ,
        u8                 rc      [1]
) {
        umax index;

        for(
                index = UMAX_MAX;
                ++index < registry->counters->toggle;
                *rc = streq(registry->toggle[index].name, tag)
        ) if (*rc) return;
}

static void
cli_parse_args_verify_tag(
        struct CliRegistry registry[1],
        u8                 tag[] ,
        u8                 rc[1]
) {
        cli_parse_args_verify_action_tag     (registry, tag, rc);
        if(rc) return;
        cli_parse_args_verify_collection_tag (registry, tag, rc);
        if(rc) return;
        cli_parse_args_verify_toggle_tag     (registry, tag, rc);
}

void
cli_parse_args(
        struct CliContext context[1],
        i32 argc,
        u8 *argv[]
) {
        struct InitialState {
                struct CliCollection collection[CLI_COLLECTION_COUNT];
                struct CliToggle     toggle    [CLI_TOGGLE_COUNT];
        
                /*MAYBE PADDING*/
        };
        
        struct CliContextualisedCallback *callback = context->callback;
        struct CliRegistry *registry = context->registry;

        struct InitialState initial_state[1];

        umax argv_index;
        u8 next_is_tag;
        for(
                argv_index = 0, next_is_tag = 1;
                0 != next_is_tag;
                ++ argv_index
        ) {
                /* Code goes here (todo) */
        }


        (void) context; (void) argc; (void) argv;
}
