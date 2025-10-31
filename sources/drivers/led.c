#include <stdlib.h>

#include "led.h"


/* Private context */
struct Led {
    led_gpio_set_fn gpio_set;
    uint32_t        pin;
    int             initialized;
};

status_t Led_Create(Led** out_led)
{
    if (!out_led) return STATUS_EINVAL;

    Led* led = (Led*)calloc(1, sizeof(*led));
    if (!led) return STATUS_EIO;

    *out_led = led;
    return STATUS_OK;
}

void Led_Destroy(Led* led)
{
    if (!led) return;
    free(led);
}

status_t Led_Init(Led* led,
                     led_gpio_set_fn gpio_set,
                     uint32_t pin)
{
    if (!led || !gpio_set) return STATUS_EINVAL;

    led->gpio_set = gpio_set;
    led->pin      = pin;
    led->initialized = 1;

    /* Drive LED off initially */
    led->gpio_set(pin, false);
    return STATUS_OK;
}

status_t Led_Set(Led* led, led_state_t on)
{
    if (!led) return STATUS_EINVAL;
    if (!led->initialized) return STATUS_ESTATE;

    return led->gpio_set(led->pin, on);
}
