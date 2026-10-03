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
cli_init_context(
        struct CliContext context[1]
) {
        assert(context);
        assert(context->registry);
        assert(context->registry->counter);

        assert(context->common_context);
        assert(context->common_context->counter);
 
        context->registry->counter->toggle     = 0;
        context->registry->counter->collection = 0;
        context->registry->counter->action     = 0;
        context->registry->counter->resource   = 0;

        context->common_context->counter->toggle     = 0;
        context->common_context->counter->collection = 0;

}

void
cli_register_toggle(
        struct CliContext context[1],
        const u8          name[]
) {
        umax counter;
        struct CliToggle *this;

        assert(context);
        assert(context->registry);
        assert(context->registry->counter);
        assert(context->registry->counter->toggle < CLI_TOGGLE_COUNT);

        assert(name);

        counter = context->registry->counter->toggle++;

        this = context->registry->toggle + counter;

        this->name  = name;
        this->value = 0;
}

void
cli_register_collection(
        struct CliContext context[1],
        const u8          name[]
) {
        umax counter;
        struct CliCollection *this;

        assert(context);
        assert(context->registry);
        assert(context->registry->counter);
        assert(context->registry->counter->collection < CLI_COLLECTION_COUNT);

        assert(name);

        counter = context->registry->counter->collection++;

        assert(context->registry->collection);

        this = context->registry->collection + counter;

        this->name = name;
}

void
cli_register_action(
        struct CliContext context[1],
        const u8          name[],
        const CliCallbackFunction callback
) {
        umax counter;
        struct CliAction *this;

        assert(context);
        assert(context->registry);
        assert(context->registry->counter);
        assert(context->registry->counter->action < CLI_ACTION_COUNT);
        
        assert(name);
        assert(callback);

        assert(context->registry->action);

        counter = context->registry->counter->action++;

        this = context->registry->action + counter;

        this->name = name;
        this->callback = callback;
}

void
cli_register_resource(
        struct CliContext     context[1],
        const u8              name[],
        const enum CliResourceType   type,
        const union CliResourceValue value
) {
        umax counter;
        struct CliResource *this;

        assert(context);
        assert(context->registry);
        assert(context->registry->counter);
        assert(context->registry->counter->resource < CLI_RESOURCE_COUNT);

        assert(name);

        assert(type >= 0);
        assert(type < CLI_RESOURCE_IDENT_COUNT);

        assert(context->registry->resource);
        
        counter = context->registry->counter->resource++;
        
        this = context->registry->resource + counter;

        this->name = name;
        this->value->type = type;
        this->value->value = value;
}

void cli_register_args(
        struct CliContext context[1],
        const u8         *argv    [],
        const s32         argc
) {

        assert(context);
        assert(context->args);
        assert(context->args);

        assert(argv);
        
        assert(argc >= 0);

        context->args->args = argv;
        context->args->count = (u32)argc;
        context->args->index = 1; /* Skip path to executable */
}
