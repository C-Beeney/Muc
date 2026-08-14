#include <stypes.h>

#define SSYS_H_NODECL
#define SCOMPAT_IMPL
#include <ssys.h>

#define _POSIX_C_SOURCE 1
#include <sys/types.h>
#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#ifndef SPOSIX_DISK_MAX_TRIES
#define SPOSIX_DISK_MAX_TRIES 15
#endif/*SPOSIX_DISK_MAX_TRIES*/

static void
gx_disk(
        const u16 fh,
        u8* buffer,
        const umax count,
        enum status status[1],
        ssize_t(*method)(int,void *,size_t)

) {
        ssize_t disk_total = 0;
        u8 tries = 0;

        *status = GX_OK;

        assert(count <= S32_MAX);
        assert((void*)method == (void*)read || (void*)method == (void*)write);

        if(0 == count) {
                *status = GX_OK;
                return;
        }

        while(disk_total != count) {
                ssize_t n = method(
                        (int)fh,
                        (void *)(buffer + disk_total),
                        (size_t)(count - disk_total)
                );


                if (n == 0 && method == read) {
                        *status = GX_EOF;
                        return;
                }

                if(n < 0 && errno == EINTR) {

                        /* TODO: Less nesting */
                        if(++tries > SPOSIX_DISK_MAX_TRIES) {
                                /* TODO: Placeholder error*/
                                *status = GX_GENERIC_ERR;
                                return;
                        }
                        continue;
                }

                if(n < 0) {
                        /* TODO: specify */
                        *status = GX_GENERIC_ERR;
                        return;
                }

                tries = 0;
                assert(disk_total + n <= U32_MAX);
                disk_total += n;
                assert(disk_total <= count);
        }

        assert(disk_total == count);
}   

void
gx_read(
        const u16 fh,
        u8 *buffer,
        const umax count,
        enum status status[1]
) {
        gx_disk(fh, buffer, count, status, (ssize_t(*)(int, const void *, size_t))read);
}

void
gx_write(
        const u16 fh,
        const u8 *buffer,
        const umax count,
        enum status status[1]
) {
        gx_disk(fh, (u8*)buffer, count, status, write);
}

/*
    U16_MAX is a valid file handle, but is also returned on error.
    check status to verify wheather there was an error
*/
static u16
gx_open_generic_mode(
        const u8 *path,
        const int mode,
        enum status status[1]
) {
        int fh = open((const char *)path, mode);

        if(fh < 0) {
                /* TODO: Proper value */
                *status = GX_GENERIC_ERR;
                return U16_MAX;
        }

        if(fh > U16_MAX) {
                /* TODO: Proper value */
                *status = GX_GENERIC_ERR;
                return U16_MAX;
        }

        *status = GX_OK;
        return (u16)fh;
}

u16
gx_open(
        const u8 *path,
        enum status status[1]
) {
        return gx_open_generic_mode(path, O_RDWR, status);
}

u16
gx_open_readonly(
    const u8 *path,
    enum status status[1]
) {
        return gx_open_generic_mode(path, O_RDONLY, status);
}

u16
gx_create(
        const u8 *path,
        enum status status[1]
) {
        int fh = open((const char*)path, O_RDWR | O_CREAT | O_EXCL, 0664);

        if(-1 == fh && EEXIST == errno) {
                /* TODO: Proper value */
                *status = GX_GENERIC_ERR;
                return U16_MAX;
        }

        if(-1 == fh) {
                /* Todo: Specify */
                *status = GX_GENERIC_ERR;
                return U16_MAX;
        }

        *status = GX_OK;

        assert(fh <= U16_MAX);
        assert(fh >= 0);

        return (u16)fh;
}

void
gx_create_passive(
    const u8 *path,
    enum status status[1]
) {

        u16 fh = gx_create(path, status);
        int rc;

        if(*status != GX_OK) {
            return;
        }

        rc = close(fh);
        assert(rc == 0 || rc == -1);

        if(rc == -1) {
            /* TODO: Proper value */
            *status = GX_GENERIC_ERR;
            return;
        }

        *status = GX_OK;

}

void
gx_close(
    const u16 fh,
    enum status status[1]
) {
    int rc;
    rc = close((int)fh);

    assert(rc == 0 || rc == -1);

    /* TODO: More comprehensive */
    if(-1 == rc) {
        *status = GX_GENERIC_ERR;
        return;
    }

    *status = GX_OK;
}

void
gx_exit(
    const s8 ec
) {
    exit((int)ec);
}

void
gx_abort(
    void
) {
    abort();
}

int gx__assert_fail(
    const u8 *expr,
    const u8 *file,
    const s32 line,
    const u8 *func
) {
    u8 line_buf[64];

    write(STDERR_FILENO, (const char*) "Assertion failed: ", 19          );
    write(STDERR_FILENO, (const char*) expr                , strlen((const char*)expr));
    write(STDERR_FILENO, (const char*) "\n"                , 1);

    write(STDERR_FILENO, (const char*) file, strlen((const char*)file));
    write(STDERR_FILENO, (const char*) " -> ", 4);
    write(STDERR_FILENO, (const char*) func, strlen((const char*)func));
    write(STDERR_FILENO, (const char*) " at line ", 10);
    gx_itoa((smax) line, line_buf);
    write(STDERR_FILENO, (const char*) line_buf, strlen((const char*)line_buf));
    write(STDERR_FILENO, (const char*) "\n", 1);

    abort();
}

smax
gx_strlen(
    const u8 *buffer
) {
    size_t len = strlen((const char*)buffer);
    assert(len < SMAX_MAX);
    return (smax) len;
}

u8
gx_streq(
    const u8 *buffer0,
    const u8 *buffer1
) {
    int res = strcmp((const char*)buffer0, (const char*)buffer1);
    return (u8) (0 == res);
}

u8*
gx_itoa(
    smax i,
    u8 *buffer
) {
    assert(i<=INT_MAX);
    assert(i>=INT_MIN);
    sprintf((char *)buffer, "%d", i);
    return buffer;
}

extern s8 gx_main(const s32 argc, const u8 *argv[], const u8 *env[]);

int main(int argc, char **argv) {
    extern char **environ;
    return gx_main(argc, (const u8 **)argv, (const u8 **)environ);
}
