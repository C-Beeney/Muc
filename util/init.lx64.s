.att_syntax

.globl _start

.section .text

_start:

    mov (%rsp), %rdi                   ;# argc
    leaq 8(%rsp), %rsi                 ;# argv
    leaq 16(%rsp, %rdi, 8), %rdx       ;# env

    andq $-16, %rsp                    ;# sp is 16-byte aligned

    call entry
