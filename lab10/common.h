#ifndef COMMON_H
#define COMMON_H

typedef struct {
    volatile int dana[10]; // volatile zapobiega optymalizacjom kompilatora
    volatile int idx_read;
    volatile int idx_write;
} SharedData;

#endif