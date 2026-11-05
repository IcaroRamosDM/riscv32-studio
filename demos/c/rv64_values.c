#include <stdint.h>
#include <stdio.h>

// Choose RV64IM / LP64 and the Teaching environment.
// Watch left, right and result; the register view preserves all sixteen hex digits.
volatile uint64_t left = UINT64_C(0xfedcba9876543210);
volatile uint64_t right = UINT64_C(0x100000003);
volatile uint64_t result;

__attribute__((noinline)) uint64_t calculate(uint64_t a, uint64_t b)
{
    return a * b + a / b;
}

int main(void)
{
    result = calculate(left, right);
    puts("RV64: inspect result, a0 and the saved history.");
    return sizeof(void *) == 8 && sizeof(long) == 8 ? 0 : 1;
}
