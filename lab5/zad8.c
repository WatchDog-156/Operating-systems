#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    if (argc!=2){
        printf("Zla liczba argumentow!!!\n");
        exit(1);
    }

    printf("Parent: start\n");
    printf("Parent, PID: %d, PPID: %d\n", getpid(), getppid());
    for (int i=0; i<atoi(argv[1]); i++){
        if (fork() == 0)
        {
            printf("Child %d: start\n", i+1);
            printf("Child %d, PID: %d, PPID: %d\n", i+1, getpid(), getppid());
            printf("Child %d: end\n", i+1);
            exit(0);
        }
    }
    sleep(1);
    wait(NULL);
    printf("Parent: end\n");
    exit(0);
}   