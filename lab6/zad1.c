#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(){
    int a = rand()%11;
    int b = 20+rand()%11;
    pid_t signal;
    printf("a = %d, b = %d\n", a, b);

    if(fork()==0){
        exit(a+b);
    }else{
        wait(&signal);
        printf("Zwrócony sygnał przez potomka to: %d\n", WEXITSTATUS(signal));
        return 0;
    }
}