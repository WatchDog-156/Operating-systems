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
        printf("Child: end\n");
        exit(0);
    }
    else
    {
        sleep(30);
        printf("Parent: before wait()\n");
        wait(NULL);
        printf("Parent: end\n");
        exit(0);
    }
}