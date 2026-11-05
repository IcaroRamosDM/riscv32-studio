#include <stdint.h>
volatile uint32_t output[4];
/* GCC before 13 accepts vector Assembly through GNU as but cannot name vector
   registers in clobbers; it does not allocate those registers itself. */
#if defined(__clang__) || __GNUC__ >= 13
    #define VECTOR_CLOBBERS "v0", "v4", "v8",
#else
    #define VECTOR_CLOBBERS
#endif
int main(void)
{
    const uint32_t input[4] = {1, 2, 3, 4};
    __asm__ volatile("li t0,4\n"
                     "vsetvli t1,t0,e32,m1,tu,mu\n"
                     "vle32.v v8,(%0)\n"
                     "vid.v v4\n"
                     "vand.vi v4,v4,1\n"
                     "vmseq.vi v0,v4,0\n"
                     "vadd.vi v8,v8,10,v0.t\n"
                     "vse32.v v8,(%1)\n"
        :
        : "r"(input), "r"(output)
        : "t0", "t1", VECTOR_CLOBBERS "memory");
    return 0;
}
