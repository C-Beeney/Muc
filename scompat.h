#ifndef SCOMPAT_H
#define SCOMPAT_H
#define read          gx_read
#define write         gx_write
#define open          gx_open
#define close         gx_close
#define exit          gx_exit
#define abort         gx_abort
#define __assert_fail gx__assert_fail
#define memcpy        gx_memcpy
#define memset        gx_memset
#define memmove       gx_memmove
#define strlen        gx_strlen
#define streq         gx_streq
#define itoa          gx_itoa
#endif /*SCOMPAT_H*/