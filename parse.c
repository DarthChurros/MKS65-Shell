#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "parse.h"

char** parse_args(char* ln, char* delim) {
  char* line = strip(ln);

  if (!line) return NULL;

  // char str_d[] = {delim, '\0'};
  int i = 0;
  int num_args = 1;
  while (line[i]) {
    if (strchr(delim, line[i])) {
      num_args++;
    }
    i++;
  }

  char** split = malloc((num_args + 1) * sizeof(char*));

  split[num_args] = NULL;
  char* arg = line;
  i = 0;

  while (arg) {
    line = strsep(&arg, delim);
    if (line[1]) {
      split[i] = line;
      i++;
    }
  }

  return split;
}

char* strip(char* str) {
  char* end;

  while (isspace(*str)) {
    *str = '\0';
    str++;
  }

  if(*str == 0) return str;

  end = str + strlen(str) - 1;
  while (end > str && isspace(*end)) {
    *end = '\0';
    end--;
  }

  return str;
}
