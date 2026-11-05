.text
.globl _start
_start:
    addi t0, zero, 20
    addi t1, zero, 22
    add a0, t0, t1
    addi a7, zero, 93
    ecall
