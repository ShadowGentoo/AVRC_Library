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

void USART_transmit_string(const char * message)
{
    unsigned int symbolNumber = 0;

    //transmit symbols, until we reach end of string
    do
    {
        USART_transmit(message[symbolNumber]);
        symbolNumber++;
    }
    while (message[symbolNumber] != '\0');
}

char USART_receive(void)
{
    //wait for data to be received
    while (!(UCSR0A & (1 << RXC0)));
    
    //take your data
    return UDR0;
}

void USART_transmit_number(const int message)
{
    int number = message;
    int reverseIndex = 0;
    char rev_msg[7] = { 0, 0, 0, 0, 0 ,0, '\0'};
    char msg[7] = {0, 0, 0, 0, 0, 0, '\0'};
    char symbol;

    if (message < 0)
    {
        number *= -1;
    }

    do
    {
        symbol = (number % 10) + 48;
        rev_msg[reverseIndex] = symbol;
        reverseIndex++;

        number = (number / 10);
    } while (number != 0);

    if (message < 0)
    {
        rev_msg[reverseIndex] = '-';
        reverseIndex++;
    }

    for (int i = 0; i < reverseIndex; i++)
    {
        msg[i] = rev_msg[(reverseIndex-1-i)];
    }
    
    USART_transmit_string(msg);

}