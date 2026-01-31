#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>  
#include "common.h"


int main(){
    const char* shm = "/lab_shm";
    const int SIZE = sizeof(SharedData); 

    int fd = shm_open(shm, O_CREAT | O_RDWR, 0666);
    if (fd == -1) { perror("shm_open"); return 1; }

    if (ftruncate(fd, SIZE) == -1) { perror("ftruncate"); return 1; }

    SharedData *ptr = (SharedData*)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) { perror("mmap"); return 1; }

    for(int i=0; i<1000000; i++)
        ptr->licznik++;

    munmap(ptr, SIZE);
    close(fd);
    printf("[Konsument] Zakończyłem pracę.\n");

    return 0;   
}