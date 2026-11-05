/* Four 32-bit elements fit in a 128-bit vector. Inspect v8 and v9.
 * Memory clobber describes the load/store to the C compiler. */
#include <stdint.h>
#include <stdio.h>

static const uint32_t input[4] = {1, 2, 3, 4};
static uint32_t output[4];

int main(void)
{
    unsigned long count = 4;
    __asm__ volatile("vsetvli zero,%0,e32,m1,tu,mu\n"
                     "vle32.v v8,(%1)\n"
                     "vadd.vi v9,v8,10\n"
                     "vse32.v v9,(%2)\n"
        :
        : "r"(count), "r"(input), "r"(output)
        : "v8", "v9", "memory");
    printf("vector = [%u, %u, %u, %u]\n",
        (unsigned)output[0],
        (unsigned)output[1],
        (unsigned)output[2],
        (unsigned)output[3]);
    return output[0] == 11 && output[1] == 12 && output[2] == 13 && output[3] == 14 ? 0 : 1;
}
