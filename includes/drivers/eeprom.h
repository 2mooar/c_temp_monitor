#ifndef EEPROM_H
#define EEPROM_H

#include <stdint.h>
#include "config.h"   

#ifdef __cplusplus
extern "C" {
#endif


/* low-level I2C read function */
typedef status_t (*i2c_read_fn)(uint8_t dev_addr,
                                   uint16_t mem_addr,
                                   uint8_t* buffer,
                                   uint16_t length);

/* Forward declaration */
typedef struct Eeprom Eeprom;

/* Lifecycle */
status_t Eeprom_Create(Eeprom** out_eeprom);
void Eeprom_Destroy(Eeprom* eeprom);

/**
 * @brief Initialize EEPROM driver.
 *
 * Sets up the EEPROM instance with the given I²C read function and device address.
 *
 * @param eeprom       EEPROM handle
 * @param read_func    Function pointer for low-level I2C read
 * @param i2c_address  I2C device address of the EEPROM
 * @return STATUS_OK on success, error code otherwise
 */
status_t Eeprom_Init(Eeprom* eeprom,
                        i2c_read_fn read_func,
                        uint8_t i2c_address);

/**
 * @brief Read data from EEPROM.
 *
 * Copies `length` bytes starting at `mem_addr` into `buffer`.
 *
 * @param eeprom   EEPROM handle
 * @param mem_addr Start address in EEPROM
 * @param buffer   Destination buffer
 * @param length   Number of bytes to read
 * @return STATUS_OK on success, error code otherwise
 */
status_t Eeprom_Read(Eeprom* eeprom,
                        uint16_t mem_addr,
                        uint8_t* buffer,
                        uint16_t length);

#ifdef __cplusplus
}
#endif

#endif /* EEPROM_H */
