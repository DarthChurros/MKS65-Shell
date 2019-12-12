#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "parse.h"

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
