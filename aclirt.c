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

/**
 * @brief Initialise collection iterator.
 * 
 * Prepare iterator for iteration over a named CLI collection.
 * 
 * @param it        [out] Iterator to initialize
 * @param ctx       [in]  Callback context
 * @param name      [in]  Collection name (e.g., "with", "as")
 * 
 * @return OK on success, or COLLECTION_NOT_FOUND on failure.
 * 
 * @note Buffers from cli_collection_next() are static. Not thread-safe.
 * 
 * @see cli_collection_next, cli_collection_reset, cli_collection_change
 */
enum status
cli_collection_begin(
        struct CliCollectionIterator it[1],
        const struct CliCallbackContext ctx[1],
        const u8 *name
) {
        u8 idx;

        assert(it);
        assert(ctx);
        assert(ctx->owned);
        assert(ctx->owned->counter);
        assert(ctx->owned->counter->collection);

        assert(name);

        assert(ctx->owned->collection);

        it->chunk = 0;
        it->offset = 0;
        it->ctx = ctx;
        it->collection = null;

        for(idx=0;idx<ctx->owned->counter->collection;++idx) {
                const struct CliCollection *collection =
                        ctx->owned->collection + idx;

                assert(collection->name);

                if(streq(collection->name, name)) {
                        it->collection = collection;
                        break;
                }
        }

        if(idx == ctx->owned->counter->collection) {
                return COLLECTION_NOT_FOUND;
        }

        assert(null != it->collection);
        assert(null != it->collection->name);

        return OK;
}

const u8*
cli_collection_next(
        struct CliCollectionIterator it[1]
) {
        u32 index;

        assert(it);
        assert(it->collection);
        assert(it->collection->start);
        assert(it->chunk < it->collection->starts);
        assert(it->collection->start[it->chunk]);

        assert(it->collection->start[it->chunk] <= U32_MAX - it->offset);
        index = it->collection->start[it->chunk] + it->offset;

        assert(it->ctx);
        assert(it->ctx->arg);
        assert(index <= it->ctx->arg->count);
        assert(it->ctx->arg->args);
        
        #ifndef NDEBUG
                if(index != it->ctx->arg->count) {
                        assert(it->ctx->arg->args[index]);
                        assert(it->ctx->arg->args[index][0]);
                }
        #endif/*NDEBUG*/
        
        if(index == it->ctx->arg->count ||
                it->ctx->arg->args[index][0]=='-') {

                it->offset = 0;
                ++it->chunk;

                if(it->chunk == it->collection->starts) {
                        return null;
                }

                index = it->collection->start[it->chunk];
                assert(index < it->ctx->arg->count);
        }
        
        assert(it->offset < U16_MAX);
        ++it->offset;

        #ifndef NDEBUG
                if(index != it->ctx->arg->count) {
                        assert(it->ctx->arg->args[index]);
                        assert(it->ctx->arg->args[index][0]);
                }
        #endif/*NDEBUG*/

        assert(it->ctx->arg->args[index][0]!='-');

        return it->ctx->arg->args[index];
}

void
cli_collection_reset(
        struct CliCollectionIterator it[1]
) {
        assert(it);
        it->chunk = 0;
        it->offset = 0;
}

enum status
cli_collection_change(
        struct CliCollectionIterator *it,
        const u8 *name
) {
        enum status status;

        assert(it);
        assert(it->ctx);
        assert(name);

        status = cli_collection_begin(it, it->ctx, name);
        return status;
}

u8
cli_toggle_get(
        const struct CliCallbackContext ctx[1],
        const u8 *name
) {
        umax idx;

        assert(ctx);
        assert(name);

        assert(ctx->owned);
        assert(ctx->owned->counter);
        assert(ctx->owned->toggle);

        for(idx = 0; idx < ctx->owned->counter->toggle; ++idx) {
                assert(null != ctx->owned->toggle + idx);
                assert(null != ctx->owned->toggle[idx].name);

                if(streq(ctx->owned->toggle[idx].name, name)) {
                        assert(ctx->owned->toggle[idx].value == 0 ||
                                ctx->owned->toggle[idx].value == 1);
                        return ctx->owned->toggle[idx].value;
                }
        }

        assert(0 && "Toggle name not found.");
        return 0;
}

void
cli_execute_callback(
        struct CliCallback callback[1],
        u8                 rc     [1]
) {
        callback->callback(callback->context, rc);
}
