#include <rv_display.h>
volatile unsigned result;
static volatile uint8_t frame[128 * 64 / 8];

int main(void)
{
    if (rv_display_configure(frame, 128, 64, RV_DISPLAY_BITMAP))
        return 1;
    for (unsigned x = 8; x < 120; ++x) {
        rv_display_pixel(frame, 128, 64, x, 8, 1);
        rv_display_pixel(frame, 128, 64, x, 55, 1);
    }
    for (unsigned y = 8; y < 56; ++y) {
        rv_display_pixel(frame, 128, 64, 8, y, 1);
        rv_display_pixel(frame, 128, 64, 119, y, 1);
    }
    for (unsigned x = 20; x < 108; ++x)
        rv_display_pixel(frame, 128, 64, x, 16 + (x - 20) / 3, 1);
    result = 42;
    return 0;
}
