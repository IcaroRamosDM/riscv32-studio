#include <rv_uart.h>
#include <stdio.h>
volatile unsigned result;

int main(void)
{
    static const char text[] = "UART works!";
    if (rv_uart_configure(8, RV_UART_EVEN, 1, 1))
        return 1;
    for (unsigned i = 0; text[i]; ++i) {
        if (rv_uart_putc((unsigned char)text[i]))
            return 2;
        int byte;
        do {
            byte = rv_uart_getc();
        } while (byte < 0);
        if (byte != (unsigned char)text[i])
            return 3;
        putchar(byte);
        ++result;
    }
    while (rv_uart_status() & RV_UART_TX_BUSY) {
    }
    return 0;
}
