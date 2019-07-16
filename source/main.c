//#define __AVR_ATmega328P__
#define F_CPU 16000000UL

#include "avr/io.h"
#include "util/delay.h"
#include "../include/USART.h"
#include "../include/TM1637Display.h"
#include "../include/DHT11.h"

int main (void)
{
    USART_init(F_CPU, 9600, PARITY_DISABLED);
  
    DHT_sensor DHT11;
    DHT11.SENSOR_DDR = &DDRB;
    DHT11.SENSOR_PORT = &PORTB;
    DHT11.SENSOR_PINR = &PINB;
    DHT11.SENSOR_PIN = 4;

    DHT_data data;
    data.temperatureI = 0;
    data.temperatureD = 0;
    data.humidityI = 0;
    data.humidityD = 0;
    data.checksum = 0;

    DHT11_init(DHT11);  

    int i = -40;
    while (1)
    {
        /* 
        _delay_ms(5000);
        DHT11_read(DHT11, &data);
        USART_transmit_string("temperature: ");
        USART_transmit_number(data.temperatureI);
        */
       
       DHT11_read(DHT11, &data);
       USART_transmit_string("temperature: ");
       USART_transmit_number((int) data.temperatureI);
       USART_transmit_string("humidity: ");
       USART_transmit_number((int) data.humidityI);
       _delay_ms(2000);
    }

    return 0;
}