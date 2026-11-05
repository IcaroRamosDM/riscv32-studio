#include <stdio.h>
#include <rv_display.h>

volatile int result;
static char name[32];
static volatile uint8_t screen[32 * 6];

int main(void)
{
    puts("Hello World!");
    printf("Your name: ");
    if (scanf("%31s", name) != 1)
        return 1;
    printf("An integer: ");
    if (scanf("%d", &result) != 1) {
        puts("Expected an integer.");
        return 2;
    }
    printf("Hello, %s! You entered %d.\n", name, result);
    if (rv_display_configure(screen, 32, 6, RV_DISPLAY_TEXT))
        return 3;
    rv_display_text(screen, 32, 6, 0, 0, "Hello World!");
    rv_display_text(screen, 32, 6, 0, 2, name);
    rv_display_text(screen, 32, 6, 0, 4, "View > Graphical display");
    return 0;
}
