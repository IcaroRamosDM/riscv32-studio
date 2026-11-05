#include <stdio.h>
#include <rv_stdio.h>
volatile int result;

int main(void)
{
    result = getchar();
    putchar(result);
    rv_leds(1);
    return 0;
}
