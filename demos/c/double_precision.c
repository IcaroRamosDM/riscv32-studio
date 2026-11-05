#include <stdio.h>
#include "rv_double.h"

volatile double sum, root, fused;
volatile uint64_t result_bits;
volatile uint32_t exceptions;

int main(void)
{
    volatile double one = 1.0, half_ulp = 0x1p-53;
    rv_float_set_rounding(RV_FLOAT_RNE);
    rv_float_clear_flags();
    sum = one + half_ulp;
    root = rv_double_sqrt(4.0);
    fused = rv_double_fma(1.5, 2.0, 0.5);
    result_bits = rv_double_bits(sum);
    exceptions = rv_float_flags();
    printf("double: sum=%.17g sqrt=%.17g fma=%.17g\n", sum, root, fused);
    printf("bits=%016llx flags=%u\n", (unsigned long long)result_bits, (unsigned)exceptions);
    return sum == 1.0 && root == 2.0 && fused == 3.5 && exceptions == RV_FLOAT_NX ? 0 : 1;
}
