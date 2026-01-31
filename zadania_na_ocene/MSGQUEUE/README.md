Napisano dwa programy:

**- master (producent)** - generuje zadania i umieszcza je w kolejce do wykonania,

**- slave (konsument)** - wykonuje nadesłane zadania, wyświetla je i odsyła wynik z powrotem do producenta.

Zadania do wykonania opisane są przez specjalną strukturę - *Zadanie*.
Liczba zadań do obliczenia jest podawana jako parametr uruchomienia programu producenta. Zadania są generowane losowo.
Konsument pracuje w nieskończonej pętli. Odczytuje zadania z kolejki i wykonuje je. Po obliczeniu wyniku konsument wyświetla obiekt struktury *Zadanie* i odszyła go do z powrotem producentowi, który także go wyświetla.