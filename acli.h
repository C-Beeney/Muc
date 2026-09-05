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

#ifndef ACLI_H
        #define ACLI_H

        #include <stypes.h>
        #include <uassert.h>
        #include <ustring.h>

        #ifndef CLI_CONFIG_H
                #define CLI_CONFIG_H
                #include <ccli.h>
        #endif/*CLI_CONFIG_H*/

/*--------------------------------------------------------------------------*/
/* Configuration macro safety checks                                        */
/*--------------------------------------------------------------------------*/

        #ifndef CLI_COLLECTION_COUNT
                #define CLI_COLLECTION_COUNT -1
        #endif/*CLI_COLLECTION_COUNT*/
        #ifndef CLI_COLLECTION_MAX_STARTS
                #define CLI_COLLECTION_MAX_STARTS 3
        #endif/*CLI_COLLECTION_MAX_STARTS*/
        #ifndef CLI_TOGGLE_COUNT
                #define CLI_TOGGLE_COUNT -1
        #endif/*CLI_TOGGLE_COUNT*/
        #ifndef CLI_RESOURCE_IDENTS
                #define CLI_RESOURCE_IDENTS resource_idents_undefined
        #endif/*CLI_RESOURCE_IDENTS*/
        #ifndef CLI_RESOURCE_TYPES
                #define CLI_RESOURCE_TYPES resource_types_undefined
        #endif/*CLI_RESOURCE_TYPES*/
        #ifndef CLI_RESOURCE_COUNT
                #define CLI_RESOURCE_COUNT -1
        #endif/*CLI_RESOURCE_COUNT*/
        #ifndef CLI_ACTION_COUNT
                #define CLI_ACTION_COUNT -1
        #endif/*CLI_ACTION_COUNT*/
        #ifndef CLI_MAX_ACTIONS
                #define CLI_MAX_ACTIONS 4
        #endif/*CLI_MAX_ACTIONS*/

        #ifndef NO_STATIC_CLI_ASSERTIONS
                typedef char __collections_count_is_defined
                        [CLI_COLLECTION_COUNT != -1 ?1 :-1];
                typedef char __toggles_count_is_defined
                        [CLI_TOGGLE_COUNT     != -1 ?1 :-1];
                typedef char __resources_count_is_defined
                        [CLI_RESOURCE_COUNT   != -1 ?1 :-1];
                typedef char __actions_count_is_defined
                        [CLI_ACTION_COUNT     != -1 ?1 :-1];
                typedef char __macros_count_is_defined
                        [CLI_MACRO_COUNT      != -1 ?1 :-1];
                typedef char __aliases_count_is_defined
                        [CLI_ALIAS_COUNT      != -1 ?1 :-1];
        #endif/*NO_STATIC_CLI_ASSERTIONS*/

