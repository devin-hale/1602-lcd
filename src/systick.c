#include "systick.h"
#include <stdbool.h>

static volatile uint32_t now;

void SysTick_Handler(void) { now++; }

uint32_t get_now() { return now; }
void delayms(uint32_t ms) {
    uint32_t start = now;
    while (now < (start + ms)) {};
}

bool timer_expired(volatile uint32_t *exp, uint32_t prd) {
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
