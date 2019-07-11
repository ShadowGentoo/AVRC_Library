//#define __AVR_ATmega328P__
#define F_CPU 12000000UL

#include "avr/io.h"
#include "util/delay.h"
#include "../include/USART.h"
#include "../include/TM1637Display.h"
#include "../include/DHT11.h"

int main (void)
{
    USART_init(F_CPU, 9600, PARITY_DISABLED);
    DDRB |= (1 << DDB5);

    TM1637Display display;
    display.SDA_DDR = &DDRD;
    display.SDA_PORT = &PORTD;
    display.SDA_PIN = PIN7;
    display.SCL_DDR = &DDRD;
    display.SCL_PORT = &PORTD;
    display.SCL_PIN = PIN6;

    TM1637Display display2;
    display2.SDA_DDR = &DDRB;
    display2.SDA_PORT = &PORTB;
    display2.SDA_PIN = PIN1;
    display2.SCL_DDR = &DDRB;
    display2.SCL_PORT = &PORTB;
    display2.SCL_PIN = PIN0;

    DHT_sensor DHT11;
    DHT11.SENSOR_DDR = &DDRB;
    DHT11.SENSOR_PORT = &PORTB;
    DHT11.SENSOR_PIN = PIN4;
    DHT11.SENSOR_PINR = & PINB;

    DHT_data data;

    DHT11_init(DHT11);
    TM1637Display_init(display);
    TM1637Display_init(display2);    

    while (1)
    {
        _delay_ms(5000);
        DHT11_read(DHT11, &data);
        TM1637Display_show(display, data.temperatureI);
        TM1637Display_show(display2, data.humidityI);
    }

    return 0;
}