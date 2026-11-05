# Select RV32I + Zifencei (Instruction extensions), Teaching, then Build & load.
# Step over SW to see the word at patched change from "li a0,1" to "li a0,42".
# FENCE.I makes the stored bytes visible to this hart's subsequent instruction
# fetch. Back/Replay restores both code bytes and execution. Expected exit: 42.
    .option norvc
    .option norelax
.text
.globl _start
_start:
    la t0, patched
    li t1, 0x02a00513
    sw t1, 0(t0)
    fence.i
    j patched
    .balign 4
patched:
    addi a0, zero, 1
    addi a7, zero, 93
    ecall
