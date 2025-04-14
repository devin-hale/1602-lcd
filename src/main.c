#include "hal.h"

static volatile uint32_t s_ticks;
void SysTick_Handler(void) { s_ticks++; }

uint32_t SystemCoreClock = FREQ;
void SystemInit(void) {
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    SysTick_Config(18750000 / 1000);
	return;
}

int main() {
    uint16_t led = PIN('B', 0);
    gpio_set_mode(led, GPIO_MODE_OUTPUT);
    gpio_write(led, true);
    uart_init(UART3, 115200);
    volatile uint32_t timer = 0, period = 500;
    for (;;) {
        if (timer_expired(&timer, period, s_ticks)) {
            static bool on;
            gpio_write(led, on);
            on = !on;
			uart_write_buf(UART_DEBUG, "Hi!\r\n", 5);
            //printf("LED: %d\r\n", on);
        }
    }
    return 0;
}
