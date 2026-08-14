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
        context->registry->counter->toggle     = U8_MAX;
        context->registry->counter->collection = U8_MAX;
        context->registry->counter->action     = U8_MAX;
        context->registry->counter->resource   = U8_MAX;

                /* No other values require initialisation */
}

void
cli_register_toggle(
        struct CliContext context[1],
        const u8          name[]
) {
        umax counter = ++ context->registry->counter->toggle;

        struct CliToggle *this;

        assert(context);
        assert(name);
        assert(counter < CLI_TOGGLE_COUNT);

        this = context->registry->toggle + counter;

        this->name  = name;
        this->value = 0;
}

void
cli_register_collection(
        struct CliContext context[1],
        const u8          name[]
) {
        umax counter = ++context->registry->counter->collection;

        struct CliCollection *this;

        assert(context);
        assert(name);
        assert(counter < CLI_COLLECTION_COUNT);

        this = context->registry->collection + counter;

        this->name = name;
}

void
cli_register_action(
        struct CliContext context[1],
        const u8          name[],
        const CliCallbackFunction callback
) {
        umax counter = ++context->registry->counter->action;

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
        const enum CliResourceType   type,
        const union CliResourceValue value
) {
        umax counter = ++context->registry->counter->resource;

        struct CliResource *this;

        assert(context);
        assert(name);
        assert(type >= 0);
        assert(type < CLI_RESOURCE_IDENT_COUNT);
        assert(counter < CLI_RESOURCE_COUNT);

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
        assert(argv);

        context->args->args = argv;
        assert(argc >= 0);
        context->args->count = (u32)argc;
        context->args->index = 1; /* Skip path to executable */
}

