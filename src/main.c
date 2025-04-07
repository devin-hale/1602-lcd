#include "gpio.h"
#include "system.h"
#include "uart.h"
#include <stdint.h>

int main() {
    systick_init(SYSTICK_TICK_HZ);
    gpio_init(GPIOB, 0, GPIO_MODE_OUTPUT);
	uart_init(UART3, 115200);

    uint32_t exp_timer, period = 500;
    for (;;) {
        if (timer_expired(&exp_timer, period)) {
            static bool on;
            gpio_write(GPIOB, 0, on);
            on = !on;
			uart_write_buf(UART3, "hi\r\n", 4);
        }
    }
    return 0;
}