/*--------------------------------------------------------------------------*/
/* Structure definitions                                                    */
/*--------------------------------------------------------------------------*/

        union CliResourceValue {
                CLI_RESOURCE_TYPES
                PAD(2,4,8)
        };

        enum  CliResourceType  {
                CLI_RESOURCE_IDENTS,
                CLI_RESOURCE_IDENT_COUNT
        };

        struct CliResourceBinding {
                union CliResourceValue value;
                enum CliResourceType   type;
                PAD(    (8-sizeof(enum CliResourceType))%2,
                        (8-sizeof(enum CliResourceType))%4,
                        (8-sizeof(enum CliResourceType))%8)
        };

        struct CliResource {
                const u8                 *name;
                struct CliResourceBinding value[1];
        };

        struct CliCollection {
                const u8 *name;

                /* These variables are intended for use with argv's data.
                 * start signifies the index of the first argv value in the
                 * collection. Each following start represents just that,
                 * continue parsing the collection from that point. A segment
                 * ends when it typically would syntactically. length signifies
                 * the number of argv values in the collection.
                 * */
                u16 start[CLI_COLLECTION_MAX_STARTS]
                ,   length;
        };

        struct CliCollectionIterator {
                const struct CliCallbackContext *ctx;
                const struct CliCollection      *collection;
                u16                              offset;
                u8                               chunk;
                PAD(0,0,5)
        };

        struct CliToggle {
                const u8 *name;
                u8        value;

                PAD(1, 3, 7)
        };

        struct CliOwnableContextCounters {
                u8 collection, toggle;

                PAD(0,2,6)
        };

        struct CliOwnableContext {
                struct CliCollection collection [CLI_COLLECTION_COUNT];
                struct CliToggle     toggle     [CLI_TOGGLE_COUNT];
                struct CliOwnableContextCounters counter[1];
        };

        struct CliCallbackContext {
                struct CliOwnableContext   owned[1];
                const struct CliResource **resource;
                const struct CliArgData   *arg;
        };

        typedef void (*CliCallbackFunction)(
                const struct CliCallbackContext context [1],
                u8                              rc      [1]
        );

        struct CliAction {
                const u8           *name;
                CliCallbackFunction callback;
        };

        /* Example macro: [name = install] [sequence = 'dicp'] [expanded = "-dicp"] [expanded = "--download --install --clear-cache --with-package-names"]*/
        struct CliMacro {
                const u8 *name;
                const u8 *shortcuts;
        };

        /* Example alias: [name = ins] [expanded = "install"] */
        /* Example alias: [name = -d] [expanded = "--download"] */
        /* Example full expansion with macro: [n] -> --new-muc --compression best --crc 32 --props posix+ --format latest --as */
        struct CliAlias {
                const u8 *name;
                const u8 *expanded;
        };

        struct CliCallback {
                struct CliCallbackContext context[1];
                CliCallbackFunction       callback;
        };

        struct CliCallbackIterator {
                s32 temp;
                PAD(0,0,4)
        };

        struct CliRegistryCounters {
                u8 action, collection, toggle, resource, macro, alias;

                PAD(0, 2, 2)
        };

        struct CliRegistry {
                struct CliAction            action [CLI_ACTION_COUNT];
                struct CliCollection    collection [CLI_COLLECTION_COUNT];
                struct CliToggle            toggle [CLI_TOGGLE_COUNT];
                struct CliResource        resource [CLI_RESOURCE_COUNT];
                struct CliMacro              macro [CLI_MACRO_COUNT];
                struct CliAlias              alias [CLI_ALIAS_COUNT];
                struct CliRegistryCounters counter [1];
        };

        struct CliArgData {
                const u8 **args;
                u32        count;
                u32        index;
        };

        struct CliContext {
                struct CliRegistry       registry       [1];
                struct CliOwnableContext common_context [1];
                struct CliArgData        args           [1];
        };

/*--------------------------------------------------------------------------*/
/* Function declarations                                                    */
/*--------------------------------------------------------------------------*/

        void
        cli_init_context(
                struct CliContext context[1]
        );

        void
        cli_register_verify(
                const struct CliContext context[1]
        );
