#ifndef _USART_H_
#define _USART_H_

typedef enum {
    PARITY_DISABLED = 0,
    PARITY_EVEN = 2,
    PARITY_ODD = 3
} parity;

void USART_init(const unsigned long cpuFreq, const unsigned long baudRate, const parity parityMode);
void USART_transmit_string(const char * message);
char USART_receive(void);
void USART_transmit_number(const int message);

#endif