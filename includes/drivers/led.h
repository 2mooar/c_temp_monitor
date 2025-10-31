#ifndef LED_H
#define LED_H

#include <stdint.h>
#include <stdbool.h>
#include "temperature_sensor.h"   /* reuse status_t */

#ifdef __cplusplus
extern "C" {
#endif

/* Dependency: function to set a GPIO pin to a logical level */
typedef status_t (*led_gpio_set_fn)(uint32_t pin, bool on);

/* Forward declaration */
typedef struct Led Led;

/* Lifecycle */
status_t Led_Create(Led** out_led);
void        Led_Destroy(Led* led);

/* Initialization with injected dependency */
status_t Led_Init(Led* led,
                     led_gpio_set_fn gpio_set,
                     uint32_t pin);

/* Single public control function */
status_t Led_Set(Led* led, led_state_t on);

#ifdef __cplusplus
}
#endif

#endif /* LED_H */
