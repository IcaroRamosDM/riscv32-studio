/* View > Panels > Graphical display shows pixels stored in simulated RAM. */
#include <rv_display.h>
#include <stdio.h>

static volatile uint8_t pixels[64 * 32 / 8];

int main(void)
{
    if (rv_display_configure(pixels, 64, 32, RV_DISPLAY_BITMAP))
        return 1;
    for (unsigned x = 0; x < 64; ++x)
        rv_display_pixel(pixels, 64, 32, x, x / 2, 1);
    puts("64 pixels drawn. Open Graphical display.");
    return 0;
}
