#include "hal.h"
#include "lcd.h"
#include "led.h"
#include "stm32f446xx.h"
#include "trafficlight.h"

#define CLOCK_HZ 18750000

void SystemInit(void) {
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    SysTick_Config(CLOCK_HZ / 1000);
    return;
}

int main() {
    uart_init(UART3, 115200);
    init_led(LED_PIN_GREEN, LED_MODE_FLASHING_ON, LED_STATE_ON);

    led_typedef *red = init_led(LED_PIN_RED, LED_MODE_SOLID, LED_STATE_ON);
    (void)red;

    lcd_typedef lcd = {.addr = 0x27};
    lcd_init(&lcd, I2C_1);

    struct traffic_light tl;
    uint16_t ping = PIN('G', 0);
    uint16_t piny = PIN('D', 1);
    uint16_t pinr = PIN('D', 0);
    tl_init(&tl, pinr, piny, ping);
    tl_set_default_state(&tl, TL_STATE_GREEN);
    tl_handle_state(&tl);

    gpio_write(LED_PIN_RED, false);

    while (1) {
        handle_led_states();
        decrement_count(&lcd);
    }
    return 0;
}
