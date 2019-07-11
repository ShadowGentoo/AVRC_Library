#ifndef F_CPU
#define F_CPU 16000000UL
#endif

//TU in microseconds
#define TU 4

#include "../include/TM1637Display.h"
#include "avr/io.h"
#include "util/delay.h"

enum commands
{
    WRITE_AUTO = 0x40,
    WRITE_ADDR = 0x44,
    ADDR_C0H = 0xC0,
    ADDR_C1H = 0xC1,
    ADDR_C2H = 0xC2,
    ADDR_C3H = 0xC3,
    ADDR_C4H = 0xC4,
    ADDR_C5H = 0xC5
};

//CH0 - RIGHT
//CH3 - LEFT
//if command WRITE_AUTO, it will move throught CH4 and CH5 TOO!
//CH1 have ':' on 7 bit
//TODO mechanism to write ':' and dots..

void TM1637Display_transmit(const TM1637Display display, const unsigned char numberOfBytes, const unsigned char * bytes)
{
    //start message
    *(display.SDA_PORT) &= ~(1 << display.SDA_PIN);
    _delay_us(TU);
    *(display.SCL_PORT) &= ~(1 << display.SCL_PIN);
    _delay_us(TU);

    for (unsigned char i = 0; i < numberOfBytes; i++)
    {
        //transmit one byte
        for (unsigned char bit = 0; bit < 8; bit++)
        {
            if (bytes[i] & (1 << bit))
            {
                *(display.SDA_PORT) |= (1 << display.SDA_PIN);
            }
            else 
            {
                *(display.SDA_PORT) &= ~(1 << display.SDA_PIN);
            }
            *display.SCL_PORT |= (1 << display.SCL_PIN);
            _delay_us(TU);
            *display.SCL_PORT &= ~(1 << display.SCL_PIN);
            _delay_us(TU);
        }

        //9 CLK with "ACK" on SDA
        *(display.SDA_PORT) &= ~(1 << display.SDA_PIN);
        *display.SCL_PORT |= (1 << display.SCL_PIN);
        _delay_us(TU);
        *display.SCL_PORT &= ~(1 << display.SCL_PIN);
        _delay_us(TU);
    }

    //stop message
    *(display.SCL_PORT) |= (1 << display.SCL_PIN);
    _delay_us(TU);
    *(display.SDA_PORT) |= (1 << display.SDA_PIN);
    _delay_us(TU);
}

void TM1637Display_set_brightness(const TM1637Display display, enum display_brightness brightness)
{
    unsigned char byte = brightness;
    TM1637Display_transmit(display, 1, &byte);
}

void TM1637Display_init(const TM1637Display display)
{
    *(display.SDA_DDR) |= (1 << display.SDA_PIN);
    *(display.SCL_DDR) |= (1 << display.SCL_PIN);
    *(display.SDA_PORT) |= (1 << display.SDA_PIN);
    *(display.SCL_PORT) |= (1 << display.SCL_PIN);

    TM1637Display_set_brightness(display, PULSE_1_16);
    
    unsigned char commands[] = {WRITE_AUTO, SEG_G, SEG_G, SEG_G, SEG_G, NONE, NONE};
    TM1637Display_transmit(display, 7, commands);
}

void TM1637Display_show(const TM1637Display display, const int number)
{
    int numberInWork = number;
    int symbol;

    unsigned char message[7] = {WRITE_AUTO, 0, 0, 0, 0, 0, 0};
    unsigned char index = 1;

    if (number < 0)
    {
        numberInWork *= -1;
    }

    do
    {
        symbol = numberInWork % 10;
        numberInWork = numberInWork / 10;

        switch (symbol)
        {
        case 0:
            message[index] = SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F;
            break;
        case 1:
            message[index] = SEG_B | SEG_C;
            break;
        case 2:
            message[index] = SEG_A | SEG_B | SEG_D | SEG_E | SEG_G;
            break;
        case 3:
            message[index] = SEG_A | SEG_B | SEG_C | SEG_D | SEG_G;
            break;
        case 4:
            message[index] = SEG_B | SEG_C | SEG_F | SEG_G;
            break;
        case 5:
            message[index] = SEG_A | SEG_C | SEG_D | SEG_F |SEG_G;
            break;
        case 6:
            message[index] = SEG_A | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G;
            break;
        case 7:
            message[index] = SEG_A | SEG_B | SEG_C;
            break;
        case 8:
            message[index] = SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G;
            break;
        case 9:
            message[index] = SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G;
            break;       
        default:
            message[index] = 0;
            break;
        }

        index++;
    } while ((numberInWork != 0) || (index == 6));

    //if we can write "minus" and number is negative
    if ((index <= 4) && (number < 0))
    {
        message[index] = SEG_G;
    }

    //transfer
    TM1637Display_transmit(display, 7, message);
}

void TM1637Display_showRaw(const TM1637Display display, const unsigned char rawData[4])
{
    unsigned char message[] = {WRITE_AUTO, rawData[0], rawData[1], rawData[2], rawData[3], 0, 0};

    TM1637Display_transmit(display, 7, message);
}
