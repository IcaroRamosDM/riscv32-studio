#include <stdint.h>
volatile uint32_t input = 5, result;
static uint32_t factorial(uint32_t n)
{
    if (n < 2) return 1;
    return n * factorial(n - 1);
}
int main(void)
{
    result = factorial(input);
    return result == 120 ? 0 : 1;
}
