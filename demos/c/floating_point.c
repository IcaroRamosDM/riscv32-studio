#include <stdio.h>
#include <rv_float.h>
volatile float first = 0.1f, second = 0.2f, sum, root, fused;
volatile uint32_t result, exceptions;

int main(void)
{
    rv_float_set_rounding(RV_FLOAT_RNE);
    rv_float_clear_flags();
    sum = first + second;
    root = rv_float_sqrt(2.0f);
    fused = rv_float_fma(rv_float_from_bits(0x3f800001u),
        rv_float_from_bits(0x3f800001u),
        rv_float_from_bits(0xbf800002u));
    result = rv_float_bits(sum);
    exceptions = rv_float_flags();
    printf("sum bits=%x root bits=%x fused bits=%x flags=%u\n",
        (unsigned)result,
        (unsigned)rv_float_bits(root),
        (unsigned)rv_float_bits(fused),
        (unsigned)exceptions);
    return result == 0x3e99999au && rv_float_bits(root) == 0x3fb504f3u &&
            rv_float_bits(fused) == 0x28800000u && exceptions == RV_FLOAT_NX
        ? 0
        : 1;
}
