#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
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
    if (execvp(cmd[0], cmd) == -1) {
      printf("Command not found!");
    }
  }
  return 0;
}

int exec_redir(char** cmd, char** files, int* modes) {

  int in = 0;
  int out = 0;

  int in_old = dup(fileno(stdin));
  int out_old = dup(fileno(stdout));

  int i;
  for (i = 0; files[i]; i++) {
    int fd;
    switch (modes[i]) {
      case 0:
        fd = open(files[i], O_RDONLY);
        if (!in) {
          dup2(fd, fileno(stdin));
          in = fd;
        } else {
          dup2(in, fd);
        }
        break;
      case 1:
        fd = open(files[i], O_WRONLY | O_CREAT, 0644);
        if (!out) {
          dup2(fd, fileno(stdout));
          out = fd;
        } else {
          dup2(out, fd);
        }
        break;
      case 2:
        fd = open(files[i], O_WRONLY | O_APPEND);
        if (!out) {
          dup2(fd, fileno(stdout));
          out = fd;
        } else {
          dup2(out, fd);
        }
        break;
      default:
        exit(1);
    }
  }

  exec_std(cmd);

  close(out);
  close(in);

  stdin = fdopen(in_old, "r");
  stdout = fdopen(in_old, "w");
  return 0;
}

int exec_pipe(char** cmd1, char** cmd2) {
  return 0;
}
