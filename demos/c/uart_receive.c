#include <rv_uart.h>
#include <stdio.h>
volatile unsigned ready, result;

int main(void)
{
    if (rv_uart_configure(8, RV_UART_EVEN, 1, 0))
        return 1;
    ready = 1;
    // Pause after ready becomes 1, queue bytes in the UART pane, then resume.
    for (;;) {
        int byte = rv_uart_getc();
        if (byte < 0)
            continue;
        putchar(byte);
        ++result;
        if (byte == '\n')
            break;
    }
    return 0;
}
