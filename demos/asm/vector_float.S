# Zve32f: out[i] = left[i] * 2.0f + right[i], returning the sum's binary32 bits.
# Integer ABI arguments work with either ILP32 or LP64.
.text
.globl vector_float_sum
.type vector_float_sum,@function
vector_float_sum:
    fmv.w.x ft0,zero
    li t1,0x40000000
    fmv.w.x ft1,t1
    beqz a3,2f
1:
    vsetvli t0,a3,e32,m1,tu,mu
    vle32.v v8,(a0)
    vle32.v v12,(a1)
    vfmacc.vf v12,ft1,v8
    vse32.v v12,(a2)
    vfmv.s.f v16,ft0
    vfredosum.vs v16,v12,v16
    vfmv.f.s ft0,v16
    sub a3,a3,t0
    slli t0,t0,2
    add a0,a0,t0
    add a1,a1,t0
    add a2,a2,t0
    bnez a3,1b
2:
    fmv.x.w a0,ft0
    ret
.size vector_float_sum,.-vector_float_sum
