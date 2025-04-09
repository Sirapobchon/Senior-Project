/* ATmega328P Memory Layout */
MEMORY
{
  text (rx)  : ORIGIN = 0x7000, LENGTH = 0x0800  /* 2 KB Bootloader region */
  data  (rw!x): ORIGIN = 0x0100, LENGTH = 0x02FF  /* 512 bytes SRAM */
  eeprom (rw)  : ORIGIN = 0x0000, LENGTH = 0x0400   /* EEPROM 1KB */
}

ENTRY(_start)

SECTIONS
{
  .text :
  {
    *(.init0) 
    *(.init*) 
    *(.text*) 
    *(.rodata*)
  } > text

  .data : {
    *(.data)
    *(.data*)
  } > data

  .bss : {
    *(.bss)
    *(.bss*)
  } > data

  .noinit (NOLOAD) : {
    *(.noinit)
    *(.noinit*)
  } > data
}
