#include <util/string.h>
#include <util/assert.h>

umax strlen(const u8 *buffer) {
    assert(buffer && "Buffer may not be null.");
    umax len = (umax) -1;
    while(buffer[++len]);
    return len;
}
