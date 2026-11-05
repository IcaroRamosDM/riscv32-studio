# The virtual firmware exit register accepts the program status.
.text
.globl _start
_start:
    li t0, 0xffff0018
    sw zero, 0(t0)
halt:
    j halt
