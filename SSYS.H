#include<STYPES.H>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

u32  read  (u32 fh  , u8* buffer, imax count);
u32  write (u32 fh  , u8 *buffer, imax count);
u32  open  (u8 *path, u32 mode);
void close (u32 fh);
void exit  (i8 ec);