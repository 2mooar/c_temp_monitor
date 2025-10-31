#ifndef HAL_IF_H
#define HAL_IF_H

#include <stdint.h>
#include <stdbool.h>
#include "config.h" 

/* Core HAL interface — used by higher modules */

/* EEPROM I2C read */
status_t hal_i2c_read(uint8_t dev_addr,
                         uint16_t mem_addr,
                         uint8_t* buffer,
                         uint16_t length);

/* ADC read (for TemperatureSensor) */
status_t hal_adc_read(uint16_t* out_raw);

/* GPIO set (for LED control) */
status_t hal_gpio_set(uint32_t pin, bool on);

/* ISR stub — called by simulated ADC interrupt on PC */
void hal_adc_irq_handler(void);

#endif /* HAL_IF_H */

