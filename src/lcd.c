#include "lcd.h"
#include "hal.h"
#include "systick.h"

#define COMMAND 0x00

#define FUNCTION_SET 0x20
#define FUNCTION_8D 0x10
#define FUNCTION_4D 0x00

#define FUNCTION_2L 0x08
#define FUNCTION_CHAR_57 0x00

static void lcd_write(lcd_typedef *, uint8_t);

void lcd_init(lcd_typedef *lcd, I2C_TypeDef *i2c) {

    i2c_init_master(i2c);
    lcd->i2c = i2c;

    uint8_t instr = FUNCTION_SET | FUNCTION_8D | FUNCTION_2L | FUNCTION_CHAR_57;

    delayms(15);
    lcd_write(lcd, instr);
    delayms(5);
    lcd_write(lcd, instr);
    delayms(1);
    lcd_write(lcd, instr);
}

static void lcd_write(lcd_typedef *lcd, uint8_t cmd) {
    i2c_write(lcd->i2c, lcd->addr, &cmd, 1);
}
