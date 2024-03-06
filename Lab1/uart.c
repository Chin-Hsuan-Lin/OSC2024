#include "HEADER/uart.h"

void uart_init(){
    //GPIO alternative function selection
    register unsigned int selector;

    selector = *GPFSEL1;
    selector &= ~(7<<12);   //clean gpio14
    selector |= 2<<12;      //set alt5 for gpio14
    selector &= ~(7<<15);   //clean gpio15
    selector |= 2<<15;
    *GPFSEL1 = selector;

    //GPIO pull-up/down setting
    *GPPUD = 0;                                 //write to GPPUD to set the require signal
    selector = 150;
    while(selector--){asm volatile("nop");}     //wait 150 cycles to provide the set-up time
    //GPIO controls 54 pins
    //GPPUDCLK0 controls 0-31 pins
    //set no.14 and 15 bit to 1 to clock the control signal into the GPIO pads.
    *GPPUDCLK0 = (1<<14)|(1<<15);
    selector = 150;
    while(selector--){asm volatile("nop");}
    //flush the modify bits.
    *GPPUDCLK0 = 0;

    //Mini UART initialize
    // *Enable mini uart (this also enables access to all the mini uart registers)
    *AUX_ENABLE = 1;
    // *Disable the receiver and transmitter before the configuration is finished
    //  Disable the auto flow control
    *AUX_MU_CNTL = 0;
    // *Disable receive and transmit interrupts
    *AUX_MU_IER = 0;
    // *Enable 8 bit mode (mini uart supports either 7- or 8-bit operations)
    *AUX_MU_LCR = 3;
    // *The RTS line is used in the flow control and we don't need it. Set high all the time
    *AUX_MU_MCR = 0;
    // *Set baud rate to 115200
    //  baudrate = system_clock_freq / (8 * ( baudrate_reg + 1 )) 
    *AUX_MU_BAUD = 270;
    //  *Disable FIFO
    *AUX_MU_IIR = 0xc6;
    // *Enable transmitter and receiver
    *AUX_MU_CNTL = 3;
    //Finish

}

void uart_send( unsigned int c ){
    // bit five is set to 1 if the transmit (is empty) FIFO can accept at least one
    while(!(*AUX_MU_LSR&0x20)){
        break;
    }
    *AUX_MU_IO = c;
}

char uart_recv(){
    // bit zero is set to 1 indicates that the data is ready
    while(!(*AUX_MU_LSR&0x01)){
        break;
    }
    char r = (char)(*AUX_MU_IO);
    return r=='\r'? '\n':r;
}

void uart_send_string(char *s){
    for(int i = 0; s[i]!='\0'; ++i){
        if((char)s[i] == '\n'){
            uart_send('\r');
        }
        uart_send((char)s[i]);
    }
}

void uart_binary_to_hex(unsigned int d){
    unsigned int n;
    int c;
    uart_send_string("0x");
    for(c=28; c>=0; c-=4){
        //get the highest 4 bits
        n = (d>>c)&0xF;
        // if n = 0-9 => '0' - '9', 10-15 => 'A'-'F'
        n += (n>9? 0x37: 0x30);
        uart_send(n);
    }

}
/*
10 + 0x37 = 0x41 (which is the ASCII code for the character 'A')
11 + 0x37 = 0x42 (which is the ASCII code for the character 'B')
12 + 0x37 = 0x43 (which is the ASCII code for the character 'C')
13 + 0x37 = 0x44 (which is the ASCII code for the character 'D')
14 + 0x37 = 0x45 (which is the ASCII code for the character 'E')
15 + 0x37 = 0x46 (which is the ASCII code for the character 'F')
*/