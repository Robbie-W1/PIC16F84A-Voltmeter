#include <stdio.h>

void bin(unsigned n)
{
    unsigned i;
    for (i = 1 << 31; i > 0; i = i / 2)
        (n & i) ? printf("1") : printf("0");
}

int main()
{
    unsigned short raw_reading = 1000;
    unsigned short reading;
    unsigned short int_voltage;
    unsigned short dec_voltage;

    // find the integer part
    raw_reading = (raw_reading >> 2);
    printf("Raw reading: \t%i\n", raw_reading);
    reading = (raw_reading << 2) + raw_reading;
    int_voltage = reading >> 8;
    printf("Int voltage: \t%i\n", int_voltage);

    // find the decimal part
    reading = raw_reading;
    printf("Reading: \t%i\n", reading);
    reading = (reading << 6) + (reading << 5) + (reading << 2);
    printf("Reading: \t%i\n", reading);
    dec_voltage = reading >> 8;
    dec_voltage = (dec_voltage << 2) + dec_voltage;
    printf("Dec voltage: \t%i\n", dec_voltage);

    // combining them to find decimal and integer parts
    unsigned short temp;
    temp = (int_voltage << 6) + (int_voltage << 5) + (int_voltage << 2);
    dec_voltage -= temp;

    // output
    printf("Measured value: \t%i.%iV\n", int_voltage, dec_voltage);

    int x = 10;
    int y = 10;

    printf("%i \n", x == y);

    unsigned int a = 0;
    bin(a);
    a ^= 1;
    printf("\n%i \n", a);
    bin(a);
    return 0;
}
