.att_syntax

.globl read
.globl write
.globl open
.globl close

.section .text

read:
    movq $0, %rax
    syscall
    ret


write:
    movq $1, %rax
    syscall
    ret

open:
    movq $2, %rax
    syscall
    ret

close:
    movq $3, %rax
    syscall
    ret
