#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    if (fork() == 0) // child
    {
        printf("Child: %d, Parent: %d\n", getpid(), getppid());
        for (int i=1; i<11; i++){
            printf("Child: %d\n", i);
            sleep(1);
        }
        printf("Child: end\n");
        exit(1);
    }
    else // parent
    {
        printf("Me: %d\n", getpid());
        for (int i=1; i<11; i++){
            printf("Parent: %d\n", i);
            sleep(1);
        }
        wait(NULL);
        printf("Parent: end\n");
        return 0;
    }
}