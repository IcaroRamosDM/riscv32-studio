#include <rv_uart.h>
#include <stdio.h>
volatile unsigned result;

int main(void)
{
    if (rv_uart_configure(8, RV_UART_EVEN, 1, 1))
        return 1;
    if (rv_uart_putc('B'))
        return 2;
    int byte;
    do {
        byte = rv_uart_getc();
    } while (byte < 0);
    result = (unsigned)byte;
    putchar(byte);
    while (rv_uart_status() & RV_UART_TX_BUSY) {
    }
    return 0;
}
