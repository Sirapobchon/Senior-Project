@echo off
REM Upload the bootloader via USBasp at 0x7000 (for 2 KB boot section)

avrdude -c usbasp -p m328p -B 10 -U lfuse:w:0xe2:m -U hfuse:w:0xd8:m -U efuse:w:0xff:m
avrdude -c usbasp -p m328p -U lock:w:0xFF:m
avrdude -c usbasp -p m328p -B 10 -U flash:w:dist/default/production/Bootloader.X.production.hex:i -U lock:w:0x0F:m

pause
