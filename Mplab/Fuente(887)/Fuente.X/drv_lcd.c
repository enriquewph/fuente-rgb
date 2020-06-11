#include	<stdio.h>
#include	"drv_lcd.h"

/* Organiza el byte a escribir segun como definimos las conexiones */
void Lcd_Port(char a) {
    if (a & 1) {
        LCD_D4 = 1;
    } else {
        LCD_D4 = 0;
    }

    if (a & 2) {
        LCD_D5 = 1;
    } else {
        LCD_D5 = 0;
    }

    if (a & 4) {
        LCD_D6 = 1;
    } else {
        LCD_D6 = 0;
    }

    if (a & 8) {
        LCD_D7 = 1;
    } else {
        LCD_D7 = 0;
    }
}

/* Escribe un byte en el LCD en modo de 4-bits */
void lcd_write(unsigned char c) {
    Lcd_Port(c >> 4);

    __delay_us(5);

    LCD_EN = 1;

    __delay_us(5);

    LCD_EN = 0;

    Lcd_Port(c & 0x0F);

    __delay_us(5);

    LCD_EN = 1;

    __delay_us(5);

    LCD_EN = 0;
    __delay_us(200);
}

/* Borra el LCD y setea el cursor a la dir. 0 */
void lcd_clear(void) {
    LCD_RS = 0;
    lcd_write(0x1);
    __delay_ms(5);
}

char lcd_print(const char * s) {
    unsigned char cs = 0;
    while (*s) {
        lcd_putch(*s++);
        cs++;
    }
    return cs;
}

void lcd_putch(char c) {
    LCD_RS = 1;
    lcd_write(c);
}

void lcd_setCursor(unsigned int columna, unsigned int fila) {
    LCD_RS = 0;
    if (fila == 0) {
        lcd_write(0x80 + columna);
    } else if (fila == 1) {
        lcd_write(0xC0 + columna);
    }
}

void lcd_init(void) {
    __delay_ms(100);

    Lcd_Port(0x2);

    LCD_EN = 1;
    __delay_us(2);

    LCD_EN = 0;
    __delay_us(2);

    Lcd_Port(0xC);

    LCD_EN = 1;
    __delay_us(2);
    LCD_EN = 0;

    __delay_us(50);
    lcd_write(0xC); // Display ON, cursor off, blink off

    __delay_us(50);
    lcd_write(0x1); // Display clear

    __delay_ms(2);
    lcd_write(0x6); // entry mode
}
