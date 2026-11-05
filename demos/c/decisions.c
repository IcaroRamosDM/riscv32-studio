#include <stdint.h>
volatile int32_t input = -3;
volatile uint32_t signed_less, unsigned_less, result;

int main(void)
{
    signed_less = input < 2;
    unsigned_less = (uint32_t)input < UINT32_C(2);
    if (input < 0)
        result = 100;
    else
        result = 200;
    return signed_less == 1 && unsigned_less == 0 && result == 100 ? 0 : 1;
}
