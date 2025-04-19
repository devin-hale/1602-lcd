#include "hal.h"
#include "led.h"

void SystemInit(void) {
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    SysTick_Config(18750000 / 1000);
    return;
}

void startup_with_feedback() {
    user_led* blue = init_led(LED_PIN_BLUE, LED_MODE_FLASHING_ON, LED_STATE_ON);
	init_led(LED_PIN_RED, LED_MODE_SOLID, LED_STATE_OFF);

    uart_init(UART3, 115200);

    init_led(LED_PIN_GREEN, LED_MODE_SOLID, LED_STATE_ON);
	led_set_mode(blue, LED_MODE_FLASHING_OFF);
}

int main() {
	startup_with_feedback();
    while (1) { handle_led_states(); }
    //    for (;;) {
    //
    //        if (timer_expired(&timer, period, s_ticks)) {
    //            static bool on;
    //            gpio_write(led, on);
    //            on = !on;
    //			uart_write_buf(UART_DEBUG, "Hi!\r\n", 5);
    //            //printf("LED: %d\r\n", on);
    //        }
    //    }
    return 0;
}
