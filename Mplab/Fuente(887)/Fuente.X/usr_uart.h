#include <xc.h>

#define B9600   (int) (_XTAL_FREQ/9600)/16  - 1

void U1RxIntEnable(void);
void SendU1Tx(unsigned char data);
unsigned char ReceiveU1Rx(void);
void PutU1String(unsigned char *cadena);