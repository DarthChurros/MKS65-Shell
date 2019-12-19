#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/errno.h>
#include <sys/wait.h>
#include "execute.h"
#include "parse.h"

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

  // char** p;
  // int* q;
  // for (p = cmd; *p; p++) printf("cmd: \'%s\'\n", *p);
  // for (p = files; *p; p++) printf("file: \'%s\'\n", *p);
  // for (q = modes; *q; q++) printf("mode: \'%d\'\n", *q);

  int use_in = 1;
  int use_out = 1;

  int in_old = dup(fileno(stdin));
  int out_old = dup(fileno(stdout));

  int i;
  for (i = 0; files[i] || files[i-1]; i++) {
    int in;
    if (!files[i] && use_in) {
      // printf("using stdin\n");
      stdin = fdopen(in_old, "r");

    } else if (modes[i] == 1) {
      use_in = 0;
      in = open(files[i], O_RDONLY);
      dup2(in, fileno(stdin));
    }
    int j;
    for (j = 0; files[j] || files[j-1]; j++) {
      int out;

      if (!files[j]) {
        if (use_out) {
          // printf("using stdout\n");
          stdout = fdopen(out_old, "w");
        }
        int ret = exec_std(cmd);
        stdout = fdopen(out_old, "w");
        stdin = fdopen(in_old, "r");

        if (ret) return 1;
        return 0;

      } else if (modes[j] == 2) {
        use_out = 0;
        out = open(files[j], O_WRONLY | O_CREAT | O_TRUNC, 0644);

      } else if (modes[j] == 3) {
        use_out = 0;
        out = open(files[j], O_WRONLY | O_APPEND);

      }
      dup2(out, fileno(stdout));

      close(out);
    }

    close(in);
  }
  stdin = fdopen(in_old, "r");
  stdout = fdopen(out_old, "w");

  return 0;
}

int exec_pipe(char** cmd1, char** cmd2) {
  return 0;
}
