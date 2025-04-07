#include "gpio.h"
#include "main.h"
#include "rcc.h"

inline void gpio_init(gpio_port pt, uint8_t pin, gpio_mode mode) {
    RCC->AHB1ENR |= (1 << pt);
    struct gpio *gpio = GPIO(pt);
    gpio->mode &= RESET_BANK(pin);
    gpio->mode |= SET_BANK(pin, mode);
}

inline void gpio_set_af(gpio_port pt, uint8_t pin, uint8_t af_mode) {
    struct gpio *gpio = GPIO(pt);
    // pin >> 3 means that all pins under 8 resolve to index 0, and above to
    gpio->afr[pin >> 3] &= ~(0xFU << ((pin & 7U) * 4)); // reset pin
    gpio->afr[pin >> 3] |= ((uint32_t)af_mode) << ((pin & 7) * 4);
}

void gpio_write(gpio_port pt, uint8_t pn, bool state) {
    struct gpio *gpio = GPIO(pt);
    gpio->bsrr |= state ? BIT(pn) : BIT(pn) << 16;
}
