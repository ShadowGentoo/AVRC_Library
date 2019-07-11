#set compiler to avr-gc
CC = avr-gcc
#set compiler flags
CFLAGS = -Wall -Os -mmcu=atmega328p
#set build dir
DIR = ./build/
sourceDir = ./source/

dependecy = $(sourceDir)USART.c $(sourceDir)TM1637Display.c $(sourceDir)DHT11.c
#avr-gcc -Os -mmcu=atmega328p ./source/main.c -o main.o
#avr-objcopy -O ihex ./main.o main.hex
#avrdude -p atmega328p -c arduino -U flash:w:main.hex:i -P /dev/ttyUSB0
#arduino may be on ttyUSB and ttyACM. Curse you, China!

main : 
	$(CC) $(CFLAGS) ./source/main.c $(dependecy) -o $(DIR)main.o 
	avr-objcopy -O ihex $(DIR)main.o $(DIR)main.hex

flash : main
	avrdude -p atmega328p -c arduino -U flash:w:$(DIR)main.hex:i -P /dev/ttyUSB0

clean :
	rm -rf $(DIR)*