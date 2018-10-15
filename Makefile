# File:   Makefile
# Author: M. P. Hayes, UCECE
# Date:   12 Sep 2010
# Descr:  Makefile for game

# Definitions.
CC = avr-gcc
CFLAGS = -mmcu=atmega32u2 -Os -Wall -Wstrict-prototypes -Wextra -g -I. -I../../utils -I../../fonts -I../../drivers -I../../drivers/avr -I../../extra -I/modules
OBJCOPY = avr-objcopy
SIZE = avr-size
DEL = rm


# Default target.
all: game.out


# Compile: create object files from C source files.
game.o: game.c ../../drivers/avr/system.h ../../drivers/button.h ../../utils/pacer.h ../../drivers/navswitch.h ../../drivers/avr/ir_uart.h ../../utils/tinygl.h ../../utils/font.h modules/displayer.h modules/initializer.h ../../utils/task.h ../../extra/mmelody.h  ../../extra/ticker.h ../../extra/tweeter.h game.h modules/sound.h
	$(CC) -c $(CFLAGS) $< -o $@

system.o: ../../drivers/avr/system.c ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

pio.o: ../../drivers/avr/pio.c ../../drivers/avr/pio.h ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

button.o: ../../drivers/button.c ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/button.h
	$(CC) -c $(CFLAGS) $< -o $@

timer.o: ../../drivers/avr/timer.c ../../drivers/avr/system.h ../../drivers/avr/timer.h
	$(CC) -c $(CFLAGS) $< -o $@

pacer.o: ../../utils/pacer.c ../../drivers/avr/system.h ../../drivers/avr/timer.h ../../utils/pacer.h
	$(CC) -c $(CFLAGS) $< -o $@

navswitch.o: ../../drivers/navswitch.c ../../drivers/avr/delay.h ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/navswitch.h
	$(CC) -c $(CFLAGS) $< -o $@

usart1.o: ../../drivers/avr/usart1.c ../../drivers/avr/system.h ../../drivers/avr/usart1.h
	$(CC) -c $(CFLAGS) $< -o $@

prescale.o: ../../drivers/avr/prescale.c ../../drivers/avr/prescale.h ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

timer0.o: ../../drivers/avr/timer0.c ../../drivers/avr/bits.h ../../drivers/avr/prescale.h ../../drivers/avr/system.h ../../drivers/avr/timer0.h
	$(CC) -c $(CFLAGS) $< -o $@

ir_uart.o: ../../drivers/avr/ir_uart.c ../../drivers/avr/ir_uart.h ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/avr/timer0.h ../../drivers/avr/usart1.h
	$(CC) -c $(CFLAGS) $< -o $@

font.o: ../../utils/font.c ../../drivers/avr/system.h ../../utils/font.h
	$(CC) -c $(CFLAGS) $< -o $@

ledmat.o: ../../drivers/ledmat.c ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/ledmat.h
	$(CC) -c $(CFLAGS) $< -o $@

display.o: ../../drivers/display.c ../../drivers/avr/system.h ../../drivers/display.h ../../drivers/ledmat.h
	$(CC) -c $(CFLAGS) $< -o $@

tinygl.o: ../../utils/tinygl.c ../../drivers/avr/system.h ../../drivers/display.h ../../utils/font.h ../../utils/tinygl.h
	$(CC) -c $(CFLAGS) $< -o $@

displayer.o: modules/displayer.c ../../drivers/avr/system.h ../../drivers/avr/pio.h ../../utils/pacer.h ../../utils/tinygl.h ../../utils/font.h ../../drivers/navswitch.h modules/displayer.h
	$(CC) -c $(CFLAGS) $< -o $@

initializer.o: modules/initializer.c ../../drivers/avr/system.h ../../drivers/avr/pio.h ../../utils/pacer.h ../../utils/tinygl.h ../../utils/font.h ../../drivers/navswitch.h ../../drivers/avr/ir_uart.h ../../drivers/button.h modules/initializer.h
	$(CC) -c $(CFLAGS) $< -o $@

task.o: ../../utils/task.c ../../drivers/avr/system.h ../../drivers/avr/timer.h ../../utils/task.h
	$(CC) -c $(CFLAGS) $< -o $@

ticker.o: ../../extra/ticker.c
	$(CC) -c $(CFLAGS) $< -o $@

tweeter.o: ../../extra/tweeter.c ../../drivers/avr/system.h ../../extra/ticker.h ../../extra/tweeter.h
	$(CC) -c $(CFLAGS) $< -o $@

mmelody.o: ../../extra/mmelody.c ../../drivers/avr/system.h ../../extra/mmelody.h
	$(CC) -c $(CFLAGS) $< -o $@

sound.o: modules/sound.c ../../drivers/avr/system.h ../../extra/tweeter.h ../../drivers/avr/pio.h ../../extra/mmelody.h modules/sound.h
	$(CC) -c $(CFLAGS) $< -o $@




# Link: create ELF output file from object files.
game.out: game.o system.o pio.o button.o timer.o pacer.o navswitch.o usart1.o prescale.o timer0.o ir_uart.o font.o ledmat.o display.o tinygl.o displayer.o initializer.o task.o ticker.o tweeter.o mmelody.o sound.o
	$(CC) $(CFLAGS) $^ -o $@ -lm
	$(SIZE) $@


# Target: clean project.
.PHONY: clean
clean:
	-$(DEL) *.o *.out *.hex


# Target: program project.
.PHONY: program
program: game.out
	$(OBJCOPY) -O ihex game.out game.hex
	dfu-programmer atmega32u2 erase; dfu-programmer atmega32u2 flash game.hex; dfu-programmer atmega32u2 start


