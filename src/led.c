#include "led.h"
#include "hal.h"
#include "systick.h"

#define USER_LED_MAX 3
#define DEFAULT_FLASH_RATE 1000 // ms

static user_led led_buf[USER_LED_MAX];
static uint8_t led_buf_count = 0;

user_led *get_led(uint16_t pin) {
    for (int i = 0; i < USER_LED_MAX; i++) {
        if (led_buf[i].pin == pin) return &led_buf[i];
    }
    return NULL;
};

user_led *init_led(uint16_t pin, led_mode m, led_state s) {
    if (get_led(pin) != NULL) return get_led(pin);
    if (led_buf_count >= USER_LED_MAX) return NULL;
    user_led *led = &led_buf[led_buf_count++];
    led->active = true;
	led->mode = m;
    led->flash_rate = DEFAULT_FLASH_RATE;
    led->pin = pin;
	led->state = LED_STATE_OFF;
    gpio_set_mode(pin, GPIO_MODE_OUTPUT);
	led_set_state(led, s);
    return led;
}

void led_set_mode(user_led *led, led_mode m) { led->mode = m; }

int led_set_state(user_led *led, led_state state) {
    if (led->active) {
        if (led->state == state) return 1;
        led->state = state;
        gpio_write(led->pin, (bool)state);
        return 1;
    }
    return 0;
}

int led_state_toggle(user_led *led) {
    if (led->active) {
        if (led->state == LED_STATE_ON)
            return led_set_state(led, LED_STATE_OFF);
        if (led->state == LED_STATE_OFF)
            return led_set_state(led, LED_STATE_ON);
    }
    return 0;
}

void led_set_flash_rate(user_led* led, uint32_t ms) {
	led->flash_rate = ms;
}

int handle_flashing(user_led *led) {
    if (led->active) {
        if (timer_expired_ms(&led->flash_timer, led->flash_rate)) {
            led_state_toggle(led);
        }
    }
    return -1;
}

void handle_led_states() {
    for (int i = 0; i < led_buf_count; i++) {
        user_led *led = &led_buf[i];
        if (led->active) {
            switch (led->mode) {
            case LED_MODE_SOLID:
                break;
            case LED_MODE_FLASHING_ON:
                handle_flashing(led);
                break;
			case LED_MODE_FLASHING_OFF:
				if(led->state == LED_STATE_ON) led_set_state(led, LED_STATE_OFF);
				break;
            }
        }
    }
}
