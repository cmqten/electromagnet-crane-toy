MCU=attiny85
AVRDUDEMCU=t85
CC=/usr/bin/avr-gcc
CFLAGS=-g -Os -Wall -mcall-prologues -mmcu=$(MCU)
OBJ2HEX=/usr/bin/avr-objcopy
AVRDUDE=/usr/local/bin/avrdude

none :
	@echo "Usage:"
	@echo "make install-[TARGET] : Uploads TARGET. TARGET must be the name of a program \n\twithout the .hex/.o/.c extension.\n"
	@echo "make noreset-[TARGET] : Uploads TARGET without resetting the mcu. TARGET must\n\tbe the name of a program without the .hex/.o/.c extension.\n"
	@echo "make clock-1mhz : Sets attiny85's clock source to the internal 1 mhz clock.\n"
	@echo "make clock-8mhz : Sets attiny85's clock source to the internal 8 mhz clock.\n"

install-% : %.hex
	sudo gpio -g mode 22 out
	sudo gpio -g write 22 0
	sudo $(AVRDUDE) -p $(AVRDUDEMCU) -P /dev/spidev0.0 -c linuxspi -b 10000 -U flash:w:$<
	sudo gpio -g write 22 1

noreset-% : %.hex
	sudo $(AVRDUDE) -p $(AVRDUDEMCU) -P /dev/spidev0.0 -c linuxspi -b 10000 -U flash:w:$<

%.hex : %.o
	$(OBJ2HEX) -R .eeprom -O ihex $^ $@

%.o : %.c
	$(CC) $(CFLAGS) $< -o $@

clock-1mhz :
	sudo gpio -g mode 22 out
	sudo gpio -g write 22 0
	sudo $(AVRDUDE) -p $(AVRDUDEMCU) -P /dev/spidev0.0 -c linuxspi -b 10000 -U lfuse:w:0x62:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m
	sudo gpio -g write 22 1

clock-8mhz :
	sudo gpio -g mode 22 out
	sudo gpio -g write 22 0
	sudo $(AVRDUDE) -p $(AVRDUDEMCU) -P /dev/spidev0.0 -c linuxspi -b 10000 -U lfuse:w:0xe2:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m
	sudo gpio -g write 22 1

clean :
	rm -f *.hex *.obj *.o 
