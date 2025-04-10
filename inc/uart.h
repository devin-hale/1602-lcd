#pragma once

#include <stdint.h>

struct uart {
    volatile uint32_t SR, DR, BRR, CR1, CR2, CR3, GTPR;
};

#define UART1 ((struct uart *)0x40011000)
#define UART2 ((struct uart *)0x40004400)
#define UART3 ((struct uart *)0x40004800)

#define FREQ 18000000

void uart_init(struct uart *uart, uint32_t baud);
