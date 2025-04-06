#include "gpio.h"
#include "system.h"
#include <stdint.h>

int main() {
    systick_init(SYSTICK_CLOCK_SPEED / 1000);
    gpio_init(GPIOB, 0, GPIO_MODE_OUTPUT);
    gpio_init(GPIOB, 14, GPIO_MODE_OUTPUT);

    uint32_t exp_timer, period = 1000;
    for (;;) {
        if (timer_expired(&exp_timer, period)) {
            static bool on;
            if (on) {
                gpio_reset(GPIOB, 0);
            } else {
                gpio_set(GPIOB, 0);
            }
			on = !on;
        }
    }
    return 0;
}
