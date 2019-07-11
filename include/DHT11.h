#ifndef _DHT11_H_
#define _DHT11_H_

typedef struct DHT_data
{
    char temperatureI;
    char temperatureD;
    char humidityI;
    char humidityD;
    char checksum;
}DHT_data;

typedef struct DHT_sensor
{
    volatile unsigned char * SENSOR_DDR;
    volatile unsigned char * SENSOR_PORT;
    volatile unsigned char * SENSOR_PINR;
    unsigned char SENSOR_PIN;
}DHT_sensor;

void DHT11_init(DHT_sensor sensor);
int DHT11_read(DHT_sensor sensor, DHT_data * data);

#endif