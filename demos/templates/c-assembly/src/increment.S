.text
.globl increment
.type increment,@function
increment:
    addi a0, a0, 1
    ret
.size increment, .-increment
