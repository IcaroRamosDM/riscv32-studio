#include <rv_bits.h>
#include <rv_stdio.h>
#if __riscv_xlen != 64
    #error This example requires RV64 Zba/Zbb/Zbc/Zbs.
#endif
volatile uintptr_t input = UINT64_C(0x8000000100000001), shift = 63;
volatile uintptr_t reversed, rotated, ones, unsigned_word, word_rotation, polynomial;

int main(void)
{
    reversed = rv_reverse_bytes(input);
    rotated = rv_ror(input, shift);
    ones = rv_cpop(input);
    unsigned_word = rv_sh3add_uw(input, UINT64_C(0x100000000));
    word_rotation = rv_rorw(UINT64_C(0x0123456780000001), 1);
    polynomial = rv_clmul_high(UINT64_MAX, UINT64_MAX);
    rv_puts("RV64 bits OK\n");
    return reversed != UINT64_C(0x0100000001000080) || rotated != UINT64_C(0x0000000200000003) ||
        ones != 3 || unsigned_word != UINT64_C(0x100000008) ||
        word_rotation != UINT64_C(0xffffffffc0000000) || polynomial != UINT64_C(0x5555555555555555);
}
