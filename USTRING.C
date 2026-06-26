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
umax strlen(const u8 *buffer) {
        umax len = (umax) -1;
        assert(buffer && "Buffer may not be null.");
        while(buffer[++len]);
        return len;
}

u8 streq(
        const u8 buffer0[],
        const u8 buffer1[]
) {
        umax idx = UMAX_MAX;

        assert(buffer0 && "Buffer0 may not be null.");
        assert(buffer1 && "Buffer1 may not be null.");

        while(
                buffer0[++idx] && buffer1[idx] &&
                buffer0[  idx] == buffer1[idx]
        );

        return (
                buffer0[idx] ==
                buffer1[idx]
        );
}
