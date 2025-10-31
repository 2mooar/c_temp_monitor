
#include <stdlib.h>
#include <stdio.h>

#include "temperature_monitor.h"

/* Private context */
struct TemperatureMonitor {
    TemperatureSensor* sensor;
    RingBufferU16*     rb;
    Led*               led_green;
    Led*               led_yellow;
    Led*               led_red;
    int                initialized;
};

/* Core classification logic */
temp_status_t TemperatureMonitor_GetState(float celsius)
{
    if (celsius >= TEMP_CRITICAL_HIGH || celsius < TEMP_CRITICAL_LOW)
        return TEMP_STATUS_CRITICAL;
    if (celsius >= TEMP_WARNING)
        return TEMP_STATUS_WARNING;
    return TEMP_STATUS_NORMAL;
}


/* Internal: drive LEDs if provided (NULL-safe) */
static void tm_apply_leds(TemperatureMonitor* tm, temp_status_t s)
{
    if (!tm) return;

    led_state_t g = LED_OFF; 
    led_state_t y = LED_OFF; 
    led_state_t r = LED_OFF;

    switch (s) 
    {
        case TEMP_STATUS_INIT:
            g = y = r = LED_ON;  /* all on */
            break;
        case TEMP_STATUS_NORMAL:
            g = LED_ON; break;
        case TEMP_STATUS_WARNING:
            y = LED_ON; break;
        case TEMP_STATUS_CRITICAL:
            r = LED_ON; break;
        default:
            break;
    }

    if (tm->led_green)  (void)Led_Set(tm->led_green,  g);
    if (tm->led_yellow) (void)Led_Set(tm->led_yellow, y);
    if (tm->led_red)    (void)Led_Set(tm->led_red,    r);
}

status_t TemperatureMonitor_Create(TemperatureMonitor** out_tm)
{
    if (!out_tm) return STATUS_EINVAL;
    TemperatureMonitor* tm = (TemperatureMonitor*)calloc(1, sizeof(*tm));
    if (!tm) return STATUS_ESTATE;
    *out_tm = tm;
    return STATUS_OK;
}

void TemperatureMonitor_Destroy(TemperatureMonitor* tm)
{
    if (!tm) return;
    free(tm);
}

status_t TemperatureMonitor_Init(TemperatureMonitor* tm,
                                    TemperatureSensor* sensor,
                                    RingBufferU16* rb,
                                    Led* led_green,
                                    Led* led_yellow,
                                    Led* led_red)
{
    if (!tm || !sensor || !rb) return STATUS_EINVAL;
    
    tm->sensor = sensor;
    tm->rb = rb;
    tm->led_green  = led_green;
    tm->led_yellow = led_yellow;
    tm->led_red    = led_red;
    tm->initialized = 1;
    
    /* Initialize LEDs (if provided) to INIT state = all on */
    tm_apply_leds(tm, TEMP_STATUS_INIT);
    
    return STATUS_OK;
}

/* Drain ring buffer and print state */
void TemperatureMonitor_Poll(TemperatureMonitor* tm)
{
    if (!tm || !tm->initialized) return;

    uint16_t raw;
    while (RingBuffer_Pop(tm->rb, &raw)) 
    {
        float c = TemperatureSensor_ConvertRawToCelsius(tm->sensor, raw);
        
        temp_status_t s = TemperatureMonitor_GetState(c);

        
        /* Drive LEDs if available (debug prints are still emitted) */
        tm_apply_leds(tm, s);

        const char* label =
            (s == TEMP_STATUS_CRITICAL) ? "CRITICAL" :
            (s == TEMP_STATUS_WARNING)  ? "WARNING"  : "NORMAL";

        printf("[%s] %.2f C\n", label, c);
    }
}
