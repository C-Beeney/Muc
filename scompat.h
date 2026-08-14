#ifndef SCOMPAT_H
#define SCOMPAT_H
#define read           gx_read
#define write          gx_write
#define open           gx_open
#define open_readonly  gx_open_readonly
#define close          gx_close
#define create         gx_create
#define create_passive gx_create_passive
#define exit           gx_exit
#define abort          gx_abort
#define __assert_fail  gx__assert_fail
#define strlen         gx_strlen
#define streq          gx_streq
#define itoa           gx_itoa
#define main           gx_main
#ifdef NDEBUG
    #define assert(expr) ((void)0)
#else
    #define assert(expr) \
        ((expr) ? (void)0 : gx__assert_fail((const u8*)#expr    , \
                                            (const u8*)__FILE__ , \
                                            (const s32)__LINE__ , \
                                            (const u8*) __func__))
#endif
#endif /*SCOMPAT_H*/
