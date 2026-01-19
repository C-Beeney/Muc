.att_syntax

.globl _start

.section .text

_start:

    mov (%rsp), %rdi                   ;# argc
    leaq 8(%rsp), %rsi                 ;# argv
    leaq 16(%rsp, %rdi, 8), %rdx       ;# env

    andq $-16, %rsp                    ;# sp is 16-byte aligned

    call main

    mov %rax, %rdi                     ;# set prog exit code to main retval
    movq $0x3c, %rax                   ;# exit syscall
    syscall
