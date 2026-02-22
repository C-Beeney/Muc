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

#include <STYPES.H>
#include <UASSERT.H>
#include <USTRING.H>

#ifndef CLI_CONFIG_H
#define CLI_CONFIG_H
#include <CCLI.H>
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
        #define CLI_GENERIC_TYPE_IDENTIFIERS resource_types_must_be_defined
#endif/*GENERIC_TYPES*/
#ifndef CLI_GENERIC_VALUE_TYPES
        #define CLI_GENERIC_VALUE_TYPES resource_values_must_be_defined
#endif/*GENERIC_VALUES*/
#ifndef CLI_RESOURCE_COUNT
        #define CLI_RESOURCE_COUNT -1
#endif/*CLI_RESOURCE_COUNT*/
#ifndef CLI_ACTIONS_COUNT
        #define CLI_ACTIONS_COUNT -1
#endif/*CLI_ACTIONS_COUNT*/
#ifndef CLI_MAX_ACTIONS
        #define CLI_MAX_ACTIONS -1
#endif/*CLI_MAX_ACTIONS*/

#ifndef NO_STATIC_CLI_ASSERTIONS
        typedef char collections_count_is_defined
                [CLI_COLLECTION_COUNT        != -1 ?1 :-1];
        typedef char collections_max_children_is_defined_assertion
                [CLI_COLLECTION_MAX_CHILDREN != -1 ?1 :-1];
        typedef char toggles_count_is_defined
                [CLI_TOGGLE_COUNT            != -1 ?1 :-1];
        typedef char resources_count_is_defined
                [CLI_RESOURCE_COUNT          != -1 ?1 :-1];
        typedef char actions_count_is_defined
                [CLI_ACTIONS_COUNT           != -1 ?1 :-1];
#endif/*NO_STATIC_CLI_ASSERTIONS*/

/*--------------------------------------------------------------------------*/
/* Structure definitions                                                    */
/*--------------------------------------------------------------------------*/

union CliGenericValue CLI_GENERIC_VALUE_TYPES;
enum  CliGenericType  CLI_GENERIC_TYPE_IDENTIFIERS;

struct CliGeneric {
        const union CliGenericValue value;
        const enum CliGenericType   type;

        const u8 _pad[4];
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

        const u8 _pad[7];
};

struct CliCallbackContext {
        const struct CliCollection collection[CLI_COLLECTION_COUNT];
        const struct CliToggle     toggle[CLI_TOGGLE_COUNT];
        const struct CliResource   resource[CLI_RESOURCE_COUNT];
};

struct CliAction {
        const u8    *name;
        void (      *callback)(
                const struct CliCallbackContext context[1],
                u8                              rc[1]
        );
};

struct CliContextualisedCallback {
        struct CliCallbackContext context[1];
        void(                    *callback)(
                const struct CliCallbackContext context[1],
                u8                              rc[1]
        );
};

struct CliRegistry {
        struct CliAction     action     [CLI_ACTIONS_COUNT];
        struct CliCollection collection [CLI_COLLECTION_COUNT];
        struct CliToggle     toggle     [CLI_TOGGLE_COUNT];
};

struct CliContext {
        struct CliRegistry               registry[1];
        struct CliContextualisedCallback callback[CLI_MAX_ACTIONS];
};

/*--------------------------------------------------------------------------*/
/* Function declarations                                                    */
/*--------------------------------------------------------------------------*/

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
        void (           *callback)(
                struct CliCallbackContext context[1],
                u8                        rc[1]
        )
);

#endif/*ACLI_H*/
