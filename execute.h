#ifndef EXECUTE_H
#define EXECUTE_H


int exec_std(char** cmd);
int exec_redir(char** cmd, char** files, int* modes);
int exec_pipe(char** cmd1, char** cmd2);
void int_handler(int sig);

#endif
