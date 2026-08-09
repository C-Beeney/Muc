#include <stypes.h>

#define _POSIX_C_SOURCE 1
#include <sys/types.h>
#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <limits.h>

u32
gx_read(
    const u32 fh,
    u8* buffer,
    const smax count
) {

    ssize_t read_count;

    assert(fh <= INT_MAX);
    assert(count > -1);

    read_count = (u32)read((int)fh, (void*)buffer, (size_t)count);

    assert(read_count <= U32_MAX);
    
    if(read_count != count) {
        ssize_t read_count1 = (u32)read((int)fh, (void*)buffer, (size_t)count);
        return ((read_count1 + read_count) == count) ? count : 0;
    }

    return (u32)count;

}

u32  gx_write (const u32 fh  , const u8 *buffer, const smax count);
u32  gx_open  (const u8 *path, const u32 mode);
void gx_close (const u32 fh);
void gx_exit  (const s8 ec);
void gx_abort (void);