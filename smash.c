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
      // char** pipe_array = parse_args(*p0, '|');
      // char** p1 = pipe_array;
      // while (*p1) {
      //   char** redir_array = parse_args(*p1, '<');
      // }

      char** args = parse_args(*p0, " ");

      if (exec_std(args)) return 0;

      free(args);

      p0++;
    }
  }
  return 0;
}
