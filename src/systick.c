#include "systick.h"
#include <stdbool.h>

static volatile uint32_t now;

void SysTick_Handler(void) { now++; }

uint32_t get_now_ms() { return now; }
uint32_t get_now_us() { return now; }

void delayms(uint32_t ms) {
    uint32_t start = now;
    while (now < (start + (ms))) {};
}

void delayus(uint32_t us) {
    uint32_t start = now;
    while (now < (start + us)) {};
}

bool timer_expired_ms(volatile uint32_t *exp, uint32_t prd) {
    uint32_t prd_us = prd;

    if (now + prd_us < *exp) *exp = 0;
    if (*exp == 0) *exp = now + prd_us;
    if (*exp > now) return false;

    // Has Expired
    if ((now - *exp) > prd_us) {
        *exp = now + prd_us;
    } else {
        *exp = *exp + prd_us;
    }
    return true;
};

bool timer_expired_us(volatile uint32_t *exp, uint32_t prd) {
    if (now + prd < *exp) *exp = 0;
    if (*exp == 0) *exp = now + prd;
    if (*exp > now) return false;

    // Has Expired
    if ((now - *exp) > prd) {
        *exp = now + prd;
    } else {
        *exp = *exp + prd;
    }
    return true;
};
