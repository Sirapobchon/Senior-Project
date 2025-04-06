/* ATmega328P Memory Layout */
MEMORY
{
  text (rx)  : ORIGIN = 0x7000, LENGTH = 0x0800  /* 2 KB Bootloader region */
  data  (rw!x): ORIGIN = 0x0100, LENGTH = 0x0100  /* 256 bytes SRAM */
}

ENTRY(_start)

SECTIONS
{
  .vectors 0x7000 :
  {
    KEEP(*(.init0))  /* now _start() is safely the reset vector */
    KEEP(*(.vectors))  /* add if needed, but no default jmp */
  } > text

  .text :
  {
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
