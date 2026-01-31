#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

// Funkcja wypisująca napis znak po znaku - nasza SEKCJA KRYTYCZNA
void drukuj_powoli(const char *napis) {
    for (int i = 0; napis[i] != '\0'; i++) {
        // Wypisanie jednego znaku
        putchar(napis[i]);

        // Wymuszenie opróżnienia bufora (żeby znak pojawił się od razu)
        fflush(stdout); 

        // Symulacja pracy / oddanie czasu procesora
        usleep(10000); // 10 milisekund opóźnienia
    }
}

int main() {
    sem_t* sem = sem_open("/guard", O_CREAT, 0666, 1);
    
    // Rozwidlenie procesu
    pid_t pid = fork();

    if (pid == 0) {
        // --- Proces Potomny ---
        sem_wait(sem);
        drukuj_powoli("Proces POTOMNY: To jest wazny komunikat.\n");
        sem_post(sem);
        
        sem_close(sem);
        exit(0);
    } else if (pid > 0) {
        // --- Proces Macierzysty ---
        sem_wait(sem);
        drukuj_powoli("Proces MACIERZYSTY: A to jest inny tekst.\n");
        sem_post(sem);

        // Czekanie na zakończenie dziecka
        wait(NULL);
    } else {
        perror("Błąd fork");
        return 1;
    }

    sem_close(sem);
    sem_unlink("/guard");

    return 0;
}