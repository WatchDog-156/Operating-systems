#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

int main()
{
    for (int i=0; i<10; i++){
        if (fork() == 0)
        {
            printf("Child %d: start\n", i+1);
            printf("Child %d: end\n", i+1);
            exit(0);
        }
    }
    printf("Parent: before sleep()\n");
    sleep(30);
    printf("Parent: before wait()\n");
    wait(NULL);
    printf("Parent: end\n");
    exit(0);
}   