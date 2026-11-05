#include <stdint.h>
uint32_t values[5] = {3, 6, 9, 12, 15};
volatile uint32_t result;

int main(void)
{
    uint32_t total = 0;
    for (uint32_t i = 0; i < 5; ++i) {
        values[i] += 1;
        total += values[i];
    }
    result = total;
    return result == 50 && values[0] == 4 && values[4] == 16 ? 0 : 1;
}
