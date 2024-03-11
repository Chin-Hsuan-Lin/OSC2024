.section ".text .boot"
.global _start
 
_start: // = 0x80000;
    // read cpu id, stop slave cores
    mrs     x1, mpidr_el1
    and     x1, x1, #3
    // four-core cpu, & with 11 to preserve latest two bits
    cbz     x1, setting
    // cbz: compare branch zero, if x1==0 jump to setting; otherwise continue

halt: 	
    wfe //(Wait for event)
	b	halt
setting:
    //set top of stack just before the code (_start).
    ldr     x1, =_start
    mov     sp, x1
    ldr     x1, =__bss_start
    ldr     w2, = __bss_size
clear_bss:
    cbz     w2, kernel_main
    str     xzr, [x1], #8
    sub     w2, w2, #1
    cbnz    w2, clear_bss
kernel_main:
    bl  main
    b   halt



