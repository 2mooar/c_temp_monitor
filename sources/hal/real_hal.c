#include "hal_if.h"

/* Stubs for target build — implement later with real drivers */

status_t hal_i2c_read(uint8_t dev_addr,
                         uint16_t mem_addr,
                         uint8_t* buffer,
                         uint16_t length)
{
    (void)dev_addr; (void)mem_addr; (void)buffer; (void)length;
    return STATUS_EIO; /* not implemented */
}

status_t hal_adc_read(uint16_t* out_raw)
{
    (void)out_raw;
    return STATUS_EIO; /* not implemented */
}

status_t hal_gpio_set(uint32_t pin, bool on)
{
    (void)pin; (void)on;
    return STATUS_OK; /* no-op */
}

void hal_adc_irq_handler(void)
{
    /* will call real ISR logic later */
}
