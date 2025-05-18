#ifndef LCD_H
#define LCD_H

#include "stm32f446xx.h"

#define LCD_ADDR 0x27

typedef struct lcd_typedef {
    I2C_TypeDef *i2c;
    uint8_t addr;
} lcd_typedef;

void lcd_init(lcd_typedef *, I2C_TypeDef *);
void decrement_count(lcd_typedef *lcd);
void clear_and_write(lcd_typedef *lcd, char *str);

#endif
