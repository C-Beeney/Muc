.att_syntax

.globl strlen
.globl itoa
;# system v abi -> rax, rdi, rsi, rdx, rcx, r8, r9, r10, r11
;# linux cconv  -> rax,	rdi, rsi, rdx, r10, r8, r9

;# rdi is the char* to measure length of, return int (rax)
strlen:
    xor %rax, %rax                     ;# Clear the return value
.strlen_loop:
    cmpb $0, (%rdi, %rax, 1)           ;# See if current byte is null terminator
    jz .end                            ;# If it is, return with current %rax (length)
    inc %rax                           ;# Increment length
    jmp .strlen_loop                   ;# Perform the test again
.end:
    ret

itoa:
;# %edi 32-bit integer
;# %rsi pointer to end of string
;# %rax pointer to beginning of number within cstring ending at %rsi

;# Move important data to more important registers, perform divisons on %edi, write mod10 plus '0' to %rsi output, decrementing index until %edi is 0
    movl %edi, %eax                    ;# Move passed num to dividend
    movq %rsi, %r8                     ;# Save the end of buffer to a non-overwritable store
    decq %r8                           ;# Move backward to be at end of string not 1 past e.g. calculated by strlen
    movb $0, (%r8)                     ;# Ensure there is a null terminator at the end of the string
    movl $10, %ecx                     ;# Set the divisor
.itoa_loop:
    xorl %edx, %edx                    ;# Clear edx
    divl %ecx                          ;# Divide by ecx, %eax now contains the same int but divided by 10, %edx is the digit we want :)
    addb $'0', %dl                     ;# Convert the remainder (%edx, but we know it's under 8 bit limit (0-9)) to ASCII instead of plain number
    decq %r8                           ;# Decrement string pointer
    movb %dl, (%r8)                    ;# Write the byte out to ram :)
    testl %eax, %eax                   ;# Set ZF if eax is zero (Indicating we are done)
    jnz .itoa_loop                     ;# If ZF not set, restart loop (Convert next digit to char)
    movq %r8, %rax                     ;# Move ptr to numstr start to retval reg
    ret                                ;# Return from function

;# TODO: Handle zero case (return string of 0 specifically) :)
