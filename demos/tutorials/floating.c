/* Binary fractions are exact here. Try 0.1 + 0.2 afterward and inspect bits.
 * RV64IFD uses hardware double instructions; rounding flags live in fflags. */
#include <math.h>
#include <stdio.h>

volatile double left = 1.5, right = 2.25, result;

int main(void)
{
    result = fma(left, 2.0, right);
    printf("1.5 * 2 + 2.25 = %.2f\n", result);
    return result == 5.25 ? 0 : 1;
}
