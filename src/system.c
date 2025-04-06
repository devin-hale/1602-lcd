#include "system.h"
#include "main.h"

inline void systick_init(uint32_t ticks) {
    if ((ticks - 1) > 0xffffff) return;
    SYSTICK->LOAD = ticks - 1;
    SYSTICK->VAL = 0;
    SYSTICK->CTRL = BIT(0) | BIT(1) | BIT(2);
}

volatile uint32_t s_ticks;
void _systick() { s_ticks++; }

void delay(uint32_t ms) {
    uint32_t until = s_ticks + ms;
    while (s_ticks < until) asm("nop");
}

