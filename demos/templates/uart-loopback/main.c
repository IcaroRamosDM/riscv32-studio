#include <rv_uart.h>
volatile unsigned result;

int main(void)
{
    if (rv_uart_configure(8, RV_UART_EVEN, 1, 1))
        return 1;
    if (rv_uart_putc(42))
        return 2;
    int byte;
    do {
        byte = rv_uart_getc();
    } while (byte < 0);
    result = (unsigned)byte;
    while (rv_uart_status() & RV_UART_TX_BUSY) {
    }
    return byte == 42 ? 0 : 3;
}
