#pragma once

#include "hal.h"

#define LED_PIN_GREEN PIN('B', 0)
#define LED_PIN_BLUE PIN('B', 7)
#define LED_PIN_RED PIN('B', 14)

typedef enum {
    LED_STATE_OFF,
    LED_STATE_ON,
} led_state;

typedef enum {
    LED_MODE_SOLID,
    LED_MODE_FLASHING_ON,
    LED_MODE_FLASHING_OFF
} led_mode;

typedef struct led_typedef {
    bool active;
    uint16_t pin;
    led_state state;
    led_mode mode;
    uint32_t flash_rate;
    volatile uint32_t flash_timer;
} led_typedef;

void handle_led_states();
led_typedef *init_led(uint16_t, led_mode, led_state);
led_typedef *get_led(uint16_t);
int led_set_state(led_typedef *, led_state);
void led_set_mode(led_typedef *, led_mode);
void led_set_flash_rate(led_typedef *, uint32_t);
