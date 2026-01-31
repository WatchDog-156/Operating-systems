#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <mqueue.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>


struct Zadanie{
    int masterPID;
    int id;
    int A;
    int B;
    int wynik;
};


void print_error(const char *func_name, int errnum) {
    fprintf(stderr, "Błąd w %s: %s (kod: %d)\n", func_name, strerror(errnum), errnum);
}


int main(int argc, char *argv[]){
    if(argc!=2){
        printf("Zła liczba argumentów wywołania programu\n");
        exit(-1);
    }

    int liczba_zadan = atoi(argv[1]);
    if (liczba_zadan<=0){
        printf("Zła liczba zadań do wykonania\n");
        exit(-1);
    }

    const char* q1 = "/to_slave";
    const char* q2 = "/to_master";
    mqd_t mq1, mq2;
    struct mq_attr attr;
    attr.mq_flags = 0;         
    attr.mq_maxmsg = 10;       
    attr.mq_msgsize = sizeof(struct Zadanie);    
    attr.mq_curmsgs = 0;

    mq1 = mq_open(q1, O_CREAT | O_WRONLY | O_NONBLOCK, 0644, &attr);
    if (mq1 == (mqd_t)-1) {
        print_error("mq_open (producent mq1)", errno);
        exit(-1);
    }

    mq2 = mq_open(q2, O_CREAT | O_RDONLY, 0644, &attr);
    if (mq2 == (mqd_t)-1) {
        print_error("mq_open (producent mq2)", errno);
        exit(-1);
    }

    struct Zadanie message;
    int bytes_read;
    unsigned int priority;
    int wrong_send_mess = 0;
      
    srand(time(NULL));
    for(int i=0; i<liczba_zadan; i++){
        message.masterPID=getpid();
        message.id=i;
        message.A=rand()%1000;
        message.B=rand()%1000;
        message.wynik=-1;

        if (mq_send(mq1, (char*)&message, sizeof(message), 1) == -1) {
            print_error("mq_send (producent mq1)", errno);
            wrong_send_mess++;
        }
    }
    for(int i=0; i<liczba_zadan-wrong_send_mess; i++){
        bytes_read = mq_receive(mq2, (char*)&message, sizeof(message), &priority);
        if(bytes_read==-1)
            print_error("mq_recive (producent mq2)", errno);
        else
            printf("-- PRODUCENT -- PID producenta: %d, mój PID: %d, id: %d, A: %d, B: %d, wynik: %d\n\n", message.masterPID, getpid(), message.id, message.A, message.B, message.wynik);
    }
    printf("Producent zakończył wszystkie zadania\n");

    // message.masterPID = getpid();        // działa jedynie wtedy, gdy jest ta sama liczba producentów i konsumentów
    // message.id = -1; 
    // message.A = 0;
    // message.B = 0;
    // message.wynik = 0;
    // mq_send(mq1, (char*)&message, sizeof(message), 1);
 
    mq_unlink(q1);
    mq_unlink(q2);
    mq_close(mq1);
    mq_close(mq2);
    
}