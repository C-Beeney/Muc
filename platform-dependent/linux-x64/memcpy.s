.att_syntax

.globl memcpy

memcpy:
        ;# 1. SETUP RETURN VALUE
        ;# The C standard says memcpy returns 'dest'.
        ;# In x64, return value goes in %rax.
        movq    %rdi, %rax

        ;# 2. DIRECTIONAL SAFETY (8086 Legacy)
        ;# Clear the Direction Flag. This ensures 'rep movsb' 
        ;# increments RDI/RSI (moving forward) rather than decrementing.
        cld

        ;# 3. SET COUNTER
        ;# The 'rep' prefix uses the CX register. In x64, this is %rcx.
        movq    %rdx, %rcx

        ;# 4. THE STRING MOVE
        ;# Inherited from 8086: Copies RCX bytes from [RSI] to [RDI].
        rep     movsb

        ;# 5. RETURN
        ret

;# Sourced from gemini
