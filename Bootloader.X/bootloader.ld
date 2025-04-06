/* ATmega328P Memory Layout */
MEMORY
{
  text (rx)  : ORIGIN = 0x7000, LENGTH = 0x0800  /* 2 KB Bootloader region */
  data  (rw!x): ORIGIN = 0x0100, LENGTH = 0x0100  /* 256 bytes SRAM */
}

ENTRY(_start)

SECTIONS
{
  .text : 
  {
    *(.vectors)      /* Optional: jump vector to bootloader */
    *(.text)         /* Main code */
    *(.text*)        /* Subfunctions */
    *(.rodata*)      /* Constants */
  } > text

  .bss (NOLOAD) :
  {
    __bss_start = .;
    *(.bss*)
    *(COMMON)
    __bss_end = .;
  } > data
}
