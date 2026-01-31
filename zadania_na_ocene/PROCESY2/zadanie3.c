#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>

int main(){

    float *tablica;
    tablica=(float*)malloc(1000000*sizeof(float));
    if(tablica==NULL){
        printf("Nie utworzono tablicy\n");
        return 1;
    }

    srand(time(NULL));
    for(int i=0; i<1000000; i++)
        tablica[i]=((float)rand()/RAND_MAX)*2.0-1;    

    int childPID[10];
    for(int i=0; i<10; i++){
        int result = fork();
        if(result<0){
            printf("Nie utworzono potomka nr %d\n", i);
            free(tablica);
            return 1;
        }

        if(result==0){
            float local_sum=0;
            for(int el=0; el<100000; el++)
                local_sum+=tablica[el+100000*i];
            printf("Suma dla %d przedziału to %.4f\n", i, local_sum);
            
            char fileName[30];
            sprintf(fileName, "sum%d.txt", getpid());
            FILE* plik=fopen(fileName, "w");
            if(plik==NULL){
                printf("Nie udało się utworzyć pliku %s\n", fileName);
                free(tablica);
                return 1;
            }

            fprintf(plik, "%f", local_sum);
            fclose(plik);
            exit(0);
        }else
            childPID[i]=result;
    }

    for(int i=0; i<10; i++){
        wait(NULL);
    }

    float suma=0.0;
    float odczytanaDana;
    for(int i=0; i<10; i++){
        char fileName[30];
        sprintf(fileName, "sum%d.txt", childPID[i]);

        FILE* plik=fopen(fileName, "r");
        if(plik==NULL){
            printf("Nie udało się otworzyć pliku %s\n", fileName);
            free(tablica);
            return 1;
        }

        if(fscanf(plik, "%f", &odczytanaDana)==1)
            suma+=odczytanaDana;
        else 
            printf("Nie udało się odczytać danej z pliku %s\n", fileName);
        
        fclose(plik);
        if(remove(fileName)!=0)
            printf("Nie udało się usunąć pliku %s\n", fileName);
    }

    float srednia=suma/10.0;
    printf("Średnia dla całego zbioru to %.4f\n", srednia);
    free(tablica);
    return 0;
}