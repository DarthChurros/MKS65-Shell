#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <dirent.h>
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

int main() {
  char cmd[256];
  while (1) {
    // printing cursor for new line
    printf("smash § ");
    // takes input
    fgets(cmd, 256, stdin);

    // parsing by ; delimiter, splitting commands
    char** cmd_array = parse_args(cmd, ";");
    char** p0 = cmd_array;

    while (*p0) {
      // char** pipe_array = parse_args(*p0, "|");
      // char** p1 = pipe_array;
      // while (*p1) {
      //   p1++;
      // }
      char** c;
      for (c = p0; *c; c++) printf("\t%s\n", *c);

      char** redir_array = parse_args(*p0, "<>");

      for (c = redir_array; *c; c++) printf("\t%s\n", *c);

      char* d;
      int modes[sizeof(redir_array)/sizeof(char*) - 1];

      int i = 0;

      for (d = *p0; *d; d++) {
        if (*d == '>') {
          if (d[1] == '>') {
            modes[i++] = 2;
          } else {
            modes[i++] = 1;
          }
        }
        if (*d == '<') {
          modes[i++] = 0;
        }
      }

      char** args = parse_args(*redir_array, " ");

      printf("______\n");
      for (c = args; *c; c++) printf("%s\n", *c);

      if (exec_redir(args, &redir_array[1], modes)) return 0;

      free(args);
      free(redir_array);

      p0++;
    }
    free(cmd_array);
  }
  return 0;
}
