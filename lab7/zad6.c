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
    
    char* mess = "Wyslano wiadomosc";
    
    int soutfd = dup2(pipefd[1], STDOUT_FILENO);
    if(soutfd==-1){
        perror("Wykryto błąd");
        close(pipefd[1]);
        exit(-1);
    }
    
    printf("(Dziecko wysyla: %s)", mess);
    close(pipefd[1]);
    exit(0);
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

    printf("Rodzic odebral: %s\n", buf);
    close(pipefd[0]);                     
    wait(NULL);                             /* Wait for child */
    exit(0);
  }
}