.att_syntax

.globl read
.globl write
.globl open
.globl close
.globl exit
.globl _start
.globl __assert_fail
.type __assert_fail, @function

.section .rodata
    msg_prefix:  .string "Assertion failed: "
    msg_newline: .string "\n"

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

__assert_fail:
    /* Arguments received from C:
       %rdi: assertion string (char *)
       %rsi: file string (char *)
       %edx: line number (unsigned int)
       %rcx: function string (char *)
    */

    /* 1. Save assertion pointer to %r12 (callee-saved) so we don't lose it */
    pushq %r12
    movq %rdi, %r12

    /* 2. Write "Assertion failed: " to stderr (fd 2) */
    movq $1, %rax           /* syscall: write */
    movq $2, %rdi           /* fd: stderr */
    leaq msg_prefix(%rip), %rsi
    movq $18, %rdx          /* length of "Assertion failed: " */
    syscall

    /* 2.5. Calculate length of assertion fail expression string */
    movq %r12, %rdi
    call strlen
    movq %rax, %r11


    /* 3. Write the actual failed expression string */
    /* Note: For simplicity, we assume a fixed length or you can call a strlen loop */
    movq $1, %rax           
    movq $2, %rdi           
    movq %r12, %rsi         /* The pointer we saved */
    movq %r11, %rdx         /* Replace with actual length logic if needed */
    syscall

    /* 4. Write newline */
    movq $1, %rax
    movq $2, %rdi
    leaq msg_newline(%rip), %rsi
    movq $1, %rdx
    syscall

    /* 5. Exit program (syscall 60) */
    movq $60, %rax          /* syscall: exit */
    movq $1, %rdi           /* status: 1 */
    syscall

_start:

    mov (%rsp), %rdi                   ;# argc
    leaq 8(%rsp), %rsi                 ;# argv
    leaq 16(%rsp, %rdi, 8), %rdx       ;# env

    andq $-16, %rsp                    ;# sp is 16-byte aligned

    call entry
