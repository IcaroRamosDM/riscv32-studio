#include "rv_firmware.h"
#include "rv_peripherals.h"
#include "rv_stdio.h"

volatile unsigned completions, adc_value, spi_value, i2c_value, nack_count;

__attribute__((interrupt("machine"), aligned(4))) void peripheral_handler(void)
{
    if (rv_csr_read(mcause) != (RV_MCAUSE_INTERRUPT | 11u)) {
        rv_firmware_exit(1);
        return;
    }
    unsigned pending = rv_peripheral_irq_pending();
    if (pending & RV_IRQ_ADC) {
        adc_value = rv_adc_result();
        ++completions;
    }
    if (pending & RV_IRQ_SPI) {
        spi_value = rv_spi_result();
        ++completions;
    }
    if (pending & RV_IRQ_I2C) {
        i2c_value = rv_i2c_result();
        if (rv_i2c_status() & RV_DEVICE_NACK)
            ++nack_count;
        ++completions;
    }
    rv_peripheral_irq_ack(pending);
}

static void wait_for(unsigned count)
{
    while (completions < count)
        rv_wait_for_interrupt();
}

int main(void)
{
    rv_csr_write(mtvec, (uintptr_t)peripheral_handler);
    rv_peripheral_irq_route(0);
    rv_peripheral_irq_ack(RV_IRQ_ALL);
    rv_peripheral_irq_enable(RV_IRQ_ALL);
    rv_csr_set(mie, 2048);
    rv_csr_set(mstatus, 8);
    rv_adc_configure(32);
    rv_adc_input(0, 1234);
    rv_adc_start(0);
    wait_for(1);
    rv_spi_configure(2, 0, 0);
    rv_spi_reply(0xa5);
    rv_spi_start(0x3c);
    wait_for(2);
    rv_i2c_configure(3);
    rv_i2c_target(0x48, 1);
    rv_i2c_target_register(3, 0x5a);
    rv_i2c_start(0x48, 3, 0, 1);
    wait_for(3);
    rv_i2c_start(0x49, 3, 0, 1);
    wait_for(4); // An absent address also completes, with NACK.
    rv_peripheral_irq_enable(0);
    rv_puts("ADC, SPI and I2C completion interrupts received.\n");
    return adc_value == 1234 && spi_value == 0xa5 && i2c_value == 0x5a && nack_count == 1 ? 0 : 2;
}
