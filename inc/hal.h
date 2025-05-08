#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "stm32f446xx.h"

#define FREQ 18750000 // CPU frequency, 16 Mhz
#define BIT(x) (1UL << (x))
#define PIN(bank, num) ((((bank) - 'A') << 8) | (num))
#define PINNO(pin) (pin & 255)
#define PINBANK(pin) (pin >> 8)

static inline void spin(volatile uint32_t count) {
    while (count--) asm("nop");
}

#define GPIO(bank) ((GPIO_TypeDef *)(GPIOA_BASE + 0x0400 * (bank)))
enum { GPIO_MODE_INPUT, GPIO_MODE_OUTPUT, GPIO_MODE_AF, GPIO_MODE_ANALOG };

static inline void gpio_set_mode(uint16_t pin, uint8_t mode) {
    GPIO_TypeDef *gpio = GPIO(PINBANK(pin));
    int n = PINNO(pin);
    RCC->AHB1ENR |= BIT(PINBANK(pin));
    gpio->MODER &= ~(3U << (n * 2));
    gpio->MODER |= (mode & 3U) << (n * 2);
}

static inline void gpio_set_pupdr(uint16_t pin, uint8_t mode) {
    GPIO_TypeDef *gpio = GPIO(PINBANK(pin));
    int n = PINNO(pin);
    gpio->PUPDR &= ~(3U << (n * 2));
    gpio->PUPDR |= (mode & 3U) << (n * 2);
}

static inline void gpio_set_otyper(uint16_t pin, bool state) {
    GPIO_TypeDef *gpio = GPIO(PINBANK(pin));
    int n = PINNO(pin);
    gpio->OTYPER &= ~(3U << (n));
    gpio->OTYPER |= ((uint32_t)state << n);
}

static inline void gpio_set_af(uint16_t pin, uint8_t af_num) {
    GPIO_TypeDef *gpio = GPIO(PINBANK(pin)); // GPIO bank
    int n = PINNO(pin);                      // Pin number
    gpio->AFR[n >> 3] &= ~(15UL << ((n & 7) * 4));
    gpio->AFR[n >> 3] |= ((uint32_t)af_num) << ((n & 7) * 4);
}

static inline void gpio_write(uint16_t pin, bool val) {
    GPIO_TypeDef *gpio = GPIO(PINBANK(pin));
    gpio->BSRR = (1U << PINNO(pin)) << (val ? 0 : 16);
}

#define UART1 USART1
#define UART2 USART2
#define UART3 USART3

#ifndef UART_DEBUG
#define UART_DEBUG USART3
#endif

static inline void uart_init(USART_TypeDef *uart, uint32_t baud) {
    uint8_t af = 7;          // Alternate function
    uint16_t rx = 0, tx = 0; // pins

    if (uart == UART1) RCC->APB2ENR |= BIT(4);
    if (uart == UART2) RCC->APB1ENR |= BIT(17);
    if (uart == UART3) RCC->APB1ENR |= BIT(18);

    if (uart == UART1) tx = PIN('A', 9), rx = PIN('A', 10);
    if (uart == UART2) tx = PIN('A', 2), rx = PIN('A', 3);
    if (uart == UART3) tx = PIN('D', 8), rx = PIN('D', 9);

    gpio_set_mode(tx, GPIO_MODE_AF);
    gpio_set_af(tx, af);
    gpio_set_mode(rx, GPIO_MODE_AF);
    gpio_set_af(rx, af);
    uart->CR1 = 0;                          // Disable this UART
    uart->BRR = FREQ / baud;                // FREQ is a UART bus frequency
    uart->CR1 |= BIT(13) | BIT(2) | BIT(3); // Set UE, RE, TE
}

static inline void uart_write_byte(USART_TypeDef *uart, uint8_t byte) {
    uart->DR = byte;
    while ((uart->SR & BIT(7)) == 0) spin(1);
}

static inline void uart_write_buf(USART_TypeDef *uart, char *buf, size_t len) {
    while (len-- > 0) uart_write_byte(uart, *(uint8_t *)buf++);
}

static inline int uart_read_ready(USART_TypeDef *uart) {
    return uart->SR & BIT(5); // If RXNE bit is set, data is ready
}

static inline uint8_t uart_read_byte(USART_TypeDef *uart) {
    return (uint8_t)(uart->DR & 255);
}

#define I2C_1 ((I2C_TypeDef *)I2C1_BASE)

static inline void i2c_init_master(I2C_TypeDef *i2c) {
    uint8_t af = 4;
    uint8_t pupdr = 1; // pull-up

    uint16_t scl = 0, sda = 0;
    if (i2c == I2C_1) {
        RCC->APB2ENR |= BIT(1);
        RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
        scl = PIN('B', 8);
        sda = PIN('B', 9);
    }

    gpio_set_mode(scl, GPIO_MODE_AF);
    gpio_set_af(scl, af);
    gpio_set_pupdr(scl, pupdr);
    gpio_set_otyper(scl, 1);

    gpio_set_mode(sda, GPIO_MODE_AF);
    gpio_set_af(sda, af);
    gpio_set_pupdr(sda, pupdr);
    gpio_set_otyper(sda, 1);

    i2c->CR1 &= ~I2C_CR1_PE;

    i2c->CR2 = 16;
    // scl master mode clock
    // T_high = CCR * T_periph_clock
    // - Target freq is 100KHz, so period is (1/100000), or 10us
    // - T_high + T_low == T, so T_high == 5us
    // Periph clock above is 16mhz, so period is (1/16,000,000)
    // (5e-6)/(6.25e-8) = 80 (0x50)
    i2c->CCR = 0x50;
    // max rise time
    // standard mode == 1000 ns (1000e-9)
    // from reference, MAX_RISE/T_PERIPH_CLOCK = TRISE
    // (1000e-9)/(6.25e-8) = 16 (0x10)
    i2c->TRISE = 0x10;

    i2c->CR1 = I2C_CR1_PE;
}

static inline void i2c_write(I2C_TypeDef *i2c, uint8_t addr, uint8_t *data,
                             uint8_t len) {
    // generate start cond
    i2c->CR1 |= I2C_CR1_START;
    while (!(i2c->SR1 & I2C_SR1_SB)) {};

    i2c->DR = (uint32_t)addr << 1;
    while (!(i2c->SR1 & I2C_SR1_ADDR)) {};
    (void)i2c->SR2;

    for (uint8_t i = 0; i < len; i++) {
        i2c->DR = data[i];
        while (!(i2c->SR1 & I2C_SR1_TXE)) {};
    }

    i2c->CR1 |= I2C_CR1_STOP;
}
