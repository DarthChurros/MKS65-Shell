#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/errno.h>
#include <sys/wait.h>
#include "execute.h"
#include "parse.h"

extern volatile int in_backup;
extern volatile int out_backup;

int exec_std(char** cmd) {
  if (!strcmp(cmd[0], "exit")) {
    return 1;
  }

  // cd
  else if (!strcmp(cmd[0], "cd")) {
    if (cmd[1]) {
      char dir[256];
      strcpy(dir, "./");
      strcat(dir, cmd[1]);
      chdir(dir);
    }
  }

  // creating subprocess
  else if (fork()) {
    wait(0);
  }

  // executing input
  else if (execvp(cmd[0], cmd) == -1) {
    printf("%d: %s\n",errno, strerror(errno));
  }

  return 0;
}

int exec_redir(char** cmd, char** files, int* modes) {

  if (feof(stdin)) return 1;

  signal(SIGINT, int_handler);

  if (!*files) return (exec_std(cmd));

  // char** p;
  // int* q;
  // for (p = cmd; *p; p++) printf("cmd: \'%s\'\n", *p);
  // for (p = files; *p; p++) printf("file: \'%s\'\n", *p);
  // for (q = modes; *q; q++) printf("mode: \'%d\'\n", *q);

  int use_in = 1;
  int i;
  for (i = 0; !i || files[i-1]; i++) {
    int in;
    if (!files[i] && use_in) {
      // printf("using stdin\n");
      dup2(in_backup, STDIN_FILENO);
      if (feof(stdin)) return 1;

    } else if (modes[i] == 1) {
      use_in = 0;
      in = open(files[i], O_RDONLY);
      dup2(in, STDIN_FILENO);
      close(in);
    }
    int use_out = 1;
    int j;
    for (j = 0; !j || files[j-1]; j++) {
      if (j && j == i) continue;

      int out;

      if (!files[j]) {
        if (use_out) {
          // printf("using stdout\n");
          dup2(out_backup, STDOUT_FILENO);
          if (exec_std(cmd)) return 1;
        }
      } else {
        if (modes[j] == 2) {
          use_out = 0;
          out = open(files[j], O_WRONLY | O_CREAT | O_TRUNC, 0644);

        } else if (modes[j] == 3) {
          use_out = 0;
          out = open(files[j], O_WRONLY | O_APPEND);

        } else {
          // printf("nope\n");
          continue;
        }
        // char* in_name = files[i];
        // if (!files[i]) in_name = "stdin";
        // printf("using %s into %s\n", in_name, files[j]);
        dup2(out, STDOUT_FILENO);
        close(out);
        if (exec_std(cmd)) return 1;
      }
    }
  }
  dup2(in_backup, STDIN_FILENO);
  dup2(out_backup, STDOUT_FILENO);

  return 0;
}

void int_handler(int sig) {
  dup2(in_backup, STDIN_FILENO);
  dup2(out_backup, STDOUT_FILENO);
  fpurge(stdin);
  fpurge(stdout);
  printf("\b\b  \nsmash § ");
  fflush(stdout);
}

int exec_pipe(char** cmd1, char** cmd2) {
  return 0;
}
