#pragma once

#include "hal.h"

#define LED_PIN_GREEN PIN('B', 0)
#define LED_PIN_BLUE PIN('B', 7)
#define LED_PIN_RED PIN('B', 14)

typedef enum {
    LED_STATE_OFF,
    LED_STATE_ON,
} led_state;

typedef enum { LED_MODE_SOLID, LED_MODE_FLASHING_ON, LED_MODE_FLASHING_OFF } led_mode;

typedef struct user_led {
    bool active;
    uint16_t pin;
    led_state state;
    led_mode mode;
    uint32_t flash_rate;
    volatile uint32_t flash_timer;
} user_led;

user_led *init_led(uint16_t, led_mode, led_state);
user_led *get_led(uint16_t);
int led_set_state(user_led *, led_state);
void handle_led_states();
void led_set_mode(user_led *, led_mode);
void led_set_flash_rate(user_led*, uint32_t);
