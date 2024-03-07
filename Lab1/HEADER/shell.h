#ifndef _SHELL_H_
#define _SHELL_H_

#define CLI_MAX_CMD 4
#define CMD_MAX_LEN 32
#define MSG_MAX_LEN 128

typedef struct CLI_CMDS{
    char command[CMD_MAX_LEN];
    char help[MSG_MAX_LEN];
}CLI_CMDS;


#endif