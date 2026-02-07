.section .text
.global memset
.type memset, @function

;# void *memset(void *s, int c, size_t n)
;# Arguments (System V AMD64 ABI):
;# rdi = Destination pointer (s)
;# rsi = Value to set (c)
;# rdx = Number of bytes (n)
;# Return: rax = Original destination pointer

memset:
    mov %rdi, %rax    ;# Save original pointer to return at the end
    mov %sil, %cl     ;# Move the byte 'c' (low 8 bits of rsi) into cl
    mov %rdx, %rcx    ;# Move the count 'n' into rcx for the REP instruction
    mov %sil, %al     ;# Move byte 'c' into al (the byte to be stored)
    
    ;# Optional: Fill all of rax with the byte 'c' for 64-bit speed
    ;# This isn't strictly necessary for 'stosb' but is good practice
    mov %rax, %r8     ;# Temporarily store our return address
    
    rep stosb         ;# Repeat 'store byte' from al to (rdi) until rcx is 0
                      ;# rdi is automatically incremented by the CPU

    mov %r8, %rax     ;# Restore the original pointer for the return value
    ret

.size memset, .-memset
