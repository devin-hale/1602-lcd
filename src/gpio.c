#include "main.h"
#include "gpio.h"
#include "rcc.h"

inline void gpio_init(gpio_port port, uint8_t pin, gpio_mode mode) {
    RCC->AHB1ENR |= (1 << port);
    struct gpio *gpio = GPIO(port);
    gpio->mode &= RESET_BANK(pin);
    gpio->mode |= SET_BANK(pin, mode);
}

inline void gpio_set(gpio_port port, uint8_t pin) {
	struct gpio *gpio = GPIO(port);
	gpio->bsrr |= BIT(pin);
}

inline void gpio_reset(gpio_port port, uint8_t pin) {
	struct gpio *gpio = GPIO(port);
	gpio->bsrr |= BIT(pin) << 16;
}
