#!/usr/bin/env bash

if [ -z "$1" ]
  then
    echo
    echo "Cannot generate makefile"
    echo "Please provide a program file name"
    exit -1
fi

name=$1

cat > makefile << EOF
PRG            = ${name}
OBJ            = \$(PRG).o
MCU_TARGET     = atmega328p
PROGRAMMER     = arduino
AVRDUDE_TARGET = ATMEGA328P
PORT		   = /dev/ttyACM0
OPTIMIZE       = -Os
DEFS           = -DF_CPU=16000000UL
LIBS           =
DUDE           = avrdude
CC             = avr-gcc
OBJCOPY        = avr-objcopy
OBJDUMP        = avr-objdump
BAUD_RATE      = 115200
override CFLAGS        = -g -Wall \$(OPTIMIZE) -mmcu=\$(MCU_TARGET) \$(DEFS)
override LDFLAGS       = -Wl,-Map,\$(PRG).map

all: text

\$(PRG).elf: \$(OBJ)
	\$(CC) \$(CFLAGS) \$(LDFLAGS) -o \$@ \$^ \$(LIBS)

clean:
	rm -rf \$(PRG).o \$(PRG).elf \$(PRG).eps  \$(PRG).hex
	rm -rf \$(PRG).lst \$(PRG).map \$(EXTRA_CLEAN_FILES)

text: hex

hex:  \$(PRG).hex

%.hex: %.elf
	\$(OBJCOPY) -j .text -j .data -O ihex \$< \$@

upload: 
	\$(DUDE) -F -V -p \$(AVRDUDE_TARGET) -c \$(PROGRAMMER) -P \$(PORT) -b \$(BAUD_RATE) -U flash:w:\$(PRG).hex 
	 	
erase:
	\$(DUDE) -p \$(AVRDUDE_TARGET) -c \$(PROGRAMMER) -P \$(PORT) -e -vv -b \$(BAUD_RATE)
EOF