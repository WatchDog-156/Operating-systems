#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(){
    int soutfd = dup(STDOUT_FILENO);
    if(soutfd==-1){
        perror("Wykryto błąd");
        exit(-1);
    }

    int written_bites = write(soutfd, "Wypisano text za pomoca write\n", strlen("Wypisano text za pomoca write\n"));
    if(written_bites==-1){
        perror("Wykryto błąd");
        exit(-1);
    }
    if(written_bites!=strlen("Wypisano text za pomoca write\n")){
        perror("Wykryto błąd");
        exit(-1);
    }

    FILE* fakeout = fdopen(soutfd, "w");
    if(fakeout==NULL){
        printf("Nie udało się utworzyć fakeout\n");
        exit(-1);
    }

    printf("Wypisano text za pomoca domyslnego wyjscia\n");
    fprintf(stdout, "Wypisano text za pomoca stdout\n");
    fprintf(fakeout, "Wypisano text za pomoca fakeout\n");
}