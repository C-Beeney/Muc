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
        #ifndef CLI_COLLECTION_MAX_CHILDREN
                #define CLI_COLLECTION_MAX_CHILDREN -1
        #endif/*CLI_COLLECTION_MAX_CHILDREN*/
        #ifndef CLI_TOGGLE_COUNT
                #define CLI_TOGGLE_COUNT -1
        #endif/*CLI_TOGGLE_COUNT*/
        #ifndef CLI_GENERIC_TYPE_IDENTIFIERS
                #define CLI_GENERIC_TYPE_IDENTIFIERS \
                        resource_types_must_be_defined
        #endif/*GENERIC_TYPES*/
        #ifndef CLI_GENERIC_VALUE_TYPES
                #define CLI_GENERIC_VALUE_TYPES \
                        resource_values_must_be_defined
        #endif/*GENERIC_VALUES*/
        #ifndef CLI_RESOURCE_COUNT
                #define CLI_RESOURCE_COUNT -1
        #endif/*CLI_RESOURCE_COUNT*/
        #ifndef CLI_ACTION_COUNT
                #define CLI_ACTION_COUNT -1
        #endif/*CLI_ACTION_COUNT*/
        #ifndef CLI_MAX_ACTIONS
                #define CLI_MAX_ACTIONS -1
        #endif/*CLI_MAX_ACTIONS*/

        #ifndef NO_STATIC_CLI_ASSERTIONS
                typedef char __collections_count_is_defined
                        [CLI_COLLECTION_COUNT        != -1 ?1 :-1];
                typedef char __collections_max_children_is_defined_assertion
                        [CLI_COLLECTION_MAX_CHILDREN != -1 ?1 :-1];
                typedef char __toggles_count_is_defined
                        [CLI_TOGGLE_COUNT            != -1 ?1 :-1];
                typedef char __resources_count_is_defined
                        [CLI_RESOURCE_COUNT          != -1 ?1 :-1];
                typedef char __actions_count_is_defined
                        [CLI_ACTION_COUNT           != -1 ?1 :-1];
        #endif/*NO_STATIC_CLI_ASSERTIONS*/

/*--------------------------------------------------------------------------*/
/* Structure definitions                                                    */
/*--------------------------------------------------------------------------*/

        union CliGenericValue {void *_; CLI_GENERIC_VALUE_TYPES};
        enum  CliGenericType  {CLI_GENERIC_TYPE_IDENTIFIERS, CLI_GENERIC_MAX};

        struct CliGeneric {
                union CliGenericValue value;
                enum CliGenericType   type;
                PAD32
        };

        struct CliResource {
                const u8         *name;
                struct CliGeneric value[1];
        };

        struct CliCollection {
                const u8 *name;
                const u8 *value[CLI_COLLECTION_MAX_CHILDREN];
        };

        struct CliToggle {
                const u8 *name;
                u8        value;

                PAD8 PAD16 PAD32
        };

        struct CliCallbackContext {
                const struct CliCollection collection[CLI_COLLECTION_COUNT];
                const struct CliToggle     toggle[CLI_TOGGLE_COUNT];
                const struct CliResource   resource[CLI_RESOURCE_COUNT];
        };

        typedef void (*CliCallback)(
                const struct CliCallbackContext context[1],
                u8                              rc[1]
        );

        struct CliAction {
                const u8   *name;
                CliCallback callback;
        };

        struct CliContextualisedCallback {
                struct CliCallbackContext context[1];
                CliCallback               callback;
        };

        struct CliRegistryCounters {
                umax action;
                umax collection;
                umax toggle;
                umax resource;
        };

        struct CliRegistry {
                struct CliAction           action     [CLI_ACTION_COUNT];
                struct CliCollection       collection [CLI_COLLECTION_COUNT];
                struct CliToggle           toggle     [CLI_TOGGLE_COUNT];
                struct CliResource         resource   [CLI_RESOURCE_COUNT];
                struct CliRegistryCounters counters   [1];
        };

        struct CliContext {
                struct CliRegistry               registry[1];
                struct CliContextualisedCallback callback[CLI_MAX_ACTIONS];
        };

/*--------------------------------------------------------------------------*/
/* Function declarations                                                    */
/*--------------------------------------------------------------------------*/

        void cli_init_context(
                struct CliContext context[1]
        );

        void cli_register_verify(
                const struct CliContext context[1]
        );

        void cli_register_toggle(
                struct CliContext context[1],
                const u8          name[],
                const u8          value
        );

        void cli_register_collection(
                struct CliContext context[1],
                const u8          name[],
                const u8         *value[CLI_COLLECTION_MAX_CHILDREN]
        );

        void cli_register_action(
                struct CliContext context[1],
                const u8          name[],
                const CliCallback callback
        );

        void cli_register_resource(
                struct CliContext     context[1],
                const u8              name[],
                const enum CliGenericType   type,
                const union CliGenericValue value
        );

        void cli_parse_args(
                struct CliContext context[1],
                const i32 argc,
                u8 *argv[]
        );

        #if !defined(NO_CLI_STRUCT_SIZE_ASSERTS) && defined(ARCH_64)
                typedef char __cli_generic_expected_size
                        [16  == sizeof(
                                struct CliGeneric
                        )?1:-1];
                typedef char __cli_resource_expected_size
                        [24 == sizeof(
                                struct CliResource
                        )?1:-1];
                typedef char __cli_collection_expected_size
                        [40 == sizeof(
                                struct CliCollection
                        )?1:-1];
                typedef char __cli_toggle_expected_size
                        [16 == sizeof(
                                struct CliToggle
                        )?1:-1];
                typedef char __cli_callback_context_expected_size
                        [120 == sizeof(
                                struct CliCallbackContext
                        )?1:-1];
                typedef char __cli_action_expected_size
                        [16 == sizeof(
                                struct CliAction
                        )?1:-1];
                typedef char __cli_contextualised_callback_expected_size
                        [128 == sizeof(
                                struct CliContextualisedCallback
                        )?1:-1];
                typedef char __cli_registry_counters_expected_size
                        [0xFF == sizeof(
                                struct CliRegistryCounters
                        )?1:-1];
                typedef char __cli_registry_expected_size
                        [136 == sizeof(
                                struct CliRegistry
                        )?1:-1];
                typedef char __cli_context_expected_size
                        [264 == sizeof(
                                struct CliContext
                        )?1:-1];

        #elif !defined(NO_CLI_STRUCT_SIZE_ASSERTS) && defined(ARCH_32)
                        typedef char no_expected_sizes[-1];
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
                cli_action_get_collection(context, "--", #name)

        #define CLI_ACTION_CONTEXT_UNUSED (void) context
        #define CLI_ACTION_SET_EXIT_CODE(ec) *rc=ec
#endif/*ACLI_H*/
