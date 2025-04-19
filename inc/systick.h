#pragma once

#include <stdbool.h>
#include <stdint.h>

void SysTick_Handler();
uint32_t get_now();
bool timer_expired(volatile uint32_t *t, uint32_t prd);
void delayms(uint32_t);
