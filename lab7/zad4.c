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
        exit(-1);
    }

    srand(time(NULL));
    for(int i=0; i<1000000; i++)
        tablica[i]=((float)rand()/RAND_MAX)*2.0-1;    

    int pipefd[10][2];

    for(int i=0; i<10; i++){
        if(pipe(pipefd[i]) == -1){
            perror("Wykryto błąd");
            for(int j=0; j<i; j++)
                close(pipefd[j][0]);
            free(tablica);
            exit(-1);
        }

        int result = fork();
        if(result<0){
            close(pipefd[i][1]);
            for(int j=0; j<=i; j++)
                close(pipefd[j][0]);
            printf("Nie utworzono potomka nr %d\n", i);
            free(tablica);
            exit(-1);
        }if(result==0){
            for(int j=0; j<=i; j++)
                close(pipefd[j][0]);


            float local_sum=0;
            for(int el=0; el<100000; el++)
                local_sum+=tablica[el+100000*i];
            printf("Suma dla %d przedziału to %.4f\n", i, local_sum);
            
            int written_bites = write(pipefd[i][1], &local_sum, sizeof(local_sum));
            if(written_bites==-1){
                perror("Wykryto błąd");
                close(pipefd[i][1]);
                free(tablica);
                exit(-1);
            }
            if(written_bites!=sizeof(local_sum)){
                perror("Wykryto błąd");
                close(pipefd[i][1]);
                free(tablica);
                exit(-1);
            }

            close(pipefd[i][1]);
            free(tablica);
            exit(0);
        }else
            close(pipefd[i][1]);
    }

    for(int i=0; i<10; i++){
        wait(NULL);
    }

    float suma=0.0;
    for(int i=0; i<10; i++){
        int read_bites = 0;
        float odczytanaDana;
        while(read_bites<sizeof(odczytanaDana)){
            int result = read(pipefd[i][0], &odczytanaDana+read_bites, sizeof(odczytanaDana)-read_bites);
            if(result==-1){
                perror("Wykryto błąd");
                close(pipefd[i][0]);
                free(tablica);
                exit(-1);
            }
            if(result==0)
                break;
            read_bites+=result;
        }
        suma+=odczytanaDana;
        close(pipefd[i][0]);
    }

    float srednia=suma/10.0;
    printf("Średnia dla całego zbioru to %.4f\n", srednia);
    free(tablica);
    exit(0);
}