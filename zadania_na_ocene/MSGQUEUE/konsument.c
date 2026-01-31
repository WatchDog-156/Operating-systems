#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <mqueue.h>
#include <errno.h>
#include <unistd.h>


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

int main(){
    const char* q1 = "/to_slave";
    const char* q2 = "/to_master";
    mqd_t mq1, mq2;
    struct mq_attr attr;
    attr.mq_flags = 0;         
    attr.mq_maxmsg = 10;       
    attr.mq_msgsize = sizeof(struct Zadanie);    
    attr.mq_curmsgs = 0;

    mq1 = mq_open(q1, O_CREAT | O_RDONLY, 0644, &attr);
    if (mq1 == (mqd_t)-1) {
        print_error("mq_open (konsument mq1)", errno);
        exit(-1);
    }

    mq2 = mq_open(q2, O_CREAT | O_WRONLY, 0644, &attr);
    if (mq2 == (mqd_t)-1) {
        print_error("mq_open (konsument mq2)", errno);
        exit(-1);
    }

    struct Zadanie message;
    int bytes_read;
    unsigned int priority;

    while(1){
        bytes_read = mq_receive(mq1, (char*)&message, sizeof(message), &priority);
        if(bytes_read==-1)
            print_error("mq_recive (konsument mq1)", errno);
        else{
            // if(message.id==-1)       // działa jedynie wtedy, gdy jest ta sama liczba producentów i konsumentów
            //     break;

            message.wynik = message.A+message.B;
            sleep(3);

            printf("-- KONSUMENT -- PID producenta: %d, mój PID: %d, id: %d, A: %d, B: %d, wynik: %d\n\n", message.masterPID, getpid(), message.id, message.A, message.B, message.wynik);
                
            if (mq_send(mq2, (char*)&message, sizeof(message), 1) == -1) {
                print_error("mq_recive (konsument mq2)", errno);
            }
            
        }
    }
    
    // mq_close(mq1);           // działa jedynie wtedy, gdy jest ta sama liczba producentów i konsumentów
    // mq_close(mq2);
    // exit(0);
}