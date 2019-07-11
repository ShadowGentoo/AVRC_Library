#ifndef _TM1637DISPLAY_H_
#define _TM1637DISPLAY_H_

enum display_brightness 
{
    OFF = 0x80,
    PULSE_1_16 = 0x88,
    PULSE_2_16 = 0x89,
    PULSE_4_16 = 0x8A,
    PULSE_10_16 = 0x8B,
    PULSE_11_16 = 0x8C,
    PULSE_12_16 = 0x8D,
    PULSE_14_16 = 0x8E
};

enum segments
{
    NONE = 0,
    SEG_A = 0x08,
    SEG_B = 0x10,
    SEG_C = 0x20,
    SEG_D = 0x01,
    SEG_E = 0x02,
    SEG_F = 0x04,
    SEG_G = 0x40
};

typedef struct TM1637Display
{
    volatile unsigned char * SDA_DDR;
    volatile unsigned char * SDA_PORT;
    unsigned char SDA_PIN;
    volatile unsigned char * SCL_DDR;
    volatile unsigned char * SCL_PORT;
    unsigned char SCL_PIN;
}TM1637Display;

void TM1637Display_init(const struct TM1637Display display);
void TM1637Display_show(const TM1637Display display, const int number);
void TM1637Display_showRaw(const TM1637Display display, const unsigned char rawData[4]);

#endif