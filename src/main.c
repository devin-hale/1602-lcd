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
        GPIO(GPIOB)->odr &= ~(0x1U);
        GPIO(GPIOB)->odr &= ~(0x1U << 14);
        spin(999999);
        GPIO(GPIOB)->odr |= (0x1U);
        GPIO(GPIOB)->odr |= (0x1U << 14);
    }
    return 0;
}
