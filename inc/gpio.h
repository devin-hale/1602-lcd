#pragma once

#include <stdbool.h>
#include <stdint.h>

#define GPIO_BASE 0x40020000

#define RESET_BANK(pin) (~(0x3U << (pin) * 2))
#define SET_BANK(pin, val) (((val) & 0x3U) << (pin) * 2)

struct gpio {
    volatile uint32_t mode, otype, ospeed, pupd, idr, odr, bsrr, lckr, afr[2];
};
#define GPIO(port) ((struct gpio *)((port) * 0x400 + GPIO_BASE))

typedef enum {
    GPIOA,
    GPIOB,
    GPIOC,
    GPIOD,
    GPIOE,
    GPIOF,
    GPIOG,
    GPIOH
} gpio_port;

typedef enum {
    GPIO_MODE_INPUT,
    GPIO_MODE_OUTPUT,
    GPIO_MODE_AF,
    GPIO_MODE_ANALOG
} gpio_mode;

void gpio_init(gpio_port pt, uint8_t pn, gpio_mode m);
void gpio_write(gpio_port pt, uint8_t pn, bool state);
