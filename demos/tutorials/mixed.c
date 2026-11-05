/* C hands the two inputs to an Assembly instruction through constraints.
 * Step into add_on_cpu at -O0 to see argument registers and the return. */
#include <stdio.h>

volatile unsigned left = 20, right = 22, result;

static unsigned add_on_cpu(unsigned a, unsigned b)
{
    unsigned sum;
    __asm__("add %0, %1, %2" : "=r"(sum) : "r"(a), "r"(b));
    return sum;
}

int main(void)
{
    result = add_on_cpu(left, right);
    printf("20 + 22 = %u\n", result);
    return result == 42 ? 0 : 1;
}
