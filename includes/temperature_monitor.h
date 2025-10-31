#ifndef TEMPERATURE_MONITOR_H
#define TEMPERATURE_MONITOR_H

#include <stdint.h>
#include "config.h"
#include "ring_buffer.h"
#include "temperature_sensor.h"
#include "led.h" 


/* Forward declaration */
typedef struct TemperatureMonitor TemperatureMonitor;

/* Lifecycle */
status_t TemperatureMonitor_Create(TemperatureMonitor** out_tm);
void       TemperatureMonitor_Destroy(TemperatureMonitor* tm);

/* Initialization */
status_t TemperatureMonitor_Init(TemperatureMonitor* tm,
                                    TemperatureSensor* sensor,
                                    RingBufferU16* rb,
                                    Led* led_green,
                                    Led* led_yellow,
                                    Led* led_red);

/* Poll ring buffer: read, convert, and print */
void TemperatureMonitor_Poll(TemperatureMonitor* tm);

/* Classify a temperature (Celsius) into state */
temp_status_t TemperatureMonitor_GetState(float celsius);

#endif /* TEMPERATURE_MONITOR_H */
