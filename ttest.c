#include <stdio.h>

struct StructA {
    char *string;
    int some_value;
    int:32;
};

struct StructB {
    char *string;
    char *some_other_string;
};

struct StructC {
    char *string;
    char some_boolean_value;
    int:8;int:16;int:32;
};

union valid_structs {
    struct StructA* structA;
    struct StructB* structB;
    struct StructC* structC;
};

static void print_string_from_struct(
    union valid_structs s
) {
    printf("Test -> %s\n", (**(char***)&s));
}

int main(void) {
    struct StructA a = {"bingbong", 5};
    struct StructB b = {"bongbind", "bongbong"};
    struct StructC c = {"bingbing"};
    union valid_structs validStructs;
    validStructs.structA = &a;

    printf("%lu\n", sizeof(struct StructA));
    printf("%lu\n", sizeof(struct StructB));
    printf("%lu\n", sizeof(struct StructC));


    print_string_from_struct(validStructs);
    return 0;
}
