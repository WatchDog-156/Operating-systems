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

  //int flags = fcntl(pipefd[0], F_GETFL, 0);
  //fcntl(pipefd[0], F_SETFL, flags | O_NONBLOCK);
  
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
    close(pipefd[1]);                       /* Close unused write end */
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
    buf[MAX_BUF]='\0';
    printf("Odebrałem wiadomość: %s\n", buf);
    close(pipefd[0]);
    exit(0);
  } 
  else 
  {
    close(pipefd[0]);                       /* Close unused read end */
    int written_bites = write(pipefd[1], "Luke, to ja jestem twoim ojcem!", strlen("Luke, to ja jestem twoim ojcem!"));
    if(written_bites==-1){
        perror("Wykryto błąd");
        close(pipefd[1]);
        exit(-1);
    }
    if(written_bites!=strlen("Luke, to ja jestem twoim ojcem!")){
        perror("Wykryto błąd");
        close(pipefd[1]);
        exit(-1);
    }

    close(pipefd[1]);                       
    wait(NULL);                             /* Wait for child */
    exit(0);
  }
}