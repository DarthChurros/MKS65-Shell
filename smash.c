#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <ctype.h>
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
char** parse_args(char* line, char delim);
char* strip(char* str);

int main() {
  char cmd[256];
  while (1) {
    // printing cursor for new line
    printf("\nsmash § ");
    // takes input
    fgets(cmd, 256, stdin);

    // checking for ; delimiter
    int i = 0;
    int cmd_ctr = 1;
    for (i = 0; i < strlen(cmd); i++) {
      cmd_ctr += cmd[i] == ';';
    }

    // printf("parsing\n");

    char ** cmd_array = parse_args(cmd, ';');

    for (i = 0; i < cmd_ctr; i++) {
      // parseargs input
      char **args = parse_args(cmd_array[i], ' ');
      // char ** arg;
      // for (arg = args; *arg; arg++){
      //   printf("%p --> %s\n",arg, *arg);
      // }
      // printf("\nbruh\n\n");

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
  }
  return 0;
}

char** parse_args(char* line, char delim) {
  line = strip(line);

  if (!line) return NULL;

  char str_d[] = {delim, '\0'};

  int i = 0;
  int num_args = 0;
  while (line[i] != '\0') {
    if (line[i] == delim){
      num_args++;
    }
    i++;
  }
  char** split = calloc(num_args + 1, sizeof(char*));
  split[num_args] = NULL;
  char* arg = line;
  i = 0;

  while (arg) {
    strsep(&arg, str_d);
    split[i] = line;
    line = arg;
    i++;
  }
  split[i] = NULL;

  return split;
}

char* strip(char* str) {
  char *end;

  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)
    return str;

  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  end[1] = '\0';

  return str;
}
