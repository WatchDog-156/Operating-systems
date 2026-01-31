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

    int read_data;

    while(1){
        sem_wait(sem_full);
        sem_wait(sem_mutex);
        read_data = ptr->dana[ptr->idx_read];
        printf("[Konsument] Odebrana dana: %d, indeks: %d\n", read_data, ptr->idx_read);
        ptr->idx_read = (ptr->idx_read+1)%10;
        sem_post(sem_mutex);
        sem_post(sem_empty);
        usleep(500000);
    }

    munmap(ptr, SIZE);
    close(fd);
    shm_unlink(bufor);

    sem_close(sem_empty);
    sem_close(sem_full);
    sem_close(sem_mutex);

    return 0;   
}