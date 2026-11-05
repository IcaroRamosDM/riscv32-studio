#include <rv_display.h>
static volatile uint8_t cells[32 * 6];

int main(void)
{
    if (rv_display_configure(cells, 32, 6, RV_DISPLAY_TEXT))
        return 1;
    rv_display_text(cells, 32, 6, 2, 1, "RISC-V Studio");
    rv_display_text(cells, 32, 6, 2, 3, "Hello from simulated RAM!");
    rv_display_text(cells, 32, 6, 2, 4, "Try Back and Replay.");
    return 0;
}
