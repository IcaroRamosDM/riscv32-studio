#include <stdio.h>
#include <rv_stdio.h>

int main(void)
{
    puts("RISC-V Studio console");
    printf("7 * 6 = %d; hex = %x\n", 7 * 6, 42u);
    rv_leds(0x55);
    printf("Buttons: %u\n", (unsigned)rv_buttons());
    int character;
    // Drain the prequeued fixture. getchar() waits for interactive input instead.
    while ((character = rv_getchar()) != EOF)
        putchar(character);
    putchar('\n');
    rv_leds(0xaa);
    return 0;
}
