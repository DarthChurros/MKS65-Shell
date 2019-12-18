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
#include "pipe.h"

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
    printf("\nsmash § ");
    // takes input
    fgets(cmd, 256, stdin);

    // parsing by ; delimiter, splitting commands
    char ** cmd_array = parse_args(cmd, ';');
    char ** p0 = cmd_array;
    while (*p0) {
      // parsing by | delimiter, splitting pipe commands
      char ** pipe_array = parse_args(p0, '|');
      char ** p1 = pipe_array;
      while (*p1) {
        // parsing by ' ' delimiter, splitting words
        char ** args = parse_args(p1, ' ');
        // 'exit'
        if (!strcmp(args[0], "exit"))
        {
          return 0;
        }

        // cd
        if (!strcmp(args[0], "cd"))
        {
          char dir[256];
          strcpy(dir, "./");
          strcat(dir, args[1]);
          chdir(dir);
        }

        // creating subprocess
        if (fork())
        {
          wait(0);
        }
        else
        {
          // executing input
          execvp(args[0], args);
        }

        // freeing memory
        free(args);
      }
      free(pipe_array);
    }
    for (i = 0; i < cmd_ctr; i++) {
      // parseargs input
      // parsing in the case of a pipe
      char **pipe_parse = parse_args(cmd_array[i], '|');
      char **args = parse_args(pipe_parse[i], ' ');
      // char ** arg;
      // for (arg = args; *arg; arg++){
      //   printf("%p --> %s\n",arg, *arg);
      // }
      // printf("\nbruh\n\n");

      
  }
  return 0;
}
