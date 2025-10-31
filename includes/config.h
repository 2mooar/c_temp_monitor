/**
 * @file config.h
 * @brief System configuration and constants
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#include <stdbool.h>


/* Should be power of 2 for better masking */
#define RING_BUFFER_SIZE (256) 

/* Temperature thresholds in Celsius */
#define TEMP_CRITICAL_HIGH  (105.0f)
#define TEMP_CRITICAL_LOW   (5.0f)
#define TEMP_WARNING        (85.0f)

/* Sampling configuration */
#define SAMPLE_INTERVAL_US  (100)  /* 100 microseconds */

/* Hardware revisions */
typedef enum 
{
    HW_REV_A = 0,  /* 1C per digit */
    HW_REV_B = 1   /* 0.1C per digit */
} hw_revision_t;

/* LED states */
typedef enum 
{
    LED_OFF = 0,
    LED_ON = 1
} led_state_t;

/* Temperature status */
typedef enum 
{
    TEMP_STATUS_NORMAL = 0,
    TEMP_STATUS_WARNING = 1,
    TEMP_STATUS_CRITICAL = 2,
    TEMP_STATUS_INIT = 0x7f
} temp_status_t;

/* System configuration structure */
#define SERIAL_NUMBER_LENGTH  (16)  /* includes \0*/
typedef struct 
{
    hw_revision_t hw_revision;
    char serial_number[SERIAL_NUMBER_LENGTH];
} system_config_t;

//Common error types
typedef enum 
{
    STATUS_OK = 0,
    STATUS_EINVAL = 1,
    STATUS_ESTATE = 2,
    STATUS_EIO = 3,
    STATUS_APP_ERROR = 4,
} status_t;

#endif /* CONFIG_H */
