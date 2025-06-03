@echo off
echo --------------------------------------
echo Uploading with avrdude...
"C:\avrdude\avrdude.exe" -C "C:\avrdude\avrdude.conf" -c usbasp -p m328p -B 10 -U flash:w:"dist\default\production\RTOS.X.production.hex":i
echo --------------------------------------
echo AVR Memory Usage:
"c:\avr-gcc-14.1.0-x64-windows\bin\avr-size.exe" -B dist\default\production\RTOS.X.production.elf