ENTRY(loader) /* the name of the entry label */

SECTIONS {
    . = 0x00100000; /* the code should be loaded at 1 MB */

    .text ALIGN (0x1000) : /* align at 4 KB */
    {
        code = .; _code = .; __code = .;
        *(.text) /* all text sections from all files */
    }

    .rodata ALIGN (0x1000) : /* align at 4 KB */
    {
        rodata = .; _rodata = .; __rodata = .;
        *(.rodata*) /* all read-only data sections from all files */
    }

    .data ALIGN (0x1000) : /* align at 4 KB */
    {
        data = .; _data = .; __data = .;
        *(.data) /* all data sections from all files */
    }

    .bss ALIGN (0x1000) : /* align at 4 KB */
    {
        bss = .; _bss = .; __bss = .;
        *(COMMON) /* all COMMON sections from all files */
        *(.bss) /* all bss sections from all files */
    }

    end = .; _end = .; __end = .;
}