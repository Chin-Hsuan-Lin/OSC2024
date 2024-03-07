#include "gpio.h"
extern volatile unsigned int mbox[32];

//Mailbox channels
#define MB_POWER_MANAGMENT      0
#define MB_FRAME_BUFFER         1
#define MB_VIRTUAL_UART         2
#define MB_VCHIQ                3
#define MB_LEDS                 4
#define MB_BUTTONS              5
#define MB_TOUCH                6
#define MB_UNUSED               7
#define MB_ARM_TO_VC            8
#define MB_VC_TO_ARM            9

//buffer status code
#define BUFFER_REQUEST_PROCESS  0x00000000
#define BUFFER_REQUEST_SUCCUSS  0x80000000
#define BUFFER_REQUEST_FAILED   0x80000001

#define MB_RESPONSE   0x80000000
#define MB_FULL       0x80000000
#define MB_EMPTY      0x40000000

//Mailbox tags
#define MB_TAG_GET_FRAME_REVISION   0x1
//Hardware
#define MB_TAG_GET_BOARD_MODEL      0x00010001
#define MB_TAG_GET_BOARD_REVISION   0x00010002
#define MB_TAG_GET_MAC_ADDRESS      0x00010003
#define MB_TAG_GET_BOARD_SERIAL     0x00010004
#define MB_TAG_GET_ARM_MEMORY       0x00010005
#define MB_TAG_GET_VC_MEMORY        0x00010006
#define MB_TAG_GET_CLOCKS           0x00010007

#define MB_TAG_REQUEST_CODE 0x00000000
#define MB_TAG_LAST_BYTE    0x00000000

//Mailbox register
//https://jsandler18.github.io/extra/mailbox.html
#define MB_BASE     MMIO_BASE + 0xB880
#define MB_READ     ((volatile unsigned int*)(MB_BASE+0x00))
#define MB_POLL     ((volatile unsigned int*)(MB_BASE+0x10))
#define MB_SENDER   ((volatile unsigned int*)(MB_BASE+0x14))
#define MB_STATUS   ((volatile unsigned int*)(MB_BASE+0x18))
#define MB_CONFIG   ((volatile unsigned int*)(MB_BASE+0x1C))
#define MB_WRITE    ((volatile unsigned int*)(MB_BASE+0x20))

int mbox_call(unsigned char c);