\
        void
        cli_register_toggle(
                struct CliContext context[1],
                const u8          name    []
        );

        void
        cli_register_collection(
                struct CliContext context[1],
                const u8          name    []
        );

        void
        cli_register_action(
                struct CliContext context[1],
                const u8          name    [],
                const CliCallbackFunction callback
        );

        void
        cli_register_resource(
                struct CliContext     context[1],
                const u8              name    [],
                const enum CliResourceType   type,
                const union CliResourceValue value
        );

        void
        cli_register_macro(
                struct CliContext context [1],
                const u8          name     [],
                const u8          shortcuts[]
        );

        void
        cli_register_alias(
                struct CliContext context[1],
                const u8          name    [],
                const u8          expanded[]
        );

        void
        cli_register_args(
                struct CliContext context[1],
                const u8         *argv    [],
                const s32         argc
        );

        void
        cli_parse_init(
                struct CliContext context[1],
                u8                rc     [1]
        );
        
        void
        cli_parse_callback(
                struct CliContext context[1],
                u8                rc     [1]
        );

        void
        cli_execute_callback(
                struct CliContext context[1],
                u8                rc     [1]
        );

        void
        cli_reset_parse(
                struct CliContext context[1]
        );

        struct CliCollectionIterator*
        cli_collection_begin(
                struct CliCollectionIterator    it[1],
                const struct CliCallbackContext ctx[1],
                const u8                       *name
        );

        const u8*
        cli_collection_next(
                struct CliCollectionIterator    *it
        );

        void
        cli_collection_reset(
                struct CliCollectionIterator *it
        );

        /* void cli_execute(); not yet */

        #if !defined(NO_CLI_STRUCT_SIZE_ASSERTS) && defined(ARCH_64)
                typedef char __cli_generic_expected_size
                        [16  == sizeof(struct CliResourceBinding
                        )?1:-1];
                typedef char __cli_resource_expected_size
                        [24 == sizeof(struct CliResource
                        )?1:-1];
                typedef char __cli_collection_expected_size
                        [16 == sizeof(struct CliCollection
                        )?1:-1];
                typedef char __cli_collection_iterator_expected_size
                        [24 == sizeof(struct CliCollectionIterator
                        )?1:-1];
                typedef char __cli_toggle_expected_size
                        [16 == sizeof(struct CliToggle
                        )?1:-1];
                typedef char __cli_ownable_context_counters_expected_size
                        [8 == sizeof(struct CliOwnableContextCounters
                        )?1:-1];
                typedef char __cli_ownable_context_expected_size
                        [56 == sizeof(struct CliOwnableContext
                        )?1:-1];
                typedef char __cli_callback_context_expected_size
                        [72 == sizeof(struct CliCallbackContext
                        )?1:-1];
                typedef char __cli_action_expected_size
                        [16 == sizeof(struct CliAction
                        )?1:-1];
                typedef char __cli_macro_expected_size
                        [16 == sizeof(struct CliMacro
                        )?1:-1];
                typedef char __cli_alias_expected_size
                        [16 == sizeof(struct CliAlias
                        )?1:-1];
                typedef char __cli_callback_expected_size
                        [80 == sizeof(struct CliCallback
                        )?1:-1];
                typedef char __cli_callback_iterator_expected_size
                        [8 == sizeof(struct CliCallbackIterator
                        )?1:-1];
                typedef char __cli_registry_counters_expected_size
                        [8 ==sizeof(struct CliRegistryCounters
                        )?1:-1];
                typedef char __cli_registry_expected_size
                        [128 == sizeof(struct CliRegistry
                        )?1:-1];
                typedef char __cli_arg_data_expected_size
                        [16 == sizeof(struct CliArgData
                        )?1:-1];
                typedef char __cli_context_expected_size
                        [200 == sizeof(struct CliContext
                        )?1:-1];
        #elif !defined(NO_CLI_STRUCT_SIZE_ASSERTS) && defined(ARCH_16)
                        typedef char no_expected_sizes[-1];
        #endif/*Size checks*/

        #define CLI_REGISTERABLE_ACTION(name) \
                void name(const struct CliCallbackContext context[1], u8 rc[1])
        #define CLI_ACTION_GET_RESOURCE(name) \
                cli_action_get_resource(context, "--" #name)
        #define CLI_ACTION_GET_TOGGLE(name) \
                cli_action_get_toggle(context, "--" #name)
        #define CLI_ACTION_GET_COLLECTION(name) \
                cli_action_get_collection(context, "--" #name)

        #define CLI_ACTION_CONTEXT_UNUSED (void) context
        #define CLI_ACTION_SET_EXIT_CODE(ec) *rc=ec
#endif/*ACLI_H*/
