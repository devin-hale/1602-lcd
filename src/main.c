#include "main.h"
#include "gpio.h"
#include <stdint.h>

#define SYSTICK_CLOCK_SPEED ((uint32_t)18750000)

struct systick {
    volatile uint32_t CTRL, LOAD, VAL, CALIB;
};

#define SYSTICK ((struct systick *)0xe000e010)

static inline void systick_init(uint32_t ticks) {
    if ((ticks - 1) > 0xffffff) return;
    SYSTICK->LOAD = ticks - 1;
    SYSTICK->VAL = 0;
    SYSTICK->CTRL = BIT(0) | BIT(1) | BIT(2);
}

static volatile uint32_t s_ticks;
void _systick() { s_ticks++; }

void delay(uint32_t ms) {
    uint32_t until = s_ticks + ms;
    while (s_ticks < until) asm("nop");
}

int main() {
    systick_init(SYSTICK_CLOCK_SPEED / 1000);
    gpio_init(GPIOB, 0, GPIO_MODE_OUTPUT);
    gpio_init(GPIOB, 14, GPIO_MODE_OUTPUT);
    for (;;) {
        gpio_set(GPIOB, 0);
        delay(100);
        gpio_reset(GPIOB, 0);
        delay(100);
    }
    return 0;
}
