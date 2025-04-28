@echo off
echo --------------------------------------
echo Uploading with avrdude...

set HEX_FILE=dist\default\production\RTOS_NoUpload.X.production.hex
set ELF_FILE=dist\default\production\RTOS_NoUpload.X.production.elf

if exist "%HEX_FILE%" (
    "C:\avrdude\avrdude.exe" -C "C:\avrdude\avrdude.conf" -c usbasp -p m328p -B 10 -U flash:w:"%HEX_FILE%":i
) else (
    echo ERROR: HEX file not found: %HEX_FILE%
    exit /b 1
)

echo --------------------------------------
echo AVR Memory Usage:

if exist "%ELF_FILE%" (
    "c:\avr-gcc-14.1.0-x64-windows\bin\avr-size.exe" -B "%ELF_FILE%"
) else (
    echo ERROR: ELF file not found: %ELF_FILE%
)

exit /b 0
