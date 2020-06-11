#include <xc.h>
#include <stdio.h>
#include "header.h"
#include "drv_lcd.h"
#include "usr_uart.h"

#ifndef PINES
#define PINES
#define LCD_RS_PIN  PORTAbits.RA0
#define LCD_E_PIN   PORTAbits.RA1
#define LCD_D4_PIN  PORTAbits.RA2
#define VREF_PIN    PORTAbits.RA3
#define LCD_D5_PIN  PORTAbits.RA4
#define LCD_D6_PIN  PORTAbits.RA5

#define VSENS1PIN   PORTBbits.RB0
#define VSENS2PIN   PORTBbits.RB1
#define ISENS1PIN   PORTBbits.RB2
#define ISENS2PIN   PORTBbits.RB3
#define RENC_APIN   PORTBbits.RB4
#define RENC_BPIN   PORTBbits.RB5
#define BOTON1PIN   PORTBbits.RB6
#define BOTON2PIN   PORTBbits.RB7

#define SWITCH1PIN  PORTCbits.RC0
#define SWITCH2PIN  PORTCbits.RC1
#define FAN_PIN     PORTCbits.RC2
#define SCK_PIN     PORTCbits.RC3
#define CS1_PIN     PORTCbits.RC4
#define MOSI_PIN    PORTCbits.RC5
#define TXD_PIN     PORTCbits.RC6
#define RXD_PIN     PORTCbits.RC7

#define CS2_PIN     PORTDbits.RD0
#define LDAC_PIN    PORTDbits.RD1
#define RELAY2_PIN  PORTDbits.RD2
#define RELAY1_PIN  PORTDbits.RD3
#define BT_EN_PIN   PORTDbits.RD4
#define RGB_B_PIN   PORTDbits.RD5
#define RGB_R_PIN   PORTDbits.RD6
#define RGB_G_PIN   PORTDbits.RD7

#define LCD_D7_PIN  PORTEbits.RE0
#define LCD_BL_PIN  PORTEbits.RE1
#endif

#ifndef CANALES_ANALOGICOS
#define CANALES_ANALOGICOS
#define VSENS1CH    12
#define VSENS2CH    10
#define ISENS1CH    8
#define ISENS2CH    9
#endif

#define PRECARGATMR0 176

//definicion de funciones
void pwmupdate();

//definicion de variables globales

char paso = 35;

char *vector[15];

bit renc_a_ant = 0;
bit renc_b_ant = 0;
bit but1 = 0;
bit but2 = 0;

char pwmr = 255;
char pwmg = 35;
char pwmb = 0;
char encpwmmode = 1;

void interrupt isr()
{
    int rbif = INTCONbits.RBIF;
    int t0if = INTCONbits.T0IF;
    
    INTCON = 0x28; //GIE = 0; RBIE = 1; T0IE = 1;
    
    if (rbif)
    {
        if (renc_a_ant < RENC_APIN && renc_b_ant < RENC_BPIN)
        {
            paso--;
        }
        else if(renc_a_ant < RENC_APIN && renc_b_ant == RENC_BPIN)
        {
            paso++;
        }
        
        renc_a_ant = RENC_APIN;
        renc_b_ant = RENC_BPIN;
    }
    
    if (t0if) //16nS
    {
        TMR0 = 0;
        
        if (pwmr <= TMR2)
        {RGB_R_PIN = 0;}
        else
        {RGB_R_PIN = 1;}
        
        if (pwmg <= TMR2)
        {RGB_G_PIN = 0;}
        else
        {RGB_G_PIN = 1;}
        
        if (pwmb <= TMR2)
        {RGB_B_PIN = 0;}
        else
        {RGB_B_PIN = 1;}
        
        
        TMR0 = TMR0 + PRECARGATMR0;
    }
    INTCON = 0xA8; //GIE = 1; T0IE = 1; RBIE = 1;
}

void main()
{
    TRISA = 0xC8;
    PORTA = 0x00;
    TRISB = 0xFF;
    TRISC = 0x83;
    PORTC = 0x00;
    TRISD = 0x00;
    PORTD = 0x00;
    TRISE = 0x00;
    PORTE = 0x00;

    ANSEL = 0x08;
    ANSELH = 0x17;

    //Inicializo el LCD
    LCD_BL_PIN = 1;
    
    lcd_write(0);
    
    lcd_init();
    lcd_clear();
    lcd_print("Iniciado");
    
    //Config Timer 0:
    OPTION_REG = 0xC8; //IntOSC, Prescaler = 1/1
    TMR0 = PRECARGATMR0;    //Int cada 16nS
    
    //Config Timer 2:
    T2CON = 0x26; //Psa 1:16 , postscaler: 1:5, tmr2 on
    TMR2 = 0;     //Va desde 0 a 255, en 4,08mS (245,1Hz)
    PR2 = 255;
    
    //Inicializo Interrupciones
    IOCB = 0xF0;  //Int por cambio de estado en RB4,RB5,RB6 y RB7
    INTCON = 0xA8; //GIE = 1; T0IE = 1; RBIE = 1;
    
    while (1)
    {
        __delay_ms(50);
        pwmupdate();
        INTCONbits.GIE = 0;
        sprintf(vector, "R:%3dG:%3dB:%3d", pwmr, pwmg, pwmb);
        lcd_setCursor(0,0);
        lcd_print(vector);
        sprintf(vector, "set:%3d", paso);
        lcd_setCursor(0,1);
        lcd_print(vector);
        INTCONbits.GIE = 1;
        
    }
}

void pwmupdate()
{
    encpwmmode = (SWITCH1PIN << 1) | SWITCH2PIN;
    if (BOTON2PIN)
    {
        switch (encpwmmode)
        {
            case 0:
                pwmr=0;
                pwmg=0;
                pwmb=0;
                break;
            case 1:
                pwmr = paso;
                break;
            case 2:
                pwmg = paso;
                break;
            case 3:
                pwmb = paso;
                break;
        }
    }
}