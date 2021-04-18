#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config CP = OFF

#define _XTAL_FREQ 4000000

#include <xc.h>
#include "lcd.h"
#include "adc.h"

unsigned int times_by_5(unsigned int input)
{
    // multiplying by 5 using bitshifts and addition
    unsigned int temp = input;
    input = input << 2;
    input += temp;

    return input;
}

void main(void)
{
    TRISB = 0x00;
    TRISA = 0x02;

    RW = 0; // Set the R/W LCD pin to Write

    CS = 1;
    CLK = 0;
    Lcd_Init(); // Init the LCD

    unsigned int reading;

    while (1)
    {
        reading = readADC();
        // reading = 508;
        reading = times_by_5(reading);
        reading = reading >> 10;

        Lcd_Clear();
        Lcd_Set_Cursor(1, 1);
        Lcd_Write_Int(reading);

        __delay_ms(500);
    }
}