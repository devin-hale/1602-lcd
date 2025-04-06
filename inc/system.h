#pragma once

#include <stdint.h>

#define SYSTICK_CLOCK_SPEED ((uint32_t)18750000)
#define SYSTICK_TICK_HZ ((uint32_t)(SYSTICK_CLOCK_SPEED / 1000))

struct systick {
    volatile uint32_t CTRL, LOAD, VAL, CALIB;
};
#define SYSTICK ((struct systick *)0xe000e010)

void _systick();
void systick_init(uint32_t);
void delay(uint32_t ms);
