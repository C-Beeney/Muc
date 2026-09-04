/* DISCLAIMER : THIS IS ENTIRELY THE WORK OF CHATGPT, NOT ME. */

ENTRY(_start)

PHDRS
{
    text_hdr PT_LOAD FILEHDR PHDRS FLAGS(5); /* 5 = PF_R | PF_X (Read + Execute) */
    data_hdr PT_LOAD               FLAGS(6); /* 6 = PF_R | PF_W (Read + Write) */
}

SECTIONS
{
    /* PAGE 1: Read-Execute (Code & Constants) */
    . = 0x200000 + SIZEOF_HEADERS;

    .text :
    {
        *(.text)
        *(.text.*)
        *(.rodata)
        *(.rodata.*)
    } :text_hdr

    /* FORCE ALIGNMENT TO NEXT 4K PAGE BOUNDARY */
    . = ALIGN(0x1000);

    /* PAGE 2: Read-Write (Variables & Context) */
    .data :
    {
        *(.data)
        *(.data.*)
        *(.bss)
        *(.bss.*)
    } :data_hdr

    /DISCARD/ :
    {
        *(.comment)
        *(.note*)
    }
}