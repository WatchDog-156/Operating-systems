Program główny przydziela na stercie tablicę zawierającą 1000000 elementów liczb zmiennoprzecinkowych - tworzona jest dynamiczna tablica float'ów. Następnie wypełnia ją losowymi wartościami z zakresu od -1 do 1.

- Po przygotowaniu tablicy tworzone jest 10 procesów potomnych. Zadaniem każdego z procesów potomnych jest obliczenie sumy elementów fragmentu tablicy:
    - potomek 0 — od indeksu 0 do 99999
    - potomek 1 — od indeksu 100000 do 199999
    - potomek 2 — od indeksu 200000 do 299999
    - ...
    - Po wyznaczeniu sumy procesy potomne zapisują ją w pliku o nazwie „sum\<PID\>.txt” i kończą swoją pracę.

- Proces nadrzędny czeka na zakończenie obliczeń przez procesy potomne, a następnie oblicza średnią wartość elementów tablicy na podstawie zawartości plików z wynikami częściowymi, po odczytaniu pliki są usuwane.