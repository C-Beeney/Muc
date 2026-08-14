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

mkdir temp && cd temp

clang -c \
    -x c ../a*.c ../m*.c \
    -I.. -imacros ../scompat.h \
    -Weverything -Wno-reserved-identifier -Wno-empty-translation-unit \
    -Wno-unsafe-buffer-usage -Wno-c++-keyword -Wno-c2y-extensions \
    -fpie \
    -DNO_COMPILER_EXTENSIONS \
    -Werror --target=x86_64-pc-linux \
    -ansi \
    $@

cd -

clang -c \
    sposix.c -o temp/sposix.o -I. \
    -Weverything -Wno-reserved-identifier -Wno-empty-translation-unit \
    -Wno-unsafe-buffer-usage -Wno-c++-keyword -Wno-c2y-extensions -Wno-cast-qual \
    -fpie -Wno-cast-function-type-strict -Wno-incompatible-function-pointer-types-strict -Wno-incompatible-function-pointer-types \
    -DNO_COMPILER_EXTENSIONS \
    -Werror --target=x86_64-pc-linux -Wno-missing-noreturn \
    -ansi

clang temp/*.o -o out.elf -fpie -static

rm -fr temp/

strip out.elf
