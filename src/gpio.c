#include "gpio.h"
#include "main.h"
#include "rcc.h"

inline void gpio_init(gpio_port pt, uint8_t pin, gpio_mode mode) {
    RCC->AHB1ENR |= (1 << pt);
    struct gpio *gpio = GPIO(pt);
    gpio->mode &= RESET_BANK(pin);
    gpio->mode |= SET_BANK(pin, mode);
}

void gpio_write(gpio_port pt, uint8_t pn, bool state) {
	struct gpio *gpio = GPIO(pt);
    gpio->bsrr |= state ? BIT(pn) : BIT(pn) << 16;
}
