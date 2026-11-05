# 64KiB: sw traps because 0x10000 is outside RAM.
# 128KiB: Memory at 0x10000 contains 2A 00 00 00; exit code is 0.
    .option norvc
.text
.globl _start
_start:
    lui t0, 0x10
    addi t1, zero, 42
    sw t1, 0(t0)
    lw a0, 0(t0)
    addi a0, a0, -42
    addi a7, zero, 93
    ecall
