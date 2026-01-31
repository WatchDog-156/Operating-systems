#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <errno.h>
#include <unistd.h>

void print_error(const char *func_name, int errnum) {
    fprintf(stderr, "Błąd w %s: %s (kod: %d)\n", func_name, strerror(errnum), errnum);
}

void print_mq_attr(mqd_t mq, const char *queue_name) {
    struct mq_attr attr;
    
    if (mq_getattr(mq, &attr) == -1) {
        print_error("mq_getattr", errno);
        return;
    }
    
    printf("\nAtrybuty kolejki '%s':\n", queue_name);
    printf("  mq_flags   = %ld (%s)\n", attr.mq_flags, 
           (attr.mq_flags & O_NONBLOCK) ? "O_NONBLOCK" : "blokująca");
    printf("  mq_maxmsg  = %ld (max liczba komunikatów)\n", attr.mq_maxmsg);
    printf("  mq_msgsize = %ld (max rozmiar komunikatu [B])\n", attr.mq_msgsize);
    printf("  mq_curmsgs = %ld (aktualna liczba komunikatów)\n", attr.mq_curmsgs);
}

int main() {
    const char *queue_name = "/test_queue";
    mqd_t mq;
    struct mq_attr attr, old_attr;
    int result;
    
    printf("=== ZADANIE 2: Kolejki komunikatów POSIX ===\n");
    
    // 1. Tworzenie kolejki komunikatów
    printf("\n1. Tworzenie kolejki komunikatów...\n");
    
    // Ustawienie atrybutów początkowych
    attr.mq_flags = 0;          // 0 = blokująca (domyślnie), O_NONBLOCK = nieblokująca
    attr.mq_maxmsg = 10;        // maksymalna liczba komunikatów w kolejce
    attr.mq_msgsize = 2048;     // maksymalny rozmiar pojedynczego komunikatu
    attr.mq_curmsgs = 0;        // inicjalizacja (będzie aktualizowane przez system)
    
    // O_CREAT - utwórz jeśli nie istnieje
    // O_RDWR - dostęp do odczytu i zapisu
    // 0644 - uprawnienia: właściciel R/W, inni R
    mq = mq_open(queue_name, O_CREAT | O_RDWR, 0644, &attr);
    
    if (mq == (mqd_t)-1) {
        print_error("mq_open (tworzenie)", errno);
        return 1;
    }
    
    printf("Kolejka '%s' utworzona/pobrana (deskryptor: %d)\n", 
           queue_name, (int)mq);
    
    // 2. Sprawdzenie atrybutów
    printf("\n2. Sprawdzenie początkowych atrybutów...");
    print_mq_attr(mq, queue_name);
    
    // 3. Próba zmiany atrybutów
    printf("\n3. Próba zmiany atrybutów...\n");
    
    // Pobranie obecnych atrybutów
    if (mq_getattr(mq, &old_attr) == -1) {
        print_error("mq_getattr (przed zmianą)", errno);
        mq_close(mq);
        mq_unlink(queue_name);
        return 1;
    }
    
    // Próba zmiany flagi na nieblokującą
    old_attr.mq_flags = O_NONBLOCK;
    
    if (mq_setattr(mq, &old_attr, NULL) == -1) {
        print_error("mq_setattr", errno);
        printf("Uwaga: mq_setattr pozwala zmienić TYLKO mq_flags!\n");
        printf("mq_maxmsg i mq_msgsize NIE mogą być zmieniane po utworzeniu kolejki.\n");
    } else {
        printf("Pomyślnie zmieniono flagi kolejki na O_NONBLOCK\n");
        print_mq_attr(mq, queue_name);
        
        // Przywrócenie flagi blokującej
        old_attr.mq_flags = 0;
        mq_setattr(mq, &old_attr, NULL);
    }
    
    // 4. Test z zamknięciem i ponownym otwarciem
    printf("\n4. Test mq_close i ponowne otwarcie...\n");
    
    // Zamknięcie deskryptora
    if (mq_close(mq) == -1) {
        print_error("mq_close", errno);
    } else {
        printf("Kolejka zamknięta (deskryptor zwolniony).\n");
    }
    
    // Ponowne otwarcie - bez O_CREAT
    printf("\nPonowne otwarcie bez O_CREAT...\n");
    mq = mq_open(queue_name, O_RDWR);
    
    if (mq == (mqd_t)-1) {
        print_error("mq_open (bez O_CREAT)", errno);
        printf("Kolejka może nie istnieć po mq_close? Sprawdźmy...\n");
    } else {
        printf("Kolejka otwarta ponownie (deskryptor: %d).\n", (int)mq);
        print_mq_attr(mq, queue_name);
    }
    
    // 5. Różnica między mq_close a mq_unlink
    printf("\n5. Demonstracja różnicy mq_close vs mq_unlink...\n");
    
    // a) Tylko mq_close
    printf("a) Tylko mq_close (bez mq_unlink):\n");
    if (mq_close(mq) == -1) {
        print_error("mq_close", errno);
    } else {
        printf("   - mq_close wykonane\n");
        printf("   - Kolejka nadal istnieje w systemie\n");
        printf("   - Można ją otworzyć ponownie\n");
    }
    
    // Otwarcie ponownie do testu
    mq = mq_open(queue_name, O_RDWR);
    if (mq == (mqd_t)-1) {
        printf("   - Błąd otwarcia: %s\n", strerror(errno));
    } else {
        printf("   - Otwarto ponownie poprawnie\n");
    }
    
    // b) mq_unlink
    printf("\nb) mq_unlink (usuwanie z systemu):\n");
    if (mq_unlink(queue_name) == -1) {
        print_error("mq_unlink", errno);
    } else {
        printf("   - mq_unlink wykonane\n");
        printf("   - Kolejka oznaczona do usunięcia\n");
        printf("   - Zostanie usunięta gdy wszystkie procesy ją zamkną\n");
    }
    
    // c) Próba otwarcia po mq_unlink
    printf("\nc) Próba otwarcia po mq_unlink (z O_CREAT):\n");
    mqd_t mq2 = mq_open(queue_name, O_CREAT | O_RDWR, 0644, &attr);
    if (mq2 == (mqd_t)-1) {
        print_error("mq_open (po unlink)", errno);
    } else {
        printf("   - Utworzono NOWĄ kolejkę o tej samej nazwie\n");
        print_mq_attr(mq2, queue_name);
        mq_close(mq2);
    }
    
    // d) mq_close na oryginalnej kolejce (po unlink)
    printf("\nd) mq_close na oryginalnej kolejce (już po unlink):\n");
    if (mq != (mqd_t)-1) {
        if (mq_close(mq) == -1) {
            print_error("mq_close (po unlink)", errno);
        } else {
            printf("   - Ostatni proces zamknął kolejkę\n");
            printf("   - System fizycznie usuwa kolejkę teraz\n");
        }
    }
    
    // 6. Test z flagami przy ponownym uruchomieniu
    printf("\n6. Test flag O_CREAT, O_EXCL i O_RDONLY...\n");
    
    // Usunięcie ewentualnej istniejącej kolejki
    mq_unlink(queue_name);
    
    // a) Utworzenie z O_CREAT | O_EXCL
    printf("a) mq_open z O_CREAT | O_EXCL:\n");
    mq = mq_open(queue_name, O_CREAT | O_EXCL | O_RDWR, 0644, &attr);
    if (mq == (mqd_t)-1) {
        print_error("mq_open (O_CREAT|O_EXCL)", errno);
    } else {
        printf("   - Utworzono nową kolejkę (poprawnie)\n");
        mq_close(mq);
    }
    
    // b) Próba ponownego utworzenia z O_CREAT | O_EXCL
    printf("\nb) Ponowna próba z O_CREAT | O_EXCL:\n");
    mq = mq_open(queue_name, O_CREAT | O_EXCL | O_RDWR, 0644, &attr);
    if (mq == (mqd_t)-1) {
        printf("   - Błąd (oczekiwany): %s\n", strerror(errno));
        printf("   - O_EXCL zapobiega otwarciu istniejącej kolejki\n");
    }
    
    // c) Otwarcie tylko do odczytu
    printf("\nc) Otwarcie z O_RDONLY i próba zapisu:\n");
    mqd_t mq_ro = mq_open(queue_name, O_RDONLY);
    if (mq_ro == (mqd_t)-1) {
        print_error("mq_open (O_RDONLY)", errno);
    } else {
        printf("   - Otwarto do odczytu (deskryptor: %d)\n", (int)mq_ro);
        
        // Próba wysłania komunikatu (powinna się nie udać)
        char msg[] = "Testowa wiadomość";
        if (mq_send(mq_ro, msg, sizeof(msg), 1) == -1) {
            printf("   - mq_send nie powiódł się (oczekiwane): %s\n", strerror(errno));
        }
        mq_close(mq_ro);
    }
    
    // 7. Czyszczenie
    printf("\n7. Czyszczenie...\n");
    
    // Otwarcie z prawami zapisu aby móc usunąć
    mq = mq_open(queue_name, O_RDWR);
    if (mq != (mqd_t)-1) {
        mq_close(mq);
        if (mq_unlink(queue_name) == -1) {
            print_error("mq_unlink (final)", errno);
        } else {
            printf("Kolejka '%s' usunięta z systemu.\n", queue_name);
        }
    }
    
    printf("\n=== KONIEC PROGRAMU ===\n");
    printf("\nWNIOSKI:\n");
    printf("1. mq_setattr pozwala zmienić tylko flagę O_NONBLOCK\n");
    printf("2. mq_maxmsg i mq_msgsize są ustalane przy tworzeniu i niezmienne\n");
    printf("3. mq_close() zamyka tylko deskryptor w procesie\n");
    printf("4. mq_unlink() oznacza kolejkę do usunięcia z systemu\n");
    printf("5. Kolejki POSIX są trwałe (przetrwują zamknięcie deskryptora)\n");
    printf("6. Usunięcie fizyczne następuje po ostatnim mq_close() po unlink\n");
    
    return 0;
}