#include "uart.h"
#include "gpio.h"
#include "main.h"
#include "rcc.h"
#include <stdint.h>

void uart_init(struct uart *uart, uint32_t baud) {
    uint8_t af = 7;
    gpio_port gpio;
    uint8_t rx_pin = 0;
    uint8_t tx_pin = 0;

    if (uart == UART1) {
        RCC->APB2ENR |= BIT(4);
        gpio = GPIOA;
        tx_pin = 9;
        rx_pin = 10;
    }
    if (uart == UART2) {
        RCC->APB1ENR |= BIT(17);
        gpio = GPIOA;
        tx_pin = 2;
        rx_pin = 3;
    }
    if (uart == UART3) {
        RCC->APB1ENR |= BIT(18);
        gpio = GPIOD;
        tx_pin = 8;
        rx_pin = 9;
    }

    gpio_init(gpio, tx_pin, GPIO_MODE_AF);
    gpio_set_af(gpio, tx_pin, af);
    gpio_init(gpio, rx_pin, GPIO_MODE_AF);
    gpio_set_af(gpio, rx_pin, af);

    uart->CR1 = 0;
    uart->BRR = FREQ / baud;
    uart->CR1 |= BIT(13) | BIT(2) | BIT(3);
};
