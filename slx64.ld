/* DISCLAIMER : THIS IS ENTIRELY THE WORK OF CHATGPT, NOT ME. */

ENTRY(_start)

PHDRS
{
    load  PT_LOAD FILEHDR PHDRS FLAGS(5); /* PF_R | PF_X */
    stack PT_GNU_STACK       FLAGS(6);    /* PF_R | PF_W */
}

SECTIONS
{
    /*
     * Keep the ELF and program headers mapped at 0x200000, then place
     * code and constants immediately after them in the same PT_LOAD.
     */
    . = 0x200000 + SIZEOF_HEADERS;

    .text :
    {
        *(.text)
        *(.text.*)

        *(.rodata)
        *(.rodata.*)
    } :load

    /DISCARD/ :
    {
        *(.comment)
        *(.note*)
        *(.eh_frame*)
        *(.eh_frame_hdr*)
    }
}