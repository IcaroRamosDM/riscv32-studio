# Mixed 16/32-bit encodings. Select RV32IC and Teaching, then Build & load.
# Follow offsets 0 -> 2 -> 6 -> 16 -> 18 -> 8 -> 12 from _start. Exit: 42.
    .option rvc
    .option norelax
.text
.globl _start
_start:
    c.li a0, 20
    .option push
    .option norvc
    addi a0, a0, 21
    .option pop
    c.jal increment
    li a7, 93
    ecall
.type increment, @function
increment:
    c.addi a0, 1
    c.jr ra
.size increment, .-increment
