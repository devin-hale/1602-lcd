#include "hal.h"
#include "lcd.h"
#include "led.h"
#include "stm32f446xx.h"
#include "systick.h"
#include "trafficlight.h"

void SystemInit(void) {
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    SysTick_Config(18750000 / 1000);
    return;
}

void startup_with_feedback() {
    led_typedef *blue =
        init_led(LED_PIN_BLUE, LED_MODE_FLASHING_ON, LED_STATE_ON);

    uart_init(UART3, 115200);

    init_led(LED_PIN_GREEN, LED_MODE_FLASHING_ON, LED_STATE_ON);
    led_set_mode(blue, LED_MODE_FLASHING_OFF);
}

int main() {
    startup_with_feedback();
    led_typedef *red = init_led(LED_PIN_RED, LED_MODE_SOLID, LED_STATE_ON);
    (void)red;

    lcd_typedef lcd = {0};
    lcd.addr = 0x27;
    lcd_init(&lcd, I2C_1);

    uint16_t ping = PIN('G', 0);
    uint16_t piny = PIN('D', 1);
    uint16_t pinr = PIN('D', 0);
    tl_init(TL_NORTH, pinr, piny, ping);
    tl_set_default_state(TL_NORTH, TL_STATE_GREEN);
    tl_handle_state(TL_NORTH);
    gpio_write(LED_PIN_RED, false);

    uint32_t timer;
	uint32_t prd = 500;
    uint8_t state = 0;
    while (1) {
        handle_led_states();
        decrement_count(&lcd);
        if (state == 0) {
            if (timer_expired_ms(&timer, prd)) {
                event_buf_push(TL_NORTH, TL_EVENT_TRANSITION_YELLOW);
                tl_handle_next_event(TL_NORTH);
                tl_handle_state(TL_NORTH);
                state = 1;
            }
        }
        if (state == 1) {
            if (timer_expired_ms(&timer, prd)) {
                event_buf_push(TL_NORTH, TL_EVENT_TRANSITION_RED);
                tl_handle_next_event(TL_NORTH);
                tl_handle_state(TL_NORTH);
                state = 2;
            }
        }
        if (state == 2) {
            if (timer_expired_ms(&timer, prd)) {
                event_buf_push(TL_NORTH, TL_EVENT_TRANSITION_GREEN);
                tl_handle_next_event(TL_NORTH);
                tl_handle_state(TL_NORTH);
                state = 0;
            }
        }
    }
    return 0;
}
