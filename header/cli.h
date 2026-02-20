#include <util/types.h>
#include <util/assert.h>
#include <util/string.h>

#ifndef CLI_CONFIG_H
#define CLI_CONFIG_H
#include <cli.conf.h>
#endif

struct CliCollection;
struct CliToggle;
struct CliRegistry;
struct CliContext;
struct CliGeneric;
struct CliResource;

#ifndef COLLECTIONS_COUNT
#   define COLLECTIONS_COUNT -1
#endif/*COLLECTIONS_COUNT*/
#ifndef COLLECTIONS_MAX_CHILDREN
#   define COLLECTIONS_MAX_CHILDREN -1
#endif/*COLLECTIONS_MAX_CHILDREN*/
#ifndef TOGGLES_COUNT
#   define TOGGLES_COUNT -1
#endif/*TOGGLES_COUNT*/
#ifndef RESOURCE_TYPE_IDENTS
#   define RESOURCE_TYPE_IDENTS resource_type_idents_must_be_defined
#endif/*GENERIC_TYPES*/
#ifndef RESOURCE_VALUES
#   define RESOURCE_VALUES resource_values_must_be_defined
#endif/*GENERIC_VALUES*/
#ifndef RESOURCES_COUNT
#define RESOURCES_COUNT -1
#endif/*RESOURCES_COUNT*/
#ifndef NO_STATIC_CLI_ASSERTIONS
#   ifndef NO_STATIC_COLLECTIONS_COUNT_IS_DEFINED_ASSERTION
        typedef char collections_count_is_defined [COLLECTIONS_COUNT];
#   endif/*NO_STATIC_COLLECTIONS_COUNT_IS_DEFINED_ASSERTION*/
#   ifndef NO_STATIC_COLLECTIONS_MAX_CHILDREN_IS_DEFINED_ASSERTION
        typedef char collections_max_children_is_defined_assertion [COLLECTIONS_MAX_CHILDREN];
#   endif/*NO_STATIC_COLLECTIONS_MAX_CHILDREN_IS_DEFINED_ASSERTION*/
#   ifndef NO_STATIC_TOGGLES_COUNT_IS_DEFINED_ASSERTION
        typedef char toggles_count_is_defined [TOGGLES_COUNT];
#   endif/*NO_STATIC_TOGGLES_COUNT_IS_DEFINED_ASSERTION*/
#   ifndef NO_STATIC_RESOURCES_COUNT_IS_DEFINED_ASSERTION
        typedef char resources_count_is_defined [RESOURCES_COUNT];
#   endif
#endif

struct CliCollection {
    const u8 *name;
    u8 *values[COLLECTIONS_MAX_CHILDREN];
};

struct CliToggle {
    const u8 *name;
    u8 value;
};

struct CliRegistry {
    struct CliCollection collections[COLLECTIONS_COUNT];
    struct CliToggle toggles[TOGGLES_COUNT];
};

struct CliContext {
    struct CliRegistry cli_registry;
};

enum CliGenericType RESOURCE_TYPE_IDENTS;
union CliGenericValue RESOURCE_VALUES value;

struct CliGeneric {
    enum CliGenericType type;
    union CliGenericValue value;
};

struct CliResource {
    const u8 *name;
    struct CliGeneric resources[RESOURCES_COUNT];
};

void cli_register_collection(struct CliContext cli_context[1], const u8 collection_name[], u8** collection_memory);
void cli_register_toggle(struct CliContext cli_context[1], const u8 toggle_name[], u8 default_value);
void cli_register_action(struct CliContext cli_context[1], const u8 action_name[], void(*action)(u8 rc[1], const struct CliCollection *collections, const struct CliToggle *actions));