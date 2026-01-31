#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int main()
{
    printf("Parent: start\n");
    if (fork() == 0)
    {
        printf("Child: start\n");
        sleep(10);
        printf("PPID: %d\n", getppid());
        sleep(30);
        printf("PPID: %d\n", getppid());
        printf("Child: end\n");
        exit(0);
    }
    sleep(30);
    printf("Parent: end\n");
    exit(0);
}