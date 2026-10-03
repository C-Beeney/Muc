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
#include <ssys.h>
#include <uconio.h>

static void
cli_parse_collection(
        struct CliOwnableContext context  [1],
        struct CliArgData        arg      [1],
        u8                       rc       [1]
) {
        u8 index;
        const u8 *tag;
        struct CliCollection *this;

        *rc = OK;

        assert(context);
        assert(arg);
        assert(rc);
        assert(arg->args);

        assert(arg->index < arg->count);
        tag = arg->args[arg->index];
        assert(tag);

        /* Ensure collection tag really starts with "--" */
        printdbg(b"Buffer: ");
        #ifndef NDEBUG
        println(tag);
        #endif
        assert('-' == tag[0] && '-' == tag[1]);

        printdbg(b"Comparing with tag:\n\t");
        printdbgln(tag);
        /* Look for index of this collection in this context by name. */
        for(    index = 0;
                index < context->counter->collection;
                ++index
        ) {
                if(streq(context->collection[index].name, tag + 2)
                ) {
                        printdbgln(b"Collection found.");
                        goto found;
                }
        }

        assert(context->counter->collection == index);

        /* If every slot taken, collection tag not found. */
        if(     CLI_COLLECTION_COUNT <= index
        ) {
                printdbgln(b"Every slot taken, collection not found.");
                *rc = GENERIC_ERR;
                return;
        }

        /* We need to register the new collection before continuing. */
        assert(context->counter->collection <= CLI_COLLECTION_COUNT);
        context->collection[index].name     = tag + 2;
        context->collection[index].starts = 0;
        ++context->counter->collection;

found:
        this = context->collection + index;
        assert(streq(this->name, tag+2));

        ++arg->index; /* Consume name of collection. */
        if (arg->index >= arg->count) {
                printdbgln(b"Passed end of arguments.");
                *rc = GENERIC_ERR;
                return;
        }

        assert(arg->index <= U32_MAX);

        if(this->starts == CLI_COLLECTION_MAX_STARTS) {
                assert(0 && "Too many collections of this type.");

                *rc = GENERIC_ERR;
                return;
        }

        assert(arg->index <= U16_MAX);
        this->start[this->starts] = (u16) arg->index;
        ++this->starts;

        while (arg->index < arg->count && arg->args[arg->index][0] != '-') {
                ++arg->index;
        }

        assert(*rc == OK);
}

/* This assumes you are actually parsing a real, correct toggle! */
static void
cli_parse_toggle(
        struct CliOwnableContext *context,
        struct CliArgData        *arg,
        u8                        rc[1]
) {
        u8 index;

        assert(context);
        assert(context->counter->toggle <= CLI_TOGGLE_COUNT);
        assert(context->toggle);

        *rc = 0;

        for(    index = 0;
                index < context->counter->toggle;
                ++index
        ) {
                assert(index <= CLI_TOGGLE_COUNT);
                assert(context->toggle[index].name);
                assert(arg->index < arg->count);
                assert(arg->args[arg->index]);
                if(streq(context->toggle[index].name, arg->args[arg->index])
                ) {
                        context->toggle[index].value=1;
                        ++arg->index;
                        return;
                }
        }

        /* If every slot taken, toggle not found. */
        if(CLI_TOGGLE_COUNT <= context->counter->toggle
        ) {
                *rc = 1;
                ++arg->index;
                return;
        }

        index = context->counter->toggle;
        context->toggle[index].name = arg->args[arg->index] + 2;
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
                        printdbgln(b"Parsing Collection...");
                        cli_parse_collection(
                                context,
                                arg_data,
                                rc
                        );
                        continue;
                }

                cli_verify_toggle_tag(registry, args[*index], rc);
                if(*rc) {
                        printdbgln(b"Parsing Toggle...");
                        cli_parse_toggle(context, arg_data, rc);
                        continue;
                }

                cli_verify_action_tag(registry, args[*index], rc);
                if(*rc) {
                        printdbgln(b"Action Found...");
                        *rc = 0;
                        break;
                }
                ++(*index);
        }
}


void
cli_parse_init(
        struct CliContext context[1],
        u8                rc     [1]
) {
        assert(context);
        assert(rc);

        cli_parse_ownable_state(
                context->registry,
                context->common_context,
                context->args,
                rc
        );
        if(!*rc) {
                printdbgln(b"Parsed ownable state.");
        } else {
                printerrln(b"Failed to parse ownable state.");
        }
}


