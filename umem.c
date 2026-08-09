/* This file was AI-Generated! */

#include <umem.h>

void *
memcpy(
        void       *dest,
        const void *src,
        umax        n
) {
        u8 *d = (u8*) dest;
        const u8 *s = (const u8*) src;
        umax i;

        for(i=0; i<n; ++i) {
                d[i] = s[i];
        }

        return dest;
}

void *
memset(
        void       *dest,
        s32         c,
        umax        n
) {
        u8 *d = (u8*) dest;
        umax i;

        for(i=0; i<n; ++i) {
                d[i] = (u8)c;
        }

        return dest;
}

void *
memmove(
        void       *dest,
        const void *src,
        umax        n
) {
        u8 *d = (u8*) dest;
        const u8 *s = (const u8*) src;
        umax i;

        if(d < s) {
                for(i=0; i<n; ++i) {
                        d[i] = s[i];
                }
        } else if(d > s) {
                for(i=n; i>0; --i) {
                        d[i-1] = s[i-1];
                }
        }

        return dest;
}

const void *
memcmp(
        const void *s1,
        const void *s2,
        umax        n
) {
        const u8 *p1 = (const u8*) s1;
        const u8 *p2 = (const u8*) s2;
        umax i;

        for(i=0; i<n; ++i) {
                if(p1[i] != p2[i]) {
                        return (const void*)(p1 + i);
                }
        }

        return 0;
}
