OUTPUT_ARCH( "riscv" )

/* Adapted from spike.lds */

SECTIONS
{
  . = 0x0;

  .vectors_dummy : {
    /* offset +0xc0 to leave room for base vector table + 32 external IRQs
       (could make this 0; just for consistency with other hazard3 tests) */
    . = 0xc0;
  }

  .text : 
  {
    *(.text.entry)
    *(.text)
  }

  /* data segment */
  .data : { *(.data) }

  .sdata : {
    __global_pointer$ = . + 0x800;
    *(.srodata.cst16) *(.srodata.cst8) *(.srodata.cst4) *(.srodata.cst2)
    *(.srodata*)
    *(.sdata .sdata.* .gnu.linkonce.s.*)
  }

  /* bss segment */
  __bss_start = .;
  .sbss : {
    *(.sbss .sbss.* .gnu.linkonce.sb.*)
    *(.scommon)
  }
  .bss : { *(.bss) }
  __bss_end = .;

  __malloc_start = .;
  . = . + 512;

  /* End of uninitalized data segement */
  _end = .;
}
