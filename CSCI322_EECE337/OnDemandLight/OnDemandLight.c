#include <stdio.h>
#include "pico/stdlib.h"

#define SIO 0xd0000000
int32_t *SetReg = (int32_t *)(SIO|0x14);
int32_t *ClrReg = (int32_t *)(SIO|0x18);

#define PICO_LED_PIN 7
#define PICO_BUTTON_PIN 10

int pico_led_init()
{
    gpio_init(PICO_LED_PIN);
    gpio_set_dir(PICO_LED_PIN, GPIO_OUT);
    return PICO_OK;
}

void pico_set_led(bool button_push)
{
    int32_t *SetReg=(int32_t *)(SIO|0x14);
    int32_t *ClrReg=(int32_t *)(SIO|0x18);
    if(button_push)
    {
        *SetReg=1<<PICO_LED_PIN;
    }
    else
    {
        *ClrReg=1<<PICO_LED_PIN;
    }
}

int pico_button_init()
{
    gpio_init(PICO_BUTTON_PIN);
    gpio_set_dir(PICO_LED_PIN, GPIO_IN);
    return PICO_OK;
}

bool pico_get_button()
{
    while(true)
    {
        if(gpio_get(PICO_BUTTON_PIN) == 1)
        {
            return 1;
        }
        else
        {
            return 0;
        }
        sleep_ms(100);
    }
}

int main()
{
    int rc = pico_led_init();
    hard_assert(rc == PICO_OK);
    while(true)
    {
        pico_set_led(pico_get_button());
        sleep_ms(50);
        pico_set_led(pico_get_button());
        sleep_ms(50);
    }
}
