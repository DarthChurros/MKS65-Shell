#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "execute.h"

int exec_std(char** cmd) {
  if (!strcmp(cmd[0], "exit")) {
    return 1;
  }

  // cd
  else if (!strcmp(cmd[0], "cd")) {
    char dir[256];
    strcpy(dir, "./");
    strcat(dir, cmd[1]);
    chdir(dir);
  }

  // creating subprocess
  else if (fork()) {
    wait(0);
  }

  // executing input
  else {
    execvp(cmd[0], cmd);
  }
  return 0;
}

int exec_redir(char** cmd, char** files) {
  return 0;
}

int exec_pipe(char** cmd1, char** cmd2) {
  return 0;
}
