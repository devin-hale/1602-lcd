#include <stdint.h>
#include "system.h"
#include "gpio.h"

int main() {
    systick_init(SYSTICK_CLOCK_SPEED / 1000);
    gpio_init(GPIOB, 0, GPIO_MODE_OUTPUT);
    gpio_init(GPIOB, 14, GPIO_MODE_OUTPUT);
    for (;;) {
        gpio_set(GPIOB, 0);
        delay(1000);
        gpio_reset(GPIOB, 0);
        delay(1000);
    }
    return 0;
}
