
#include <stdint.h>
#define GPIO 0x40020400
#define AHB1ENR ((volatile uint32_t *)0x40023830)

struct gpio {
  volatile uint32_t MODER, OTYPER, OSPEEDR, PUPDR, IDR, ODR, BSRR, LCKR, AFR[2];
};
#define GPIOB ((volatile struct gpio *)GPIO)

void rcc_init(void) { *AHB1ENR |= 0x2; }
void gpio_init(void) {
  GPIOB->MODER &= ~(0x3);
  GPIOB->MODER |= (0x1);
}

static inline void spin(volatile uint32_t count) {
  while (count--) asm("nop");
}

int main(void) {
  rcc_init();
  gpio_init();
  for(;;) {
    spin(999999);
    GPIOB->ODR &= ~(0x1);
    spin(999999);
    GPIOB->ODR |= 0x1;
  }
  return 0;
}
