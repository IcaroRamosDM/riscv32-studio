#include <rv_float.h>
#include <stdio.h>
volatile int64_t signed_result;
volatile uint64_t unsigned_result;
volatile uint32_t rounded_bits, exceptions;

int main(void)
{
    rv_float_set_rounding(RV_FLOAT_RNE);
    rv_float_clear_flags();
    signed_result = rv_float_to_i64(rv_float_from_bits(0xd3800000u));
    unsigned_result = rv_float_to_u64(rv_float_from_bits(0x5f000000u));
    if (rv_float_bits(rv_float_from_i64(signed_result)) != 0xd3800000u)
        return 1;
    rounded_bits = rv_float_bits(rv_float_from_u64(UINT64_MAX));
    exceptions = rv_float_flags();
    int ok = signed_result == -INT64_C(1099511627776) &&
        unsigned_result == UINT64_C(9223372036854775808) && rounded_bits == 0x5f800000u &&
        exceptions == RV_FLOAT_NX;
    puts(ok ? "RV64 floating OK" : "RV64 floating failed");
    return !ok;
}
