#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#define C_SIZE 100

char * trimspace(char * a){
  int count = 0;
  char * ret = 0;
  while (a[count] == 32){
    count++;
  }
  ret = a+count;
  count = strlen(a)-1;
  while (a[count] == 32){
    count--;
  }
  a[count+1]=0;
  return ret;
}

void exec(char * line) {
    int pid, i = 0;
    char* command[C_SIZE];

    while (command[i] = strsep(&line, " ")) i++;
    //printf("%s\n", command[0]);
    if (! strcmp(command[0],"cd") ) {
      if (chdir(command[1]) == -1) printf("Error: %s", strerror(errno));
    }
    else if (! strcmp(command[0],"exit") ) exit(0);
    else {
      if ( (pid = fork()) == -1) printf("Error: %s", strerror(errno));
      else {
        if (pid) {
          wait(0);
        }
        else {
          if ( execvp(command[0], command) == -1) {
            if (errno != 2) printf("Error: %s", strerror(errno));
            else printf("Error: Not a command");
          }
          exit(0);
        }
      }
    }
}

void redirect(char * line, char *b){
  int pid, i = 0;
  char* command[C_SIZE];

  while (command[i] = strsep(&line, " ")) i++;
  //printf("%s\n", command[0]);
  if (! strcmp(command[0],"cd") ) {
    if (chdir(command[1]) == -1) printf("Error: %s", strerror(errno));
  }
  else if (! strcmp(command[0],"exit") ) exit(0);
  else {
    if ( (pid = fork()) == -1) printf("Error: %s", strerror(errno));
    else {
      if (pid) {
        wait(0);
      }
      else {
        int fd = open(b, O_CREAT|O_RDWR|O_TRUNC, 0644);
        int stdout = dup(1);
        dup2(fd,1);
        if ( execvp(command[0], command) == -1) {
          if (errno != 2) printf("Error: %s", strerror(errno));
          else printf("Error: Not a command");
        }
        dup2(stdout,1);
        exit(0);
      }
    }
  }
}

void aredirect(char * line, char *b){
  int pid, i = 0;
  char* command[C_SIZE];

  while (command[i] = strsep(&line, " ")) i++;
  //printf("%s\n", command[0]);
  if (! strcmp(command[0],"cd") ) {
    if (chdir(command[1]) == -1) printf("Error: %s", strerror(errno));
  }
  else if (! strcmp(command[0],"exit") ) exit(0);
  else {
    if ( (pid = fork()) == -1) printf("Error: %s", strerror(errno));
    else {
      if (pid) {
        wait(0);
      }
      else {
        int fd = open(b, O_RDONLY, 0644);
        if(fd==-1){
          printf("error: no such file");
          exit(0);
        }
        int stdin = dup(0);
        dup2(fd,0);
        if ( execvp(command[0], command) == -1) {
          if (errno != 2) printf("Error: %s", strerror(errno));
          else printf("Error: Not a command");
        }
        dup2(stdin,0);
        exit(0);
      }
    }
  }
}


void parse(char * line) {
  char del = 0;
  int i = 0;
  char * a;
  char * b;
  while ( line[i] ) {
    if (line[i] == '<' || line[i] == '>' || line[i] == '|' || line[i] == ';') del = line[i];
    i++;
  }

  if (!del){
    exec(line);
    return;
  }

  a = strsep(&line, "><|;");
  b = strsep(&line, "><|;");

  a = trimspace(a);
  b = trimspace(b);

  if (del == ';'){
    exec(a);
    exec(b);
    return;
  }

  if (del == '>'){
    redirect(a,b);
    return;
  }

  if (del == '<'){
    aredirect(a,b);
    return;
  }

  if (del == '|'){
    redirect(a, ".temp");
    aredirect(b, ".temp");
    execlp("rm","rm",".temp",NULL);
    return;
  }
}
