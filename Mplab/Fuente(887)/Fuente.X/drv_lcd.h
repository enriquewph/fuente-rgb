#ifndef _LCD_H
#define _LCD_H
#include	<xc.h>

void lcd_write( unsigned char );		/* Escribe 1 byte en el LCD en modo de 4 bits */

void lcd_putch( char c );

char lcd_print( const char * s);         /* Escribe una cadena de texto en el LCD */

void lcd_clear(void);					/* Borra y retorna el cursor del LCD */
	
void lcd_init(void);					/* Inicia el LCD. Llamar antes de cualquier otra funcion */

void lcd_setCursor(unsigned int columna, unsigned int fila); /* Va a la posicion especificada */

#define LCD_RS  PORTAbits.RA0
#define LCD_EN  PORTAbits.RA1
#define LCD_D4  PORTAbits.RA2
#define LCD_D5  PORTAbits.RA4
#define LCD_D6  PORTAbits.RA5
#define LCD_D7  PORTEbits.RE0

#endif
