#include "lcd.h"
#include "hal.h"
#include "systick.h"

#define COMMAND 0x00

#define LCD_FUNCTION_SET1 0x33
#define LCD_FUNCTION_SET2 0x32
#define LCD_4BIT_2LINE_MODE 0x28
#define LCD_DISP_CURS_ON 0x0E
#define LCD_DISP_ON_CURS_OFF 0x0C // Display on, cursor off
#define LCD_DISPLAY_OFF 0x08
#define LCD_DISPLAY_ON 0x04
#define LCD_CLEAR_DISPLAY 0x01
#define LCD_ENTRY_MODE_SET 0x04
#define LCD_INCREMENT_CURSER 0x06
#define LCD_SET_ROW1_COL1 0x80 // Force cursor to beginning ( 1st line)
#define LCD_SET_ROW2_COL1 0xC0 // Force cursor to beginning ( 2nd line)
#define LCD_MOVE_DISPLAY_LEFT 0x18
#define LCD_MOVE_DISPLAY_RIGHT 0x1C

#define BACKLIGHT 0x08
#define LCD_EN 0x04

#define FUNCTION_SET 0x20
#define FUNCTION_8D 0x10
#define FUNCTION_4D 0x00

#define FUNCTION_2L 0x08
#define FUNCTION_CHAR_57 0x00

// static void lcd_write(lcd_typedef *, uint8_t);
static void lcd_command(lcd_typedef *lcd, uint8_t cmd);
static void lcd_data(lcd_typedef *lcd, uint8_t data);
static void lcd_write_string(lcd_typedef *lcd, char *str);

void lcd_init(lcd_typedef *lcd, I2C_TypeDef *i2c) {

    i2c_init_master(i2c);
    lcd->i2c = i2c;
    delayms(15);
    lcd_command(lcd, LCD_FUNCTION_SET1);
    lcd_command(lcd, LCD_FUNCTION_SET2);
    lcd_command(lcd, LCD_4BIT_2LINE_MODE);
    lcd_command(lcd, 0x0C);
    lcd_command(lcd, 0x80);
    lcd_write_string(lcd, "hello");
}

// static void lcd_write(lcd_typedef *lcd, uint8_t cmd) {
//     i2c_write(lcd->i2c, lcd->addr, &cmd, 1);
// }

static void lcd_command(lcd_typedef *lcd, uint8_t cmd) {
    uint8_t high = cmd & 0xF0;
    uint8_t low = (cmd << 4) & 0xF0;

    uint8_t hightx[2], lowtx[2];

    hightx[0] = high | BACKLIGHT | LCD_EN;
    hightx[1] = high | BACKLIGHT;

    lowtx[0] = low | BACKLIGHT | LCD_EN;
    lowtx[1] = low | BACKLIGHT;

    i2c_write(lcd->i2c, lcd->addr, hightx, 2);
    delayms(5);
    i2c_write(lcd->i2c, lcd->addr, lowtx, 2);
    delayms(5);
}

static void lcd_data(lcd_typedef *lcd, uint8_t data) {
    uint8_t high = data & 0xF0;
    uint8_t low = (data << 4) & 0xF0;

    uint8_t hightx[2], lowtx[2];

    hightx[0] = high | BACKLIGHT | LCD_EN | 0x01;
    hightx[1] = high | BACKLIGHT | 0x01;

    lowtx[0] = low | BACKLIGHT | LCD_EN | 0x01;
    lowtx[1] = low | BACKLIGHT | 0x01;

    i2c_write(lcd->i2c, lcd->addr, hightx, 2);
    delayms(5);
    i2c_write(lcd->i2c, lcd->addr, lowtx, 2);
    delayms(5);
}

static void lcd_write_string(lcd_typedef *lcd, char *str) {
    while (*str) { lcd_data(lcd, *str++); }
};
