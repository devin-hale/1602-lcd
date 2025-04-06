#include "gpio.h"
#include "system.h"
#include <stdint.h>

int main() {
    systick_init(SYSTICK_TICK_HZ);
    gpio_init(GPIOB, 0, GPIO_MODE_OUTPUT);
    gpio_init(GPIOB, 14, GPIO_MODE_OUTPUT);

    uint32_t exp_timer, period = 500;
    for (;;) {
        if (timer_expired(&exp_timer, period)) {
            static bool on;
            gpio_write(GPIOB, 0, on);
            on = !on;
        }
    }
    return 0;
}
