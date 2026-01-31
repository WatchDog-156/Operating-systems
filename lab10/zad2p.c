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
    sem_t* sem_empty = sem_open("/sem_empty", O_CREAT, 0666, 10);
    sem_t* sem_full = sem_open("/sem_full", O_CREAT, 0666, 0);
    sem_t* sem_mutex = sem_open("/sem_mutex", O_CREAT, 0666, 1);

    const char* bufor = "/bufor_shm";
    const int SIZE = sizeof(SharedData);

    int fd = shm_open(bufor, O_CREAT | O_RDWR, 0666);
    if (fd == -1) { perror("shm_open"); return 1; }

    if (ftruncate(fd, SIZE) == -1) { perror("ftruncate"); return 1; }

    SharedData *ptr = (SharedData*)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) { perror("mmap"); return 1; }

    ptr->idx_read = 0;
    ptr->idx_write = 0;

    int data_to_send = 0;

    while(1){
        sem_wait(sem_empty);
        sem_wait(sem_mutex);
        ptr->dana[ptr->idx_write] = data_to_send;
        printf("[Producent] Przesłana dana: %d, indeks: %d\n", data_to_send, ptr->idx_write);
        ptr->idx_write = (ptr->idx_write+1)%10;
        data_to_send++;
        sem_post(sem_mutex);
        sem_post(sem_full);
        usleep(300000);
    }

    munmap(ptr, SIZE);
    close(fd);
    shm_unlink(bufor);

    sem_close(sem_empty);
    sem_close(sem_full);
    sem_close(sem_mutex);
    sem_unlink("/sem_empty");
    sem_unlink("/sem_full");
    sem_unlink("/sem_mutex");

    return 0;    
}