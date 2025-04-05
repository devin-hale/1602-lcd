#include "gpio.h"
#include <stdint.h>
#define AHB1ENR ((volatile uint32_t *)0x40023830)

#include <stdint.h>

void rcc_init() { *AHB1ENR |= 0x2; }

static inline void spin(volatile uint32_t count) {
    while (count--) asm("nop");
}

int main() {
    gpio_init(GPIOB, 0, GPIO_MODE_OUTPUT);
    gpio_init(GPIOB, 14, GPIO_MODE_OUTPUT);
    for (;;) {
        spin(999999);
		gpio_set(GPIOB, 0);
        spin(999999);
		gpio_reset(GPIOB, 0);
    }
    return 0;
}
