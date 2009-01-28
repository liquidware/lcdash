###############################################################################
# Makefile for the project LCDash
###############################################################################

## General Flags
PROJECT = LCDash
MCU = atmega2561
TARGET = LCDash-v2.1.elf
CC = avr-gcc.exe

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2                                                       -DF_CPU=16000000UL -Os -fsigned-char
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS += 


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"C:\Documents and Settings\chris.ladden\My Documents\AVR\LCDash\LCDash-v2.1\ms" -I"C:\Documents and Settings\chris.ladden\My Documents\AVR\LCDash\LCDash-v2.1\ms\ms1" -I"C:\Documents and Settings\chris.ladden\My Documents\AVR\LCDash\LCDash-v2.1\ms\ms2" -I"C:\Documents and Settings\chris.ladden\My Documents\AVR\LCDash\LCDash-v2.1\." -I"C:\Documents and Settings\chris.ladden\My Documents\AVR\LCDash\LCDash-v2.1\gauges" 

## Libraries
LIBS = -lm 

## Objects that must be built in order to link
OBJECTS = LCDash.o calibrate.o gui.o t6963.o touchscreen.o usb.o FT232R.o gauge.o string_manip.o messageBar.o analogGauge.o barGauge.o digiGauge.o megasquirt.o ms1.o scopeGauge.o ms1_wizard.o speedo.o menu.o XLdigiGauge.o adc_sampling.o sensors_oilpressure.o obd.o mut.o datalogging.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) LCDash-v2.1.hex LCDash-v2.1.eep size

## Compile
LCDash.o: ../LCDash.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

calibrate.o: ../calibrate.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

gui.o: ../gui.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

t6963.o: ../t6963.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

touchscreen.o: ../touchscreen.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

usb.o: ../usb.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

FT232R.o: ../FT232R.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

gauge.o: ../gauge.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

string_manip.o: ../string_manip.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

messageBar.o: ../messageBar.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

analogGauge.o: ../gauges/analogGauge.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

barGauge.o: ../gauges/barGauge.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

digiGauge.o: ../gauges/digiGauge.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

megasquirt.o: ../ms/megasquirt.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ms1.o: ../ms/ms1/ms1.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

scopeGauge.o: ../gauges/scopeGauge.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ms1_wizard.o: ../ms/ms1/ms1_wizard.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

speedo.o: ../speedo.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

menu.o: ../menu.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

XLdigiGauge.o: ../gauges/XLdigiGauge.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

adc_sampling.o: ../adc_sampling.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

sensors_oilpressure.o: ../sensors_oilpressure.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

obd.o: ../obd.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

mut.o: ../mut.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

datalogging.o: ../datalogging.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) LCDash-v2.1.elf dep/* LCDash-v2.1.hex LCDash-v2.1.eep

## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

