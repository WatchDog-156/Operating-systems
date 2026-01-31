#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdint.h>
#include <math.h>

int main(){
    int *tablica;
    tablica=(int*)malloc(1000000*sizeof(int));
    if(tablica==NULL){
        printf("Nie utworzono tablicy\n");
        return 1;
    }
    //int tablica[1000000];

    float srednia=0.0;
    int signal;

    for(int i=0; i<1000000; i++)
        tablica[i]=rand()%101;

    for(int i=0; i<10; i++){
        if(fork()==0){
            int local_sum=0;
            for(int el=0; el<100000; el++)
                local_sum+=tablica[el+100000*i];
            srednia=local_sum/100000.0;
            printf("Średnia dla %d przedziału to %.2f, w zaokrągleniu %.0f\n", i+1, srednia, round(srednia));
            exit(round(srednia));
        }
    }

    int suma=0;
    for(int i=0; i<10; i++){
        wait(&signal);
        suma+=WEXITSTATUS(signal);
    }
    srednia=suma/10.0;
    printf("Średnia dla całego zbioru to %.2f\n", srednia);
    free(tablica);
    return 0;
}