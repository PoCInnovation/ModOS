ENTRY(_start) 

SECTIONS {
    . = 1M;        /* Commencer à partir de 1 Mo */

    .text : {
        *(.text)   /* Inclut toutes les sections .text */
    }

    .data : {
        *(.data)   /* Inclut toutes les sections .data */
    }

    .bss : {
        *(.bss)    /* Inclut toutes les sections .bss */
    }
}

