
#include <stdlib.h>
#include <string.h>

#include "temperature_sensor.h"

/* Private context */
struct TemperatureSensor {
    ts_adc_read_fn    adc_read;
    RingBufferU16*    rb;
    ts_rb_push_isr_t  RingBuffer_Push;
    float             factor;     /* digits -> C */
    int               initialized;
    system_config_t   cfg_snapshot;
};

/* Derive factor from hardware revision */
static float ts_factor_from_revision(hw_revision_t rev)
{
    switch (rev) 
    {
        case HW_REV_A: return 1.0f;  /* 1 digit = 1.0C  */
        case HW_REV_B: return 0.1f;  /* 10 digits = 1.0C */
        default:       return 1.0f;
    }
}

/* Public API */
status_t TemperatureSensor_Create(TemperatureSensor** out_ts)
{
    if (!out_ts) return STATUS_EINVAL;
    TemperatureSensor* ts = (TemperatureSensor*)calloc(1, sizeof(*ts));
    if (!ts) return STATUS_EIO;
    *out_ts = ts;
    return STATUS_OK;
}

void TemperatureSensor_Destroy(TemperatureSensor* ts)
{
    if (!ts) return;
    free(ts);
}

status_t TemperatureSensor_Init(TemperatureSensor* ts,
                                   ts_adc_read_fn adc_read,
                                   RingBufferU16* rb,
                                   ts_rb_push_isr_t RingBuffer_Push,
                                   const system_config_t* sys_cfg)
{
    if (!ts || !adc_read || !rb || !RingBuffer_Push || !sys_cfg) return STATUS_EINVAL;

    ts->adc_read     = adc_read;
    ts->rb           = rb;
    ts->RingBuffer_Push  = RingBuffer_Push;
    ts->factor       = ts_factor_from_revision(sys_cfg->hw_revision);
    ts->cfg_snapshot = *sys_cfg;
    ts->initialized  = 1;

    return STATUS_OK;
}

void TemperatureSensor_IsrSample(TemperatureSensor* ts)
{
    if (!ts || !ts->initialized) return;
    uint16_t raw;
    
    if (ts->adc_read(&raw) == STATUS_OK) 
    {
        ts->RingBuffer_Push(ts->rb, raw);
    }
}

float TemperatureSensor_ConvertRawToCelsius(const TemperatureSensor* ts, uint16_t raw)
{
    if (!ts || !ts->initialized) return 0.0f;
    return (float)raw * ts->factor;
}

status_t TemperatureSensor_GetCelsius(TemperatureSensor* ts, float* out_celsius)
{
    if (!ts || !out_celsius) return STATUS_EINVAL;
    if (!ts->initialized)    return STATUS_ESTATE;

    uint16_t raw = 0;
    status_t st = ts->adc_read(&raw);
    if (st != STATUS_OK) return st;

    *out_celsius = TemperatureSensor_ConvertRawToCelsius(ts, raw);
    return STATUS_OK;
}