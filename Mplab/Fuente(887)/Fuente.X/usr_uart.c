#include "usr_uart.h"
#include <stdint.h>       

//Funcion que envia un byte por la UART1
void SendU1Tx(unsigned char data)
{

    while(TXSTAbits.TRMT == 0);
    TXREG = data;

}

//Envia una cadena de terminacion nula por la UART1
void PutU1String(unsigned char *cadena)
{
    int i = 0;
    while (cadena[i] != 0)
    {
        SendU1Tx(cadena[i]);
        i++;
    }
}


//Función que recibe datos por el rx de la uart 1
unsigned char ReceiveU1Rx(void)
{
    unsigned char data = 0;
    data = RCREG;   

    return data;
}

//Función de habilitación de la int por la llegada de un dato en el rx Uart1
void U1RxIntEnable(void)
{
    //habilito la int por el rx
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIE1bits.RCIE = 1;    
}




