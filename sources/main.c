#include <stdio.h>
#include <string.h>

#if USE_MOCK_HAL
#include <unistd.h> // for sleep only
#endif

#include "config.h"
#include "hal_if.h"
#include "temperature_sensor.h"
#include "temperature_monitor.h"
#include "ring_buffer.h"
#include "eeprom.h"
#include "led.h"


/* TODO: Can be put later into AppContext struct to avoid to much globals */
RingBufferU16 g_RawBufferValues;
TemperatureSensor* g_TemperatureSensor;
TemperatureMonitor* g_TemperatureMonitor;
Led* g_LedRed;
Led* g_LedYellow;
Led* g_LedGreen;

/* Global flag for graceful shutdown */
static volatile bool running = true;

/**
 * @brief Initialize all system components
 */
static status_t system_init(system_config_t *config) 
{
    if(config == NULL)
    {
        return STATUS_EINVAL;
    }
    
    Eeprom* e = NULL;
    if (Eeprom_Create(&e) != STATUS_OK) return 1;

    if (Eeprom_Init(e, hal_i2c_read, 0x50) != STATUS_OK) {
        Eeprom_Destroy(e);
        return STATUS_EIO;
    }

    if (Eeprom_Read(e, 0x0000, (uint8_t*)config, (uint16_t)sizeof(*config)) != STATUS_OK) 
    {
        printf("Error: Eeprom_Read failed");
        Eeprom_Destroy(e);
        return STATUS_EIO;
    }
    
    
    if(!RingBuffer_Init(&g_RawBufferValues))
    {
        printf("Error: RingBuffer init failed");
        return STATUS_EIO;
    }
    
    return STATUS_OK;
}

int main()
{
    printf("*** Temperature Monitor System ***\n");
    
    system_config_t config;
    
    /* Initialize system and buffer for RAW values */
    if (system_init(&config) != STATUS_OK) 
    {
        fprintf(stderr, "System initialization failed\n");
        return STATUS_APP_ERROR;
    }
    
    printf("*** System Ready ***\n");
    printf("Serial Number: %s\n", config.serial_number);
    printf("Hardware Revision: %s\n", config.hw_revision == HW_REV_A ? "Rev-A (1C/digit)" : "Rev-B (0.1C/digit)");
    
    /* Init Temperature Sensor*/
    if (TemperatureSensor_Create(&g_TemperatureSensor) != STATUS_OK) return STATUS_APP_ERROR;
    if (TemperatureSensor_Init(g_TemperatureSensor, hal_adc_read, &g_RawBufferValues, RingBuffer_Push, &config) != STATUS_OK) 
    {
        fprintf(stderr, "TemperatureSensor failed\n");
        TemperatureSensor_Destroy(g_TemperatureSensor);
        return STATUS_APP_ERROR;
    }
    
    /* Init Leds */ 
    Led_Create(&g_LedRed); 
    Led_Create(&g_LedYellow); 
    Led_Create(&g_LedGreen);
    
    Led_Init(g_LedGreen, hal_gpio_set, 10);
    Led_Init(g_LedYellow, hal_gpio_set, 11);
    Led_Init(g_LedRed, hal_gpio_set, 12);
    
    /* Create Temperature Monitor */
    if (TemperatureMonitor_Create(&g_TemperatureMonitor) != STATUS_OK) return STATUS_APP_ERROR;
    if (TemperatureMonitor_Init(g_TemperatureMonitor, g_TemperatureSensor, &g_RawBufferValues, g_LedGreen, g_LedYellow, g_LedRed) != STATUS_OK)
    {
        fprintf(stderr, "TemperatureMonitor failed\n");
        TemperatureMonitor_Destroy(g_TemperatureMonitor);
        return STATUS_APP_ERROR;
    }

    /* main loop */
    while (running) 
    {
        #if USE_MOCK_HAL
        /* Simulate ISR call (in real hardware, this would be timer-triggered) */
        hal_adc_irq_handler();
        #endif
         
        TemperatureMonitor_Poll(g_TemperatureMonitor);
        
        #if USE_MOCK_HAL
        /* ... sleep/yield ... */
        sleep(1);
        #endif
    }


    /* Destroy everything */
    TemperatureSensor_Destroy(g_TemperatureSensor);
    TemperatureMonitor_Destroy(g_TemperatureMonitor);
    
    Led_Destroy(g_LedGreen);
    Led_Destroy(g_LedYellow);
    Led_Destroy(g_LedRed);
     

    return STATUS_OK;
}
