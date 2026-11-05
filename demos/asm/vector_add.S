.text
.globl add_vectors
.type add_vectors,@function
# C ABI: a0=left, a1=right, a2=out, a3=count. All vector registers
# are caller-saved. vsetvli handles the final, shorter group too.
add_vectors:
    beqz a3,2f
1:
    vsetvli t0,a3,e32,m1,tu,mu
    vle32.v v8,(a0)
    vle32.v v12,(a1)
    vadd.vv v16,v8,v12
    vse32.v v16,(a2)
    slli t1,t0,2
    add a0,a0,t1
    add a1,a1,t1
    add a2,a2,t1
    sub a3,a3,t0
    bnez a3,1b
2:
    ret
.size add_vectors,.-add_vectors
