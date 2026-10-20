
#ifndef MY_COMMANDS_H
#define MY_COMMANDS_H

#include <stdbool.h>

typedef char *(*CommandHandler)(char *args[]);

typedef struct {
    const char *name;
    CommandHandler handler;
} Command;

extern char program_dir[4096];

extern Command my_commands[];

bool find_command(char *command);
char *handleCommand(char *command, char *args[]);

// Built-in handlers.
char *handle_exit(char *args[]);
char *handle_pwd(char *args[]);
char *handle_cd(char *args[]);
char *handle_echo(char *args[]);
char *handle_type(char *args[]);

#endif
