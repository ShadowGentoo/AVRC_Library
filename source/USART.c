#include "../include/USART.h"
#include "avr/io.h"

//TODO write something like printf(format, ... args)

void USART_init(const unsigned long cpuFreq, const unsigned long baudRate, const parity parityMode) 
{
    //calculating ubrr value
    unsigned int ubrr = (cpuFreq/(unsigned long)16)/baudRate - 1;
    
    //setting ubrr value to UBRR registers
    UBRR0H = (unsigned char) (ubrr >> 8);
    UBRR0L = (unsigned char) (ubrr);

    //set frame format: 8 data + parityMode
    UCSR0C |= (parityMode << UPM00) | (3 << UCSZ00);
    //TODO stop bits? USBS0=0/1 - 1/2 stop bits

    //enable receiver and transmitter
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
}

void USART_transmit(const char data)
{
    //wait for empty buffer
    while (!(UCSR0A & (1 << UDRE0)));

    //put data into buffer
    UDR0 = data;

    //TODO check (throught TX flag in UCSR0A) if data is gone
}

void USART_transmit_number(const unsigned int message)
{
    
}

void USART_transmit_string(const char * message)
{
    unsigned int symbolNumber = 0;

    //transmit symbols, until we reach end of string
    while (message[symbolNumber] != '\0')
    {
        USART_transmit(message[symbolNumber]);
        symbolNumber++;
    }
}

char USART_receive(void)
{
    //wait for data to be received (if you are not using interruptions for example >_<)
    while (!(UCSR0A & (1 << RXC0)));
    
    //take your data
    return UDR0;
}