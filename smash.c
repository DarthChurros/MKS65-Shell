#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/errno.h>
#include <unistd.h>
#include <signal.h>
#include <dirent.h>
#include <sys/ioctl.h>
#include "parse.h"
#include "execute.h"

/*
Every function you write must have a function header describing the arguments, return value and what the function does.
Use modular design liberally. This is a large program.
You should have a readme file with the following:
     A description of what features your shell implements
     A description of what features you attempted to implement but were unsuccessful
     Any bugs or things you want me to know about your program
          I am much more forgiving of reported bugs than bugs I discover while testing
     A copy of every function header
     An example of the readme file can be found in the github repository for this assignment
This project should use separate C files as necessary.
The project should be built using make and a makefile, I will only compile by using make.

Try starting with these restrictions on input:
     To make your life easier, you could require that all items on the command line are to be separated by a single space.
     When parsing the line, it might help to count the number of tokens (individual pieces), before parsing.
     You can restrict the locations and ordering of > and <.
     You can limit piping (|) to a single pipe.

Other features
*/

volatile int in_backup;
volatile int out_backup;
char cmd[256];
char cmd_copy[256];

int main() {
  in_backup = dup(STDIN_FILENO);
  out_backup = dup(STDOUT_FILENO);

  signal(SIGINT, int_handler);

  while (!feof(stdin)) {

    // printing cursor for new line
    printf("smash § ");

    // takes input
    fgets(cmd, 256, stdin);
    strncpy(cmd_copy, cmd, 256);

    if (feof(stdin)) return 0;
    if (!strncmp(cmd, "\n", 256)) continue;

    // parsing by ; delimiter, splitting commands
    char** cmd_array = parse_args(cmd, ";");
    char** p0 = cmd_array;

    while (*p0) {
      // char** pipe_array = parse_args(*p0, "|");
      // char** p1 = pipe_array;
      // while (*p1) {
      //   p1++;
      // }


      char** redir_array = parse_args(*p0, "<>");

      // for (c = redir_array; *c; c++) printf("\t\'%s\'\n", *c);

      int modes[sizeof(redir_array)/sizeof(char*) + 1];

      char** p1;
      for (p1 = redir_array; *p1; p1++) *p1 = strip(*p1);

      // printf("%lu\n", sizeof(redir_array)/sizeof(int));

      int i = 0;

      // printf("running\n");
      char* d;
      for (d = &(cmd_copy[1]); *d; d++) {
        if (*d == '>') {
          if (d[1] == '>') {
            modes[i++] = 3;
            d++;
          } else {
            modes[i++] = 2;
          }
        }
        if (*d == '<') {
          modes[i++] = 1;
        }
      }
      modes[i] = 0;

      // int* n;
      // for (n = modes; *n; n++) printf("%d\n", *n);

      char** args = parse_args(*redir_array, " ");

      // printf("______\n");
      // char** c;
      // for (c = args; *c; c++) printf("\t\'%s\'\n", *c);

      // if (exec_std(args)) return 0;

      if (exec_redir(args, &redir_array[1], modes)) return 0;
      free(args);
      free(redir_array);

      p0++;
    }
    free(cmd_array);
    fpurge(stdin);
    *cmd = '\0';
    *cmd_copy = '\0';
  }
  return 0;
}
