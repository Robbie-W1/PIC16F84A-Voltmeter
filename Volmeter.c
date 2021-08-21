/*
Robbie Woolterton and Finn Sofroniou EE10140 Voltmeter
ADC code has been changed to take the ADC number as an argument for readdADC, and sets the CS pins accordingly

*/

#pragma config FOSC = XT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config CP = OFF

#define _XTAL_FREQ 4000000

#include <xc.h>
#include "lcd.h"
#include "adc.h"

unsigned int hold = 0; // to keep track of if we need to hold (pause) measurements

void alarm()
{
    for (int i = 0; i < 1000; i++)
    {
        RB1 = ~RB1;
        __delay_us(100);
    }
}

void __interrupt() isr() // interrupt service routine
{
    INTCONbits.INTF = 0; // reset the interrupt flag
    hold ^= 1;           // toggle hold
}

void main(void)
{
    TRISB = 0x01;
    TRISA = 0x02;

    RW = 0; // Set the R/W LCD pin to Write

    // set SPI pins
    CS0 = 1;
    CS1 = 1;
    CLK = 0;
    Lcd_Init(); // Init the LCD

    unsigned int raw_reading; // value from the ADC
    unsigned int reading;     // ADC value when doing conversions
    unsigned int int_voltage; // integer part of the voltage reading
    unsigned int dec_voltage; // decimal part of the voltage reading
    unsigned int temp;        // temporary value used in the conversions
    unsigned int adcNumber;   // which ADC we are reading from, 0 or 1

    // set up the interrupts
    INTCONbits.INTF = 0;
    OPTION_REGbits.INTEDG = 1;
    INTCONbits.INTE = 1;
    INTCONbits.GIE = 1;

    // display the welcome message
    Lcd_Set_Cursor(1, 1);
    Lcd_Write_String("Voltmeter 0-5V");
    __delay_ms(1000); // wait a second

    while (1) // infinite loop
    {
        if (hold == 0) // if we are not holding
        {

            raw_reading = readADC(adcNumber); // read the current ADC
            raw_reading += 10;                // compensate for voltage drop
            raw_reading = raw_reading >> 2;   // make it 8 bit

            // find the integer part
            reading = (raw_reading << 2) + raw_reading; // x5
            int_voltage = reading >> 8;

            //find the decimal part
            reading = raw_reading;
            reading = (reading << 6) + (reading << 5) + (reading << 2);
            dec_voltage = reading >> 8;
            dec_voltage = (dec_voltage << 2) + dec_voltage;

            // combining them to find decimal and integer parts
            temp = (int_voltage << 6) + (int_voltage << 5) + (int_voltage << 2);
            dec_voltage -= temp;

            // print results to the LCD

            // check which adc we are using and the set the LCD cursor accordingling
            if (adcNumber == 0)
            {
                Lcd_Clear();          // clear the LCD
                Lcd_Set_Cursor(1, 1); // set the cursor to the start
                adcNumber = 1;        // use the other ADC on the next loop
            }
            else if (adcNumber == 1)
            {
                // don't clear the display because we still want to show the value from the other ADC
                Lcd_Set_Cursor(1, 8); // set the cursor halfway along the display
                adcNumber = 0;        // use the other ADC on the next loop
            }

            Lcd_Write_Int(int_voltage);
            Lcd_Write_Char('.');

            // make sure it displays .05 not .5
            if (dec_voltage < 10)
                Lcd_Write_Int(0);

            Lcd_Write_Int(dec_voltage);
            Lcd_Write_Char('V');

            // low voltage alarm at 1.15V and below
            if (int_voltage < 2 && dec_voltage < 20)
                alarm();
            else if (int_voltage == 0)
                alarm();

            if (adcNumber == 0)
                __delay_ms(200);
        }
        else
            __delay_ms(200);
    }
}