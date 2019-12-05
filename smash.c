#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <dirent.h>

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
char** parse_args(char* line);

int main() {
  char cmd[256];
  while (1) {
    printf("\nsmash § ");
    fgets(cmd, 256, stdin);
    char** args = parse_args(cmd);
    if (!strcmp(args[0], "exit")) {
      break;
    }
    // char** p = args;
    // printf("[");
    // while (*p) {
    //   printf("%s, ", *p);
    //   p += sizeof(char**);
    // }
    // printf("]");
    if (fork()) {
      wait(0);
    } else {
      execvp(args[0], args);
    }
    free(args);
  }
  return 0;
}

char** parse_args(char* line) {
  int i = 0;
  int num_args = 0;
  while (line[i] != '\0') {
    if (line[i] == ' '){
      num_args++;
    }
    i++;
  }
  char** split = calloc(num_args + 1, sizeof(char*));
  split[num_args] = NULL;
  char* arg = line;
  i = 0;

  while (arg) {
    strsep(&arg, " ");
    split[i] = line;
    line = arg;
    i++;
  }
  split[i-1] = strsep(&split[i-1], "\n");

  return split;
}
