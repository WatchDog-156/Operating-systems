#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>  
#include "common.h"
#include <semaphore.h>

int main(){
    const char* shm = "/lab_shm"; 
    const int SIZE = sizeof(SharedData);

    int fd = shm_open(shm, O_CREAT | O_RDWR, 0666);
    if (fd == -1) { perror("shm_open"); return 1; }

    if (ftruncate(fd, SIZE) == -1) { perror("ftruncate"); return 1; }

    SharedData *ptr = (SharedData*)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) { perror("mmap"); return 1; }

    sem_t* sem = sem_open("/guard", O_CREAT, 0666, 1);

    for(int i=0; i<1000000; i++){
        sem_wait(sem);
        ptr->licznik++;
        sem_post(sem);
    }

    munmap(ptr, SIZE);
    close(fd);
    printf("[Konsument] Zakończyłem pracę.\n");

    sem_close(sem);

    return 0;   
}