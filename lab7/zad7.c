#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_BUF 1024

int main(void)
{
  int pipefd[2];
  int cpid;
  char buf[MAX_BUF+1];
  
  if(pipe(pipefd) == -1){
    perror("Wykryto błąd");
    exit(-1);
  }

  cpid = fork();
  if (cpid<0){
    perror("Wykryto błąd");
    close(pipefd[0]);
    close(pipefd[1]);
    exit(-1);
  }else if (cpid == 0) 
  {         
    close(pipefd[0]);     
    
    char* task = "ls";
    
    int soutfd = dup2(pipefd[1], STDOUT_FILENO);
    if(soutfd==-1){
        perror("Wykryto błąd");
        close(pipefd[1]);
        exit(-1);
    }
    
    execlp(task, task, "-al", NULL);
  } 
  else 
  {
    close(pipefd[1]);

    int read_bites = 0;
    while(read_bites<MAX_BUF){
        int result = read(pipefd[0], buf+read_bites, MAX_BUF-read_bites);
        if(result==-1){
            perror("Wykryto błąd");
            close(pipefd[0]);
            exit(-1);
        }
        if(result==0)
            break;
        read_bites+=result;
    }
    buf[read_bites]='\0';

    printf("Rodzic odebral:\n");
    printf("%s\n", buf);
    printf("Koniec wiadomosci do rodzica\n");
    close(pipefd[0]);                     
    wait(NULL);                             
    exit(0);
  }
}