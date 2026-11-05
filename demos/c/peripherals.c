#include <rv_peripherals.h>
#include <rv_stdio.h>
volatile unsigned adc_value, spi_value, i2c_value, i2c_missing;

int main(void)
{
    if (rv_pwm_configure(32, 12) || rv_adc_configure(64) || rv_adc_input(2, 3072) ||
        rv_adc_start(2))
        return 1;
    while (rv_adc_status() & RV_DEVICE_BUSY) {
    }
    adc_value = rv_adc_result();
    if (rv_spi_configure(3, 3, 1) || rv_spi_start(0xa5))
        return 2;
    while (rv_spi_status() & RV_DEVICE_BUSY) {
    }
    spi_value = rv_spi_result();
    if (rv_i2c_configure(4) || rv_i2c_target(0x48, 1) || rv_i2c_start(0x48, 3, 0x5a, 0))
        return 3;
    while (rv_i2c_status() & RV_DEVICE_BUSY) {
    }
    if (rv_i2c_status() & RV_DEVICE_NACK)
        return 4;
    if (rv_i2c_start(0x48, 3, 0, 1))
        return 5;
    while (rv_i2c_status() & RV_DEVICE_BUSY) {
    }
    i2c_value = rv_i2c_result();
    if (rv_i2c_start(0x49, 3, 0, 1))
        return 6;
    while (rv_i2c_status() & RV_DEVICE_BUSY) {
    }
    i2c_missing = !!(rv_i2c_status() & RV_DEVICE_NACK);
    rv_putchar('O');
    rv_putchar('K');
    rv_putchar('\n');
    return adc_value != 3072 || spi_value != 0xa5 || i2c_value != 0x5a || !i2c_missing;
}
