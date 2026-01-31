#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX_BUF 8000

int main(void)
{
  int pipefd[2];
  int pipefd2[2];
  int cpid;
  char buf[MAX_BUF+1];
  char text[MAX_BUF+1];
  
  if(pipe(pipefd) == -1){
    perror("Wykryto błąd");
    exit(-1);
  }
  if(pipe(pipefd2) == -1){
    perror("Wykryto błąd");
    close(pipefd[0]);  
    close(pipefd[1]); 
    exit(-1);
  }

  cpid = fork();
  if (cpid<0){
    perror("Wykryto błąd");
    close(pipefd[0]);  
    close(pipefd[1]); 
    close(pipefd2[0]);  
    close(pipefd2[1]); 
    exit(-1);
  }else if (cpid == 0) 
  {            
    close(pipefd[1]);                       /* Close unused write end */
    close(pipefd2[0]);
    while (1){
        int read_bites = 0;
        memset(buf, 0, sizeof(buf));
        while(read_bites<MAX_BUF){
            int result = read(pipefd[0], buf+read_bites, MAX_BUF-read_bites);
            if(result==-1){
                perror("Wykryto błąd");
                close(pipefd[0]);
                close(pipefd2[1]); 
                exit(-1);
            }
            if(result==0)
                break;
            read_bites+=result;
            printf("1\n");
            printf("%s\n", buf);
        }
        printf("%s\n",buf);
        buf[read_bites]='\0';
        printf("%s\n",buf);

        int written_bites = write(pipefd2[1], buf, read_bites);
        if(written_bites==-1){
            perror("Wykryto błąd");
            close(pipefd[0]);
            close(pipefd2[1]); 
            exit(-1);
        }
        if(written_bites!=read_bites){
            perror("Wykryto błąd");
            close(pipefd[0]);
            close(pipefd2[1]); 
            exit(-1);
        }
        printf("a\n");

        if(strcmp(buf, "end") == 0)
            break;
        printf("b\n");
    }
    printf("aabb\n");
    close(pipefd[0]);
    close(pipefd2[1]);
    exit(0);
  } 
  else 
  {
    close(pipefd[0]);                       /* Close unused read end */
    close(pipefd2[1]);
    while(1){
        //fflush(stdout);
        scanf("%s", text);

        int written_bites = write(pipefd[1], text, strlen(text));
        if(written_bites==-1){
            perror("Wykryto błąd");
            close(pipefd[1]);
            close(pipefd2[0]); 
            exit(-1);
        }
        if(written_bites!=(int)strlen(text)){
            perror("Wykryto błąd");
            close(pipefd[1]);
            close(pipefd2[0]); 
            exit(-1);
        }
        printf("123");

        int read_bites = 0;
        memset(buf, 0, sizeof(buf));
        while(read_bites<MAX_BUF){
            int result = read(pipefd2[0], buf+read_bites, MAX_BUF-read_bites);
            if(result==-1){
                perror("Wykryto błąd");
                close(pipefd[1]);  
                close(pipefd2[0]); 
                exit(-1);
            }
            if(result==0)
                break;
            read_bites+=result;
            printf("2\n");
        }
        buf[read_bites]='\0';

        printf("Odebrałem wiadomość: %s\n", buf);
        if(strcmp(text, "end") == 0)
            break;
    }
    close(pipefd[1]);  
    close(pipefd2[0]);                     
    wait(NULL);                             /* Wait for child */
    exit(0);
  }
  printf("abcd");
}