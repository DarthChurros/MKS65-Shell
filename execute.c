#include <stdlib.h>
#include <stdio.h>
#include "execute.h"

int exec_std(char** cmd) {
  if (!strcmp(args[0], "exit")) {
    return 1;
  }

  // cd
  else if (!strcmp(args[0], "cd")) {
    char dir[256];
    strcpy(dir, "./");
    strcat(dir, args[1]);
    chdir(dir);
  }

  // creating subprocess
  else if (fork()) {
    wait(0);
    return 0
  }

  // executing input
  else {
    execvp(args[0], args);
  }
}

void exec_redir(char* cmd, char** files) {

}

void exec_pipe(char* cmd1, char* cmd2) {

}
