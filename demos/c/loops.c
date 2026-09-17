#include <stdint.h>
volatile uint32_t limit = 10;
volatile uint32_t result;
int main(void)
{
    uint32_t total = 0;
    for (uint32_t i = 1; i <= limit; ++i) total += i;
    result = total;
    return result == 55 ? 0 : 1;
}
