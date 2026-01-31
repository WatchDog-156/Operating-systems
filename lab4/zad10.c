#include <stdlib.h>     // Dla EXIT_SUCCESS, EXIT_FAILURE
#include <unistd.h>     // Dla open, close, read, write
#include <fcntl.h>      // Dla flag O_...
#include <stdio.h>      // Dla perror, fprintf, stderr
#include <errno.h>      // Dla errno
#include <sys/stat.h>   // Wymagane dla trybu w open()

#define MAX 512

int main(int argc, char* argv[])
{
    char buf[MAX];
    int desc_zrod, desc_cel;
    int lbajt;

    if (argc < 3)
    {
        fprintf(stderr, "Za malo argumentow. Uzyj:\n");
        fprintf(stderr, "%s [<plik zrodlowy>] <plik docelowy>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Używamy open() zamiast historycznego creat()
    // 0640 - uprawnienia żądane (rw-r-----), ale ostateczne uprawnienia
    // będą zmodyfikowane przez umask procesu
    desc_cel = open(argv[argc-1], O_WRONLY | O_CREAT | O_TRUNC, 0640);
    if (desc_cel == -1)
    {
        perror("Blad utworzenia pliku docelowego");
        exit(EXIT_FAILURE);
    }

    for (int nr_zrod = 1; nr_zrod < argc - 1; nr_zrod++) {
        desc_zrod = open(argv[nr_zrod], O_RDONLY);
        if (desc_zrod == -1)
        {
            perror("Blad otwarcia pliku zrodlowego");
            exit(EXIT_FAILURE);
        }

        while((lbajt = read(desc_zrod, buf, MAX)) > 0)
        {
            int zapisano = 0;
            int do_zapisu = lbajt;

        // Pętla zapewniająca zapis wszystkich danych
        while (zapisano < do_zapisu) 
        {
            int wynik = write(desc_cel, buf + zapisano, do_zapisu - zapisano);
            if (wynik == -1) 
            {
                perror("Blad zapisu pliku docelowego");
                close(desc_zrod);
                close(desc_cel);
                exit(EXIT_FAILURE);
            }
            zapisano += wynik;
        } 
        }       
        // Jeśli lbajt == -1, pętla zakończyła się błędem odczytu
        if (lbajt == -1)
        {
            perror("Blad odczytu pliku zrodlowego");
            exit(EXIT_FAILURE);
        }

        if (close(desc_zrod) == -1)
        {
            perror("Blad zamkniecia pliku zrodlowego");
            exit(EXIT_FAILURE);
        }
    }

    if (close(desc_cel) == -1){
        perror("Blad zamkniecia pliku docelowego");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}