#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

#include <stdint.h>
#include "config.h"
#include "ring_buffer.h"

#ifdef __cplusplus
extern "C" {
#endif



/* Function to read raw ADC */
typedef status_t (*ts_adc_read_fn)(uint16_t* out_raw);

/* Exact ring buffer push signature*/
typedef void (*ts_rb_push_isr_t)(RingBufferU16* rb, uint16_t v);

/* Forward declaration */
typedef struct TemperatureSensor TemperatureSensor;

/* Lifecycle */
status_t TemperatureSensor_Create(TemperatureSensor** out_ts);

void TemperatureSensor_Destroy(TemperatureSensor* ts);

status_t TemperatureSensor_Init(TemperatureSensor* ts,
                                   ts_adc_read_fn adc_read,
                                   RingBufferU16* rb,
                                   ts_rb_push_isr_t rb_push_isr,
                                   const system_config_t* sys_cfg);

/* Ultra-fast ISR sampling: read raw and push to RB */
void TemperatureSensor_IsrSample(TemperatureSensor* ts);

/* Helpers (non-ISR) */
float       TemperatureSensor_ConvertRawToCelsius(const TemperatureSensor* ts, uint16_t raw);

status_t TemperatureSensor_GetCelsius(TemperatureSensor* ts, float* out_celsius);


#ifdef __cplusplus
}
#endif

#endif /* TEMPERATURE_SENSOR_H */
