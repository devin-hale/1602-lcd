#include "hal.h"
#include "lcd.h"
#include "led.h"
#include "stm32f446xx.h"

void SystemInit(void) {
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    SysTick_Config(18750000 / 1000);
    return;
}

void startup_with_feedback() {
    user_led *blue = init_led(LED_PIN_BLUE, LED_MODE_FLASHING_ON, LED_STATE_ON);
    led_set_flash_rate(blue, 100);
    init_led(LED_PIN_RED, LED_MODE_SOLID, LED_STATE_OFF);

    uart_init(UART3, 115200);

    init_led(LED_PIN_GREEN, LED_MODE_FLASHING_ON, LED_STATE_ON);
    led_set_mode(blue, LED_MODE_FLASHING_OFF);
}

int main() {
    startup_with_feedback();

    lcd_typedef lcd = {};
    lcd.addr = 0x27;
    lcd_init(&lcd, I2C_1);

    while (1) { 
		handle_led_states(); 
		decrement_count(&lcd);
	}
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
