#include <stdio.h>
#include "pico/stdlib.h"

#define SIO 0xd0000000
#define IO_ADDR 0x40014000

int32_t *SetReg = (int32_t *)(SIO|0x14);
int32_t *ClrReg = (int32_t *)(SIO|0x18);
int32_t *IO_GPIO7 = (int32_t *)(IO_ADDR|0x3c);
int32_t *IO_GPIO10 = (int32_t *)(IO_ADDR|0x54);
int32_t *SetOE = (int32_t *)(SIO|0x24);
int32_t *In = (int32_t *)(SIO|0x04);

#define PICO_LED_PIN 7

int pico_led_init()
{
    *IO_GPIO7 = 0x5;
    *SetOE = 1<<7;
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

void initButton()
{
    *IO_GPIO10 = 0x5;
    *SetOE =~(1<<10);
}

bool getButton()
{
    int value = *In;
    return (value & (1<<10)) != 0;
}

void Our_sleep_ms(int ms)
{
    for(int j = 0; j < ms; j++)
    {
        for(int i = 0; i < 30000; i++) // Make this 1 ms delay
        {
            __asm__("NOP;");
        }
    }
}

int main()
{
    int rc = pico_led_init();
    initButton();
    hard_assert(rc == PICO_OK);
    while(true)
    {
        pico_set_led(getButton());
        Our_sleep_ms(50);
        pico_set_led(getButton());
        Our_sleep_ms(50);
    }
}
