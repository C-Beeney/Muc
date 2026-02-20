.att_syntax

.globl read
.globl write
.globl open
.globl close
.globl exit

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

exit:
    movq $60, %rax
    syscall
