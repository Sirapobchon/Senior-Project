srec_cat RTOS.X\dist\default\production\RTOS.X.production.hex -Intel Bootloader.X\dist\default\production\Bootloader.X.production.hex -Intel -o Main.hex -Intel

avrdude -c usbasp -p m328p -B 10 -U flash:w:"Main.hex":i

pause
