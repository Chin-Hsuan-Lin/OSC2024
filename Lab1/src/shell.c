#include "power.h"
#include "uart.h"
#include "mailbox.h"
#include "shell.h"

struct CLI_CMDS cmd_list[CLI_MAX_CMD]=
{
    {.command="hello", .help="print Hello World!"},
    {.command="help", .help="print all supported command"},
    {.command="info", .help="get device information via mailbox"},
    {.command="reboot", .help="reboot the device"}
};

int cli_cmd_strcmp(const char*p1, const char* p2){
    while(*p1 && *p2 && *p1 == *p2){
        p1++;
        p2++;
    }
    return (*p1 - *p2); // if return 0 then two strings are equal.
}

void cli_cmd_clear(char* buffer, int lenth){
    for(int i = 0; i < lenth; ++i){
        buffer[i] ='\0';
    }
}

void cli_cmd_read(char* buffer){
    char c = '\0';
    int idx = 0;
    while(1){
        if(idx >= CMD_MAX_LEN) break;

        c = uart_recv();
        if(c == '\n'){
            uart_send_string("\r\n");
            break;
        }
        if(c>=0 && c<32) continue;
        if(c > 127) continue;
        buffer[idx++] = c;
        uart_send(c);
    }
    buffer[idx] = '\0';
}

void cli_cmd_exec(char* buffer){
    if(cli_cmd_strcmp(buffer, "hello") == 0){
        do_cmd_hello();
    }
    else if(cli_cmd_strcmp(buffer, "help") == 0){
        do_cmd_help();
    }
    else if(cli_cmd_strcmp(buffer, "info") == 0){
        do_cmd_info();
    }
    else if(cli_cmd_strcmp(buffer, "reboot") == 0){
        do_cmd_reboot();
    }
    else if(*buffer){
        uart_send_string(buffer);
        uart_send_string(": command not found\r\n");
    }
}

void cli_print_banner(){
    uart_send_string("==================================\r\n");
    uart_send_string("  Welcome to NYCU-OSC 2024 shell  \r\n");
    uart_send_string("==================================\r\n");
}

void do_cmd_help(){
    for(int i = 0; i < CLI_MAX_CMD; ++i){
        uart_send_string(cmd_list[i].command);
        uart_send_string("\t\t:");
        uart_send_string(cmd_list[i].help);
        uart_send_string("\r\n");
    }
}

void do_cmd_hello(){
    uart_send_string("Hello World!\r\n");
}

void do_cmd_info(){
    //print board revision
    mbox[0] = 7 * 4;
    mbox[1] = BUFFER_REQUEST_PROCESS;
    //tag begin
    mbox[2] = MB_TAG_GET_BOARD_REVISION;
    mbox[3] = 4; // maximum of request and response value buffer's length
    mbox[4] = MB_TAG_REQUEST_CODE;
    mbox[5] = 0;
    mbox[6] = MB_TAG_LAST_BYTE;
    mbox_call();
    uart_send_string("Hardware Revision\t: ");
    uart_binary_to_hex(mbox[5]);
    uart_send_string("\r\n");
    //print arm memory
    mbox[0] = 8 * 4;
    mbox[1] = BUFFER_REQUEST_PROCESS;
    mbox[2] = MB_TAG_GET_ARM_MEMORY;
    mbox[3] = 8;
    mbox[4] = MB_TAG_REQUEST_CODE;
    mbox[5] = 0;
    mbox[6] = 0;
    mbox[7] = MB_TAG_LAST_BYTE;
    mbox_call();
    uart_send_string("ARM Memory Base Address\t: ");
    uart_binary_to_hex(mbox[5]);
    uart_send_string("\r\n");
    uart_send_string("ARM Memory Size\t\t: ");
    uart_binary_to_hex(mbox[6]);
    uart_send_string("\r\n");
}

void do_cmd_reboot(){
    uart_send_string("Reboot in 5 seconds ... \r\n\r\n");
    volatile unsigned int* rst_addr = (unsigned int*)PM_RSTC;
    *rst_addr = PM_PASSWORD | 0x20;
    volatile unsigned int* wdg_addr = (unsigned int*)PM_WDOG;
    *wdg_addr = PM_PASSWORD | 5;
}