/* Step over the assignments, inspect Symbols > values / result, then Back.
 * Volatile keeps the stores visible even in an optimized build. */
#include <stdio.h>

volatile unsigned values[3] = {10, 20, 12};
volatile unsigned result;

int main(void)
{
    result = values[0];
    result += values[1];
    result += values[2];
    printf("result = %u\n", result);
    return result == 42 ? 0 : 1;
}
