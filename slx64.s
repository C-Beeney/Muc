/*---------------------------------------------------------------------------|
|                                                                            |
|   Copyright 2026 Connor R Beeney                                           |
|                                                                            |
|   Licensed under the Apache License, Version 2.0 (the "License");          |
|   you may not use this file except in compliance with the License.         |
|   You may obtain a copy of the License at                                  |
|                                                                            |
|       http://www.apache.org/licenses/LICENSE-2.0                           |
|                                                                            |
|   Unless required by applicable law or agreed to in writing, software      |
|   distributed under the License is distributed on an "AS IS" BASIS,        |
|   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. |
|   See the License for the specific language governing permissions and      |
|   limitations under the License.                                           |
|                                                                            |
|---------------------------------------------------------------------------*/

.globl  read
.globl  write
.globl  open
.globl  close
.globl  exit
.globl  abort
.globl  _start

read:
    movq    $0  , %rax
    syscall
    ret


write:
    movq    $1  , %rax
    syscall
    ret

open:
    movq    $2  , %rax
    syscall
    ret

close:
    movq    $3  , %rax
    syscall
    ret

exit:
    movq    $60 , %rax
    syscall

abort:
                    /* Get this process' PID             */
    movq $39, %rax  /* GETPID syscall                    */
    syscall
                    /* use KILL to SIGABRT this PID      */
    movq %rax, %rdi /* This PID                          */
    movq $62 , %rax /* KILL syscall                      */
    movq $6  , %rsi /* SIGABRT signal                    */
    syscall
                    /* Try to exit normally w/ code 127  */
    movq $60, %rax  /* EXIT syscall                      */
    movq $127, %rdi /* EXIT code 127                     */
    syscall

_start:

    mov (%rsp), %rdi                   /* argc */
    leaq 8(%rsp), %rsi                 /* argv */
    leaq 16(%rsp, %rdi, 8), %rdx       /* env  */

    andq $-16, %rsp                    /* sp is 16-byte aligned */

    call entry