
#include <string.h>
#include <stdio.h>

#include "hal_if.h"
#include "config.h"
#include "temperature_sensor.h"

/* Mocked EEPROM data */
static system_config_t mock_eeprom = 
{
    .hw_revision   = HW_REV_B,
    .serial_number = "ABC1234"
};

/* Mock ADC value */
static uint16_t mock_adc_value = 40;

/* Mock implementations */

status_t hal_i2c_read(uint8_t dev_addr,
                         uint16_t mem_addr,
                         uint8_t* buffer,
                         uint16_t length)
{
    (void)dev_addr;
    if (!buffer) return STATUS_EINVAL;

    const uint8_t* src   = (const uint8_t*)&mock_eeprom;
    const uint16_t total = (uint16_t)sizeof(mock_eeprom);

    /* If read starts beyond end of EEPROM, fill with 0xFF (no valid data) */
    if (mem_addr >= total) 
    {
        memset(buffer, 0xFF, length);
        return STATUS_OK;
    }

    /* Compute how many bytes are actually available */
    uint16_t n = (uint16_t)((mem_addr + length > total) ? (total - mem_addr) : length);
    memcpy(buffer, src + mem_addr, n);
    
    /* If caller requested beyond the end, pad remaining bytes with 0xFF */
    if (n < length) memset(buffer + n, 0xFF, length - n);

    return STATUS_OK;
}

status_t hal_adc_read(uint16_t* out_raw)
{
    if (!out_raw) return STATUS_EINVAL;
    *out_raw = mock_adc_value++;
    return STATUS_OK;
}

status_t hal_gpio_set(uint32_t pin, bool on)
{
    printf("[LED] pin=%u -> %s\n", (unsigned)pin, on == LED_ON ? "ON" : "OFF");
    return STATUS_OK;
}

/* Mock ADC interrupt handler - just call TemperatureSensor ISR */
extern TemperatureSensor* g_TemperatureSensor;  /* declared in main */
void hal_adc_irq_handler(void)
{
    if (g_TemperatureSensor)
        TemperatureSensor_IsrSample(g_TemperatureSensor);
}
