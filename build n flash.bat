srec_cat RTOS.X\dist\default\production\RTOS.X.production.hex -Intel Bootloader.X\dist\default\production\Bootloader.X.production.hex -Intel -o Main.hex -Intel

"C:\avrdude\avrdude.exe" -C "C:\avrdude\avrdude.conf" -c usbasp -p m328p -B 10 -b 115200 -U flash:w:"Main.hex":i

pause
