#ifndef EXECUTE_H
#define EXECUTE_H


void exec_std(char* cmd);
void exec_redir(char* cmd, char** files);
void exec_pipe(char* cmd1, char* cmd2);

#endif
