#include <util/types.h>

extern void __assert_fail(char*,char*,i32,const char*) __attribute__((__noreturn__));

// Simplified conceptual implementation in assert.h
#ifdef NDEBUG
# define assert(expr) ((void)0)
#else
# define assert(expr) \
    ((expr) ? (void)0 : __assert_fail(#expr, __FILE__, __LINE__, (const char*) __func__))
#endif
