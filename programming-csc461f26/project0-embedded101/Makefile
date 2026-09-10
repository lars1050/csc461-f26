# --- Configuration ---
# Serial port the board shows up on (Mac-style path w/ wildcard for the
# suffix, since it varies by cable). Override on the command line for a
# different OS/port, e.g.:  make flash PORT=COM3
PORT=/dev/tty.SLAB_USBtoUART*

MCU=atmega328p

# -g:               include debug symbols
# -Wall:            enable common compiler warnings
# -mcall-prologues: use library routines for function entry/exit (saves space)
# -mmcu:            target chip, needed for both compiling and linking
# -Os:              optimize for size (important on limited flash memory)
CFLAGS=-g -Wall -mcall-prologues -mmcu=$(MCU) -Os

# Strip unused sections and allow the linker to relax certain jumps to
# smaller instructions where possible -- both reduce final program size.
LDFLAGS=-Wl,-gc-sections -Wl,-relax

CC=avr-gcc
TARGET=main
OBJECT_FILES=main.o

# Targets that don't correspond to real output files
.PHONY: all clean flash

all: $(TARGET).hex

clean:
	rm -f *.o *.hex *.obj

# Compile: source (.c) -> object file (.o)
main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

# Link: object file(s) (.o) -> linked binary (.obj)
%.obj: $(OBJECT_FILES)
	$(CC) $(CFLAGS) $(OBJECT_FILES) $(LDFLAGS) -o $@

# Convert: linked binary (.obj) -> flashable Intel HEX (.hex)
%.hex: %.obj
	avr-objcopy -R .eeprom -O ihex $< $@

flash: $(TARGET).hex
	avrdude -p $(MCU) -c arduino -P $(PORT) -U flash:w:$(TARGET).hex
