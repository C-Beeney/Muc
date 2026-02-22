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

#include <ACLI.H>
#include <UASSERT.H>
#include <SSYS.H>

void cli_register_toggle(
        struct CliContext context[1],
        const u8          name[],
        const u8          value
) {
        static imax index = -1;

        struct CliToggle *this;

        index += 1;

        assert(context                  && "Context exists.");
        assert(name                     && "Name exists.");
        assert(0 == value || 1 == value && "Value is 0 or 1.");
        assert(index < CLI_TOGGLE_COUNT && "index below CLI_TOGGLE_COUNT.");

        this = context->registry->toggle + index;

        this->name  = name;
        this->value = value;
}

void cli_register_collection(
        struct CliContext context[1],
        const u8          name[],
        const u8         *value[CLI_COLLECTION_MAX_CHILDREN]
) {
        static imax index = -1;

        struct CliCollection *this;
        imax copy_index;

        index += 1;

        assert(context  && "Context exists.");
        assert(name     && "Name exists.");
        assert(value    && "Value exists.");

        this = context->registry->collection + index;

        for (copy_index = 0; 0 != value[copy_index]; ++copy_index)
                this->value[copy_index] = value[copy_index];
        

}

void cli_register_action(
        struct CliContext context[1],
        const u8          name[],
        void (           *callback)(
                struct CliCallbackContext context[1],
                u8                        rc[1]
        )
) {
        (void) context; (void) name; (void) callback;
}
