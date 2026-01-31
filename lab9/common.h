#ifndef COMMON_H
#define COMMON_H

typedef struct {
    volatile int licznik; // volatile zapobiega optymalizacjom kompilatora
} SharedData;

#endif