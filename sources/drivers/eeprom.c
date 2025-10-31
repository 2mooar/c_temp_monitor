#include "eeprom.h"
#include <stdlib.h>
#include <string.h>

/* Private context */
struct Eeprom 
{
    i2c_read_fn read_fn;
    uint8_t     i2c_addr;
    int         initialized;
};

status_t Eeprom_Create(Eeprom** out_eeprom)
{
    if (!out_eeprom) return STATUS_EINVAL;
    Eeprom* e = (Eeprom*)calloc(1, sizeof(*e));
    if (!e) return STATUS_EIO;
    *out_eeprom = e;
    return STATUS_OK;
}

void Eeprom_Destroy(Eeprom* eeprom)
{
    if (!eeprom) return;
    free(eeprom);
}

status_t Eeprom_Init(Eeprom* eeprom,
                        i2c_read_fn read_func,
                        uint8_t i2c_address)
{
    if (!eeprom || !read_func) return STATUS_EINVAL;

    eeprom->read_fn     = read_func;
    eeprom->i2c_addr    = i2c_address;
    eeprom->initialized = 1;

    return STATUS_OK;
}

status_t Eeprom_Read(Eeprom* eeprom,
                        uint16_t mem_addr,
                        uint8_t* buffer,
                        uint16_t length)
{
    if (!eeprom || !buffer) return STATUS_EINVAL;
    if (!eeprom->initialized) return STATUS_ESTATE;

    return eeprom->read_fn(eeprom->i2c_addr, mem_addr, buffer, length);
}
