#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config CP = OFF

#define _XTAL_FREQ 4000000

#include <xc.h>

// #define BUTTON PORTB.bits.RB0
#define LED1 PORTBbits.RB1
#define LED1_TRIS TRISBbits.TRISB1
#define LED2 PORTBbits.RB2
#define LED2_TRIS TRISBbits.TRISB2

void __interrupt() isr() // interrupt service routine
{
    INTCONbits.INTF = 0; // reset the interrupt flag
    LED2 = ~LED2;        // toggle LED2
}

void main(void)
{
    TRISA = 0xFF; // set all of PORTA to inputs
    LED1_TRIS = 0;
    LED2_TRIS = 0;

    INTCONbits.INTF = 0;
    OPTION_REGbits.INTEDG = 1;
    INTCONbits.INTE = 1;
    INTCONbits.GIE = 1;

    while (1)
    {
        LED1 = 1;
        __delay_ms(500);
        LED1 = 0;
        __delay_ms(500);
    }
}