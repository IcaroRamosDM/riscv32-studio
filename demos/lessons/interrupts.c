#include <rv_firmware.h>
#include <rv_stdio.h>
#define TARGET 2
volatile unsigned interrupt_count;

__attribute__((interrupt("machine"), aligned(4))) void timer_handler(void)
{
    if (rv_csr_read(mcause) != (RV_MCAUSE_INTERRUPT | 7u)) {
        rv_firmware_exit(1);
        return;
    }
    ++interrupt_count;
    rv_putchar('0' + (int)interrupt_count);
    if (interrupt_count < TARGET)
        rv_timer_after(300);
    else
        rv_csr_clear(mie, 128);
}

int main(void)
{
    rv_csr_write(mtvec, (uintptr_t)timer_handler);
    rv_timer_after(300);
    rv_csr_set(mie, 128);
    rv_csr_set(mstatus, 8);
    while (interrupt_count < TARGET)
        rv_wait_for_interrupt();
    rv_putchar('\n');
    return 0;
}
