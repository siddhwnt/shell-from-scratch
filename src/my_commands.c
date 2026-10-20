#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>

#include "my_commands.h"
#include "main.h"

char program_dir[4096];

#define OUTPUT_SIZE 4096

Command my_commands[] = {
    {"pwd", handle_pwd},
    {"cd", handle_cd},
    {"echo", handle_echo},
    {"exit", handle_exit},
    {"type", handle_type}
};

#define MY_COMMANDS_COUNT (sizeof(my_commands) / sizeof(my_commands[0]))

bool isDirectoryExists(const char *path)
{
    struct stat sb;

    if (stat(path, &sb) == 0 && S_ISDIR(sb.st_mode))
    {
        return true;
    }

    return false;
}


bool find_command(char *command)
{
    for (int i = 0; i < (int)MY_COMMANDS_COUNT; i++)
    {
        if (strcmp(my_commands[i].name, command) == 0)
        {
            return true;
        }
    }

    return false;
}


// ===========================
// 
// Command handlers
//
// ===========================

char *handle_exit(char *args[])
{
    (void)args;  // This handler doesn't need its arguments.
    exit(EXIT_SUCCESS);
}


char *handle_type(char *args[])
{
  char *output = malloc(4096);

  if (output == NULL)
  {
    perror("malloc");
    return NULL;
  }

  output[0] = '\0';

  int i = 0;

  while (args[i] != NULL)
  {
    char *keyword = args[i];

    if (find_command(keyword))
    {
      strcat(output, keyword);
      strcat(output, " is a shell builtin\n");
    }
    else if (isInPath(keyword))
    {
      char keyword_location[4096];

      strcpy(keyword_location, path_locations[current_path_count]);
      strcat(keyword_location, "/");
      strcat(keyword_location, keyword);

      strcat(output, keyword);
      strcat(output, " is ");
      strcat(output, keyword_location);
      strcat(output, "\n");
    }
    else
    {
      strcat(output, keyword);
      strcat(output, ": not found\n");
    }

    i++;
  }

  return output;
}


char *handle_pwd(char *args[])
{
    (void)args;

    char *output = malloc(4096);

    if (output == NULL) {
        perror("malloc");
        return NULL;
    }

    if (getcwd(output, 4096) == NULL) {
        perror("getcwd");
        free(output);
        return NULL;
    }

    size_t len = strlen(output);
    output[len] = '\n';
    output[len + 1] = '\0';

    return output;
}

char *handle_echo(char *args[])
{
  char *output = malloc(OUTPUT_SIZE);

  if (output == NULL)
  {
    perror("malloc");
    return NULL;
  }

  output[0] = '\0';

  int i = 0;

  while (args[i] != NULL)
  {
    strcat(output, args[i]);

    if (args[i + 1] != NULL)
      strcat(output, " ");

    i++;
  }
  strcat(output, "\n");
  return output;
}

char *handle_cd(char *args[])
{
    int arg_count = 0;

    while (args[arg_count] != NULL)
        arg_count++;

    if (arg_count > 1)
    {
        fprintf(stderr, "cd: too many arguments\n");
        return NULL;
    }

    char *directory;

    if (arg_count == 0)
        directory = getenv("HOME");
    else
        directory = args[0];

    if (strcmp(directory, "~") == 0)
        directory = getenv("HOME");

    if (chdir(directory) != 0)
    {
        fprintf(stderr, "cd: %s: No such file or directory\n", directory);
        return NULL;
    }

    getcwd(program_dir, sizeof(program_dir));

    return NULL;
}


char *handleCommand(char *command, char *args[])
{
    for (int i = 0; i < (int)MY_COMMANDS_COUNT; i++)
    {
        if (strcmp(my_commands[i].name, command) == 0)
        {
            return my_commands[i].handler(args);
        }
    }

    return NULL;
}