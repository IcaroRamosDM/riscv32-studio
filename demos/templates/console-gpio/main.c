#include <stdio.h>
#include <rv_stdio.h>
volatile unsigned result;

int main(void)
{
    puts("Hello, RISC-V!");
    result = rv_buttons();
    rv_leds(result);
    return 0;
}
