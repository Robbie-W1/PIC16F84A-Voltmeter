#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config CP = OFF

#define _XTAL_FREQ 4000000

#include <xc.h>
#include "lcd.h"
#include "adc.h"

unsigned int hold = 0;

void __interrupt() isr() // interrupt service routine
{
    INTCONbits.INTF = 0; // reset the interrupt flag

    // toggle hold
    if (hold == 0)
        hold = 1;
    else
        hold = 0;
}

void main(void)
{
    TRISB = 0x00;
    TRISA = 0x02;

    RW = 0; // Set the R/W LCD pin to Write

    CS = 1;
    CLK = 0;
    Lcd_Init(); // Init the LCD

    unsigned int raw_reading;
    unsigned int reading;
    unsigned int int_voltage;
    unsigned int dec_voltage;

    Lcd_Set_Cursor(1, 1);
    Lcd_Write_String("Voltmeter 0-5V");
    __delay_ms(1000);

    while (1)
    {
        if (hold == 0)
        {
            reading = readADC();
            reading = reading >> 2;             // shift over by 2 since we're only using 8 of the 10 bits
            reading = (reading << 2) + reading; // times by 5
            int_voltage = reading >> 8;         // divide by 256

            reading = (reading << 6) + (reading << 5) + (reading << 2); // times by 100
            dec_voltage = reading >> 8;                                 // divide by 256

            dec_voltage -= (int_voltage << 6) + (int_voltage << 5) + (int_voltage << 2);

            // print results to the LCD
            Lcd_Clear();
            Lcd_Set_Cursor(1, 1);
            Lcd_Write_Int(int_voltage);
            Lcd_Write_Char('.');
            Lcd_Write_Int(dec_voltage);
            Lcd_Write_Char('V');

            __delay_ms(200);
        }
        else
            __delay_ms(200);
    }
}