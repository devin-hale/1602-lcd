#pragma once

#include <stdbool.h>
#include <stdint.h>

void SysTick_Handler();
uint32_t get_now_ms();
uint32_t get_now_us();
bool timer_expired_ms(volatile uint32_t *t, uint32_t prd);
bool timer_expired_us(volatile uint32_t *t, uint32_t prd);
void delayms(uint32_t);
void delayus(uint32_t);