/* TODO: INDENT COMPLIANCE AND THIS FUNCTION IS DISGRACEFUL */
void
cli_parse_callback(
        struct CliContext  context [1],
        struct CliCallback callback[1],
        u8                 rc      [1]
) {
        umax idx;
        const u8 *arg;

        assert(null != context);
        assert(null != callback);
        assert(null != rc);
        assert(null != context->args);
        assert(null != context->args->args);
        assert(null != context->registry);
        assert(null != context->registry->action);
        assert(null != context->registry->counter);
        assert(context->args->index <= context->args->count);
        
        arg = context->args->args[context->args->index];
        assert(null != arg);
        ++context->args->index;
        *rc = OK;

        assert(CLI_ACTION_COUNT == context->registry->counter->action);
        assert(arg[0] == '-' && arg[1] == '-');

        for(idx = 0; idx < CLI_ACTION_COUNT; ++idx) {
                assert(null != context->registry->action[idx].name);
                if(streq(arg+2, context->registry->action[idx].name)) {
                        goto found;
                }
        }

        *rc = GENERIC_ERR;
        return;

found:

        assert(idx <= CLI_ACTION_COUNT);
        assert(null != context->registry->action[idx].callback);
        callback->callback = context->registry->action[idx].callback;
        assert(null != callback->context);
        assert(null != callback->context->owned);
        cli_parse_ownable_state(
                context->registry,
                callback->context->owned,
                context->args,
                rc
        );

        /* SPLICE CALLBACK CONTEXT WITH COMMON CONTEXT */
        for(idx = 0; idx < CLI_COLLECTION_COUNT; ++idx) {
                umax jdx;
                #ifndef NDEBUG
                {
                        u8 _[32];
                        printdbg(b"idx : ");
                        assert(idx < SMAX_MAX);
                        println(itoa((smax)idx, _));
                }
                #endif
                assert(callback->context->owned->collection[idx].name);

                for(jdx = 0; jdx < CLI_COLLECTION_COUNT; ++jdx) {
                        assert(context->common_context->collection[jdx].name);

                        #ifndef NDEBUG
                        print(b"Comparing : ");
                        print(callback->context->owned->collection[idx].name);
                        print(b" - ");
                        println(context->common_context->collection[jdx].name);
                        #endif
                        if(streq(callback->context->owned->collection[idx].name, context->common_context->collection[jdx].name)) {
                                umax kdx;
                                if(callback->context->owned->collection[idx].starts + context->common_context->collection[jdx].starts > CLI_COLLECTION_MAX_STARTS) {
                                        assert(0);
                                        *rc = GENERIC_ERR;
                                        return;

                                        /* Too many starting points to store. */
                                }

                                for(kdx = 0; kdx < context->common_context->collection[jdx].starts; ++kdx) {
                                        printdbgln(b"Iteration");
                                        callback->context->owned->collection[idx].start[callback->context->owned->collection[idx].starts+kdx]=context->common_context->collection[jdx].start[kdx];
                                }

                                callback->context->owned->collection[idx].starts += context->common_context->collection[jdx].starts;
                                #ifndef NDEBUG
                                {
                                        u8 _[32];
                                        print(callback->context->owned->collection[idx].name);
                                        print(b" has ");
                                        print(itoa(callback->context->owned->collection[idx].starts, _));
                                        println(b" starts.");
                                }
                                #endif
                                assert(callback->context->owned->collection[idx].starts <= CLI_COLLECTION_MAX_STARTS);

                        }
                }
        }

        for(idx = 0; idx < CLI_TOGGLE_COUNT; ++idx) {
                umax jdx;
                assert(context->common_context->toggle[idx].name);

                for(jdx = 0; jdx < CLI_TOGGLE_COUNT; ++jdx) {
                        /* Register it if it's not present. */
                        if(null == callback->context->owned->toggle[jdx].name) {
                                callback->context->owned->toggle[jdx].name  = context->common_context->toggle[idx].name;
                                callback->context->owned->toggle[jdx].value = context->common_context->toggle[idx].value;

                                break;
                        }
                        assert(callback->context->owned->toggle[jdx].name);

                        if(streq(callback->context->owned->toggle[jdx].name, context->common_context->toggle[idx].name)) {
                                callback->context->owned->toggle[jdx].value |= context->common_context->toggle[idx].value; 
                        }
                }
        }

        if(*rc != OK) {
                return;
        }

        callback->context->arg = context->args;
        assert(null != context->registry->resource);
        callback->context->resource = context->registry->resource;

}