void
cli_register_verify(
        const struct CliContext context[1]
) {
        umax index;

        assert(context->registry->counter->action + 1
                        == CLI_ACTION_COUNT);
        assert(context->registry->counter->collection + 1
                        == CLI_COLLECTION_COUNT);
        assert(context->registry->counter->resource + 1
                        == CLI_RESOURCE_COUNT);
        assert(context->registry->counter->toggle + 1
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

static void
cli_verify_action_tag(
        const struct CliRegistry registry[1],
        const u8               name        [] ,
        u8                     rc          [1]
) {
        const struct CliAction *actions = registry->action;
        umax action_index_max = registry->counter->action;
        umax index;

        *rc = 0;

        for(    index = 0;
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
        umax collection_index_max = registry->counter->collection;
        umax index;

        *rc = 0;

        for (   index = 0;
                index <= collection_index_max;
                ++index
        ) {
                *rc = streq(collections[index].name, name + 2) &&
                        name[0] == '-' &&
                        name[1] == '-';
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
        umax toggle_index_max = registry->counter->toggle;
        umax index;

        *rc = 0;

        for(    index = 0;
                index <= toggle_index_max;
                ++index
        ) {
                *rc = streq(toggles[index].name, name + 2);
                if (*rc) { return; }
        }
}

#if 0
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
        if(*rc) return;

        cli_verify_collection_tag(
                registry,
                tag,
                rc);
        if(*rc) return;

        cli_verify_toggle_tag(
                registry,
                tag,
                rc);
}
#endif

static void
cli_parse_collection(
        struct CliOwnableContext context  [1],
        struct CliArgData        arg      [1],
        u8                       rc       [1]
) {
        u8 index, start;
        const u8 *tag;
        struct CliCollection *this;
        enum status status[1];

        *rc = 0;

        assert(context);
        assert(arg);
        assert(arg->args);

        tag = arg->args[arg->index];
        assert(tag);

        /* Ensure collection tag really starts with "--" */
        assert('-' == tag[0] && '-' == tag[1]);

        /* Look for index of this collection in this context by name. */
        for(    index = 0;
                index < context->counter->collection;
                ++index
        ) {
                if(streq(context->collection[index].name, tag + 2)
                ) {
                        goto found;
                }
        }

        assert(context->counter->collection == index);

        /* If every slot taken, collection tag not found. */
        if(     CLI_COLLECTION_COUNT == index
        ) {
                *rc = 1;
                return;
        }

        /* We need to register the new collection before continuing. */
        assert(context->counter->collection < CLI_COLLECTION_COUNT);
        context->collection[index].name     = tag + 2;
        context->collection[index].length   = 0;
        for(start = 0; start < CLI_COLLECTION_MAX_STARTS; ++start)
        context->collection[index].start[start] = 0;
        ++context->counter->collection;

found:
        this = context->collection + index;

        write(STDOUT, this->name, strlen(this->name), status);
        write(STDOUT, tag+2, strlen(tag+2), status);
        assert(streq(this->name, tag+2));

        ++arg->index; /* Consume name of collection. */
        if (arg->index >= arg->count) {
                *rc = 1;
                return;
        }

        assert(arg->index <= U32_MAX);

        /* Look for the next available starting slot. */
        for(    start=0;
                start < CLI_COLLECTION_MAX_STARTS &&
                0 != this->start[start];
                ++start
        );

        if(start == CLI_COLLECTION_MAX_STARTS && 0 != this->start[start]) {
                assert(0 && "Too many collections of this type.");
        }

        assert(arg->index <= U16_MAX);
        this->start[start] = (u16) arg->index;

        while (arg->index < arg->count &&
        arg->args[arg->index][0] != '-') {
                ++this->length;
                ++arg->index;
        }

        return;
}

/* This assumes you are actually parsing a real, correct toggle! */
static void
cli_parse_toggle(
        struct CliOwnableContext *context,
        struct CliArgData        *arg,
        u8                        rc[1]
) {
        u8 index;
        enum status status[1];

        assert(context);

        *rc = 0;

        for(    index = 0;
                index < context->counter->toggle;
                ++index
        ) {
                if(streq(context->toggle[index].name, arg->args[arg->index])
                ) {
                        context->toggle[index].value=1;
                        ++arg->index;
                        return;
                }
        }

        /* If every slot taken, toggle not found. */
        if(CLI_TOGGLE_COUNT == context->counter->toggle
        ) {
                *rc = 1;
                ++arg->index;
                return;
        }

        write(STDOUT, (u8*)"TAST\n", 5, status);
        index = context->counter->toggle;
        context->toggle[index].name = arg->args[arg->index];
        context->toggle[index].value = 1;
        ++context->counter->toggle;
        ++arg->index;

        return;

}

static void
cli_parse_ownable_state(
        struct CliRegistry       registry[1],
        struct CliOwnableContext context[1],
        struct CliArgData        arg_data   [1],
        u8                       rc[1]
) {
        
        u32 *index = &arg_data->index;
        const u32 count = arg_data->count;
        const u8 **args = arg_data->args;
        
        assert(registry);
        assert(context);
        assert(arg_data);

        *rc = 0;

        while(  *index < count &&
                !*rc
        ) {
                cli_verify_collection_tag(registry, args[*index], rc);
                if(*rc) {
                        cli_parse_collection(
                                context,
                                arg_data,
                                rc
                        );
                        continue;
                }

                cli_verify_toggle_tag(registry, args[*index], rc);
                if(*rc) {
                        cli_parse_toggle(context, arg_data, rc);
                        continue;
                }

                cli_verify_action_tag(registry, args[*index], rc);
                ++(*index);
        }
}
static void
cli_print_collection(
        const struct CliCollection collection[1],
        const struct CliArgData arg_data[1]
){
        u16 s, o;
        enum status status[1];

        write(STDOUT, (u8*)"\n\t", 2, status);
        write(STDOUT, collection->name, strlen(collection->name), status);
        write(STDOUT, (u8*)":\n", 2, status);


        for(    s = 0;
                collection->start[s] != 0 && s < CLI_COLLECTION_MAX_STARTS;
                ++s
        ) {
                u32 s_offs = collection->start[s];
                for(    o = 0;
                        arg_data->args[s_offs+o][0]!='-';
                        ++o
                ) {
                        const u8 *buf = arg_data->args[s_offs+o];
                        write(STDOUT, (u8*)"\t\t", 2, status);
                        write(STDOUT, buf, strlen(buf), status);
                        write(STDOUT, (u8*)"\n", 1, status);
                }
        }


}
static void
cli_print_collections(
        const struct CliCollection *collections ,
        const umax                  collection_count,
        const struct CliArgData     arg_data[1]
) {
        u16 i = 0;
        enum status status[1];
        {       u8 buffer[32];
                write(STDOUT, (u8*)"Collections parsed: ", 20, status);
                assert(collection_count < SMAX_MAX);
                itoa((smax)collection_count, buffer);
                write(STDOUT, buffer, strlen(buffer), status);
                write(STDOUT, (u8*)"\n", 1, status);
        }
        

        write(STDOUT, (u8*)"Collections:", 13, status);
        for(    i=0;
                i < collection_count;
                ++i
        ) {
                cli_print_collection(collections + i, arg_data);
        }
        write(STDOUT, (u8*)"\n", 1, status);
}

static void
cli_print_toggle(
        const struct CliToggle toggle[1]
) {
        enum status status[1];
        write(STDOUT, (u8*)"\t", 1, status);
        write(STDOUT, toggle->name, strlen(toggle->name), status);
        write(STDOUT, (u8*)": ", 2, status);
        if(toggle->value) {
                write(STDOUT, (u8*)"true\n", 6, status);
        } else {
                write(STDOUT, (u8*)"false\n", 7, status);
        }
}

static void
cli_print_toggles(
        const struct CliToggle *toggles,
        const umax              toggle_count
) {
        u16 i = 0;
        enum status status[1];
        write(STDOUT, (u8*)"Toggles:\n", 9, status);
        for(    i=0;
                i < toggle_count;
                ++i
        ) {
                cli_print_toggle(toggles + i);
        }
        write(STDOUT, (u8*)"\n", 1, status);
}

void
cli_parse_init(
        struct CliContext context[1],
        u8                rc     [1]
) {
        enum status status[1];
        assert(context);
        assert(rc);

        cli_parse_ownable_state(
                context->registry,
                context->common_context,
                context->args,
                rc
        );
        if(!*rc) {
                write(STDOUT, (u8*) "INFO: Parsed ownable state\n", 27,
                        status);
        } else {
                write(STDERR, (u8*) "ERROR: Failed to parse ownable state\n",
                        37, status);
        }

        cli_print_collections(context->common_context->collection, context->common_context->counter->collection, context->args);
        cli_print_toggles(context->common_context->toggle, context->common_context->counter->toggle);
}

void
cli_parse_callback(
        struct CliContext context[1],
        u8                rc     [1]
) {
        /* TODO TOMORROW! */
        (void) context; (void) rc;
}

void
cli_execute_callback(
        struct CliContext context[1],
        u8                rc     [1]
) {
        (void) context; (void) rc;
}

struct CliCollectionIterator*
cli_collection_begin(
        struct CliCollectionIterator it[1],
        const struct CliCallbackContext ctx[1],
        const u8 *name
) {
        u8 idx;

        assert(it);
        assert(ctx);
        assert(ctx->owned->counter->collection);

        it->chunk = 0;
        it->offset = 0;
        it->ctx = ctx;
        it->collection = 0;

        for(idx=0;idx<ctx->owned->counter->collection;++idx) {
                const struct CliCollection *collection =
                        ctx->owned->collection + idx;
                assert(collection->name);
                if(streq(collection->name, name)) {
                        it->collection = &ctx->owned->collection[idx];
                        break;
                }
        }

        assert(it->collection);

        return it;
}

const u8*
cli_collection_next(
        struct CliCollectionIterator it[1]
) {

        u16 index = it->collection->start[it->chunk] + it->offset;
        const u8 *result;

        if(index >= it->ctx->arg->count) {
                assert(0 && "TEMP");
                return 0;
        }

        if(it->ctx->arg->args[index][0]=='-') {
                u16 temp_index;
                it->offset = 0;
                ++it->chunk;
                
                temp_index = it->collection->start[it->chunk] + it->offset;

                if(temp_index <= index) {
                        assert(0 && "TEMP");
                        return 0;
                }

                index = temp_index;

                assert(index < it->ctx->arg->count);
                assert(it->ctx->arg->args[index][0]!='-');
        }

        result = it->ctx->arg->args[index];

        assert(result);

        ++it->offset;

        return result;

}

void
cli_collection_reset(
        struct CliCollectionIterator it[1]
) {
        (void)it;
}

