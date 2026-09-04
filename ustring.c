/*---------------------------------------------------------------------------|
|                                                                            |
|   Copyright 2026 Connor R Beeney                                           |
|                                                                            |
|   Licensed under the Apache License, Version 2.0 (the "License");          |
|   you may not use this file except in compliance with the License.         |
|   You may obtain a copy of the License at                                  |
|                                                                            |
|       http://www.apache.org/licenses/LICENSE-2.0                           |
|                                                                            |
|   Unless required by applicable law or agreed to in writing, software      |
|   distributed under the License is distributed on an "AS IS" BASIS,        |
|   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. |
|   See the License for the specific language governing permissions and      |
|   limitations under the License.                                           |
|                                                                            |
|---------------------------------------------------------------------------*/

#include <ustring.h>
#include <uassert.h>

/* TODO: Remove all UB for future MISRA compliance */
umax
strlen(
        const u8 *buffer
) {
        umax len = UMAX_MAX;
        assert(buffer);
        while(buffer[++len]);
        return len;
}

u8
streq(
        const u8 *buffer0,
        const u8 *buffer1
) {
        umax idx = 0;

        assert(buffer0);
        assert(buffer1);

        while(
                buffer0[idx] && buffer1[idx] &&
                buffer0[idx] == buffer1[idx]
        ) ++idx;

        return (
                buffer0[idx] ==
                buffer1[idx]
        );
}

u8*
itoa(
    smax i  ,
    u8  *arr
) {
    smax idx = 0;
    smax start = 0;
    smax end;
    u8 *base = arr;

    umax u;
    if (i < 0) {
        *arr++ = '-';
        u = (umax)(-(umax)i);
    } else {
        u = (umax)i;
    }

    if (u == 0) {
        arr[idx++] = '0';
        arr[idx] = 0;
        return base;
    }

    while (u > 0) {
        arr[idx++] = (u8)('0' + (u % 10));
        u /= 10;
    }

    arr[idx] = 0;

    end = idx - 1;

    while (start < end) {
        u8 tmp = arr[start];
        arr[start] = arr[end];
        arr[end] = tmp;
        start++;
        end--;
    }

    return base;
}
