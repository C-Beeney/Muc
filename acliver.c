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

void
cli_register_verify(
        const struct CliContext context[1]
) {
        umax index;

        assert(context->registry->counter->action
                        == CLI_ACTION_COUNT);
        assert(context->registry->counter->collection
                        == CLI_COLLECTION_COUNT);
        assert(context->registry->counter->resource
                        == CLI_RESOURCE_COUNT);
        assert(context->registry->counter->toggle
                        == CLI_TOGGLE_COUNT);


        for(    index = 0;
                index < context->registry->counter->action;
                ++index
        ) {
                assert(context->registry->action[index].name);
                assert(context->registry->action[index].callback);
        }

        for(    index = 0;
                index < context->registry->counter->collection;
                ++index
        ) {
                assert(context->registry->collection[index].name);
        }

        for(    index = 0;
                index < context->registry->counter->resource;
                ++index
        ) {
                assert(context->registry->resource[index].name);
        }

        for(    index = 0;
                index < context->registry->counter->toggle;
                ++index
        ) {
                assert(context->registry->toggle[index].name);
        }
}

void
cli_verify_action_tag(
        const struct CliRegistry registry[1],
        const u8               name        [] ,
        u8                     rc          [1]
) {
        const struct CliAction *actions = registry->action;
        umax action_index_max = registry->counter->action;
        umax index;

        assert(name[0] == '-' && name[1] == '-');

        *rc = 0;

        for(    index = 0;
                index < action_index_max;
                ++index
        ) {
                *rc = streq(actions[index].name, name + 2);
                if (*rc) { return; }
        }
}

void
cli_verify_collection_tag(
        const struct CliRegistry   registry [1],
        const u8                   name     [] ,
        u8                         rc       [1]
) {
        const struct CliCollection *collections = registry->collection;
        umax collection_index_max = registry->counter->collection;
        umax index;

        *rc = 0;

        if(name[0] != '-' || name[1] != '-') {
                *rc = 1;
                return;
        }

        for (   index = 0;
                index < collection_index_max;
                ++index
        ) {
                *rc = streq(collections[index].name, name + 2);
                if (*rc) { return; }
        }
}

void
cli_verify_toggle_tag(
        const struct CliRegistry registry    [1],
        const u8                 name        [] ,
        u8                       rc          [1]
) {
        const struct CliToggle *toggles = registry->toggle;
        umax toggle_index_max = registry->counter->toggle;
        umax index;

        assert(name[0] == '-' && name[1] == '-');

        *rc = 0;

        for(    index = 0;
                index < toggle_index_max;
                ++index
        ) {
                *rc = streq(toggles[index].name, name + 2);
                if (*rc) { return; }
        }
}
