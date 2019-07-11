#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include "../include/DHT11.h"
#include "avr/io.h"
#include "util/delay.h"
#include "../include/USART.h"
#include "avr/interrupt.h"

//TODO rewrite to be usable with DHT22 if it's not
//TODO maybe use timers to avoid infinite loops. Actually needed.

void DHT11_init(DHT_sensor sensor)
{
    *(sensor.SENSOR_DDR) |= (1 << sensor.SENSOR_PIN);
    *(sensor.SENSOR_PORT) |= (1 << sensor.SENSOR_PIN);
}

char DHT11_readByte(DHT_sensor sensor)
{
    char byte = 0;

    for (char i = 7; i >= 0; i--)
    {
        //each bit starts with 50us LOW
        //so wait for bit..
        _delay_us(10);
        while (! (*(sensor.SENSOR_PINR) & (1 << sensor.SENSOR_PIN)));

        //now comes bit.. 26-28us - "0", 70us - "1"
        //lazy check. if after 30-40us you have HIGH - its "1"
        _delay_us(30);
        //ant btw.. higher bits first
        if (*(sensor.SENSOR_PINR) & (1 << sensor.SENSOR_PIN))
        {
            byte |= (1 << i);
            //wait for end of bit transmition
            while (*(sensor.SENSOR_PINR) & (1 << sensor.SENSOR_PIN));
        }
    }

    return byte;
}

int DHT11_read(DHT_sensor sensor, DHT_data * data)
{
    int checksum = 0;
    /* To init sensor we need set bus to LOW for at least 18 ms
    then we will receive response (it may take up to 4ms)
    */
    //start of tight time section
    //cli();

    *(sensor.SENSOR_PORT) &= ~(1 << sensor.SENSOR_PIN);
    //need to wait at least 18 ms
    _delay_ms(20);
    *(sensor.SENSOR_PORT) |= (1 << sensor.SENSOR_PIN);
    //DHT11 will response in 20-40us
    *(sensor.SENSOR_DDR) &= ~(1 << sensor.SENSOR_PIN);
    *(sensor.SENSOR_PORT) &= ~(1 << sensor.SENSOR_PIN);

    //provocation complete. Now receive data
    _delay_us(50);
    //DHT11 will keep LOW for 80us and then HIGH for 80us
    while (! (*(sensor.SENSOR_PINR) & (1 << sensor.SENSOR_PIN)));
    while (*(sensor.SENSOR_PINR) & (1 << sensor.SENSOR_PIN));

    data->humidityI = DHT11_readByte(sensor);
    data->humidityD = DHT11_readByte(sensor);
    data->temperatureI = DHT11_readByte(sensor);
    data->temperatureD = DHT11_readByte(sensor);
    data->checksum = DHT11_readByte(sensor);

    checksum = data->humidityI + data->humidityD + data->temperatureI + data->temperatureD;


    //turn on interruptions
    //sei();

    if ((checksum & 0xFF) == data->checksum)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}