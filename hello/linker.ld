OUTPUT_FORMAT(binary)

SECTIONS {
    .init   : { *(.init) }
    .text   : { *(.text) }
    .rodata : { *(.rodata) }
    .data   : { *(.data) }
}
