#include "mailbox.h"

volatile unsigned int __attribute__((aligned(16))) mbox[36];

int mbox_call(unsigned char c){         //unsigned char is 8 bits
    //1.Combine the message address (upper 28 bits) with channel number (lower 4 bits)
    //the message r consists of the address of the mailbox array in the upper bits
    //and the channel number(c) in the lower 4 bits.
    unsigned int r = (((unsigned int)((unsigned long)&mbox)&~0xF) | (c&0xF));
    //2.Check if Mailbox 0 status register’s full flag is set.
    do{asm volatile("nop");}while(*MB_STATUS & MB_FULL);
    //3.If not, then you can write to Mailbox 1 Read/Write register.
    *MB_WRITE = r;

    while(1){
        //4.Check if Mailbox 0 status register’s empty flag is set.
        do{asm volatile("nop");}while(*MB_STATUS & MB_EMPTY);
        //5.If not, then you can read from Mailbox 0 Read/Write register.
        if(r == *MB_READ){
            //6.Check if the value is the same as you wrote in step 1.
            return mbox[1]==MB_RESPONSE;
        }
    }
    return 0;
}