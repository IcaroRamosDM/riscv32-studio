#include <stdint.h>
volatile uint32_t values[4096];
volatile uint32_t result;
int main(void)
{
    uint32_t total = 0;
    for (uint32_t i = 0; i < 4096; ++i) {
        values[i] = i;
        total += values[i];
    }
    result = total;
    return result == 8386560 ? 0 : 1;
}
