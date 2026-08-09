#ifndef UMEM_H
#define UMEM_H

#include <stypes.h>

void *memcpy(void *dest, const void *src, umax n);
void *memset(void *dest, s32 c, umax n);
void *memmove(void *dest, const void *src, umax n);
const void *memcmp(const void *s1, const void *s2, umax n);
#endif/*UMEM_H*/
