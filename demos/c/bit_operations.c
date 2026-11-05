#include <stdio.h>
#include <inttypes.h>
#include <rv_bits.h>

volatile uint32_t input = 0x80000081u;
volatile uint32_t ones, rotated, byte_order, polynomial, selected, address;

int main(void)
{
    ones = rv_cpop(input);                /* Zbb: three one bits */
    rotated = rv_rol(input, 1);           /* Zbb: wrap the highest bit */
    byte_order = rv_reverse_bytes(input); /* Zbb: endian conversion */
    polynomial = rv_clmul(3, 3);          /* Zbc: XOR multiplication */
    selected = rv_bset(0, 31);            /* Zbs: select one bit */
    address = rv_sh2add(3, 100);          /* Zba: index into 4-byte elements */
    printf("ones=%" PRIu32 " rotated=%" PRIu32 " polynomial=%" PRIu32 " address=%" PRIu32 "\n",
        ones,
        rotated,
        polynomial,
        address);
    return ones == 3 && rotated == 259 && byte_order == 0x81000080u && polynomial == 5 &&
            selected == 0x80000000u && address == 112
        ? 0
        : 1;
}
