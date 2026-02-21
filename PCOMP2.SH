clang -x c *.C -x assembler *.S -o OUT.ELF -nostdlib -nostdinc -I. -nostartfiles -Wno-main -std=c89 -Wno-incompatible-library-redeclaration -ffreestanding -v
