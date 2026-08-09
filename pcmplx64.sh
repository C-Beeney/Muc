#*---------------------------------------------------------------------------|
#                                                                            |
#   Copyright 2026 Connor R Beeney                                           |
#                                                                            |
#   Licensed under the Apache License, Version 2.0 (the "License");          |
#   you may not use this file except in compliance with the License.         |
#   You may obtain a copy of the License at                                  |
#                                                                            |
#       http://www.apache.org/licenses/LICENSE-2.0                           |
#                                                                            |
#   Unless required by applicable law or agreed to in writing, software      |
#   distributed under the License is distributed on an "AS IS" BASIS,        |
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. |
#   See the License for the specific language governing permissions and      |
#   limitations under the License.                                           |
#                                                                            |
#---------------------------------------------------------------------------*/

clang \
    -x c a*.c m*.c u*.c sentry.c \
    -x assembler slx64.s \
    -o out.elf \
    -flto -static -Wl,--gc-sections \
    -Oz -s \
    -nostdlib -nostdinc -nostartfiles \
    -I.\
    -Weverything -Wno-reserved-identifier -Wno-empty-translation-unit \
    -Wno-unsafe-buffer-usage -Wno-c++-keyword -Wno-c2y-extensions\
    -ffreestanding -ffunction-sections -fdata-sections \
    -fvisibility=hidden -fno-stack-check -fno-pic -fno-pie \
    -fno-asynchronous-unwind-tables -fno-unwind-tables -fno-exceptions \
    -fno-stack-protector -fno-builtin -fno-common -mno-implicit-float \
    -DNO_COMPILER_EXTENSIONS \
    -Werror --target=x86_64-pc-linux -Wl,-T,slx64.ld \
    -mno-sse -mno-sse2 -mno-mmx -msoft-float \
    -ansi -mno-red-zone -Wl,--icf=all -fmerge-all-constants -fuse-ld=lld \
    $@
    
sstrip out.elf
