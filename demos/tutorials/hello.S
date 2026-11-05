    /* Teaching environment: service 0x100 writes the byte in a0 to Terminal.
    * Service 93 exits with a0=status. Put the service number in a7.
    * These are Studio services, not Linux system calls or ISA instructions.
    * No C startup or stack is needed for this program. */
    .option norelax
.section .rodata
    message: .ascii "Hello from Assembly!\n"
message_end:
    .equ message_length, message_end - message
.text
.globl _start
_start:
    la t0, message
    li t1, message_length
    li a7, 0x100
print_next:
    lbu a0, 0(t0)
    ecall
    addi t0, t0, 1
    addi t1, t1, -1
    bnez t1, print_next
    li a0, 0
    li a7, 93
    ecall
