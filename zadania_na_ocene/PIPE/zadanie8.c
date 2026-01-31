#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define MAX_BUF 4096

void parse_text(char* cmd, char** arg) {
    int i = 0;
    char* text = strtok(cmd, " \t\n");
    while (text != NULL && i < 99) {
        arg[i] = text;
        text = strtok(NULL, " \t\n");
        i++;
    }
    arg[i] = NULL;
}

void execute_run(char *command) {
    char *arg[100];

    while (*command == ' ' || *command == '\t') 
        command++;
    
    char buf[MAX_BUF];
    strncpy(buf, command, MAX_BUF - 1);
    buf[MAX_BUF - 1] = '\0';
    parse_text(buf, arg);
    
    if (arg[0] == NULL) {
        printf("Błąd run: brak polecenia do uruchomienia\n");
        exit(-1);
    }
    execvp(arg[0], arg);

    perror("Błąd run: błąd wywołania polecenia");
    exit(-1);
}

void execute_redirect(char *command) {
    char *to_position = strstr(command, " to ");
    if (to_position == NULL) {
        printf("Błąd redirect: brak słowa 'to'\n");
        exit(-1);
    }
    
    *to_position = '\0';
    char *cmd1 = command;
    while (*cmd1 == ' ' || *cmd1 == '\t') 
        cmd1++;
    char *cmd2 = to_position + 4;
    while (*cmd2 == ' ' || *cmd2 == '\t') 
        cmd2++;

    char buf1[MAX_BUF];
    strncpy(buf1, cmd1, MAX_BUF - 1);
    buf1[MAX_BUF - 1] = '\0';
    char buf2[MAX_BUF];
    strncpy(buf2, cmd2, MAX_BUF - 1);
    buf2[MAX_BUF - 1] = '\0';
    
    char *arg1[100];
    parse_text(buf1, arg1);
    char *arg2[100];
    parse_text(buf2, arg2);
    
    if (arg1[0] == NULL || arg2[0] == NULL) {
        printf("Błąd redirect: nieprawidłowe polecenia\n");
        exit(-1);
    }
    
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("Błąd pipe");
        exit(-1);
    }
    
    int pid1 = fork();
    if (pid1<0) {
        perror("Błąd fork");
        close(pipefd[0]);
        close(pipefd[1]);
        exit(-1);
    }else if (pid1==0) {
        close(pipefd[0]);

        int soutfd = dup2(pipefd[1], STDOUT_FILENO);
        if(soutfd==-1){
            perror("Błąd dup2");
            close(pipefd[1]);
            exit(-1);
        }

        close(pipefd[1]);
        execvp(arg1[0], arg1);

        perror("Błąd redirect: złe wywołanie komendy 1");
        exit(-1);
    }
    
    int pid2 = fork();
    if (pid2<0) {
        perror("Błąd fork");
        close(pipefd[0]);
        close(pipefd[1]);
        exit(-1);
    }else if (pid2==0) {
        close(pipefd[1]);

        int soutfd = dup2(pipefd[0], STDIN_FILENO);
        if(soutfd==-1){
            perror("Błąd dup2");
            close(pipefd[1]);
            exit(-1);
        }

        close(pipefd[0]);
        execvp(arg2[0], arg2);

        perror("Błąd redirect: złe wywołanie komendy 2");
        exit(-1);
    }
    
    close(pipefd[0]);
    close(pipefd[1]);
    
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    
    exit(0);
}

void run_command(char *command){
    int cpid = fork();

    if(cpid<0){
        perror("Błąd fork");
        exit(-1);
    }else if(cpid==0){
        if(strncmp(command, "run ", 4)==0)
            execute_run(command+4);
        else if(strncmp(command, "redirect ", 9)==0)
            execute_redirect(command+9);
        else{
            printf("Nieznane polecenie: %s\n", command);
            exit(-1);
        }
    }else
        wait(NULL);
}

int main(void) {
    char command[MAX_BUF];

    while (1) {
        printf("mini-shell > ");
        if (fgets(command, MAX_BUF, stdin) == NULL){
            printf("\n");
            break;
        }
        
        int len = strlen(command);
        if (len > 0 && command[len-1] == '\n'){
            command[len-1] = '\0';
        }
        if (strlen(command)== 0){
            continue;
        }

        if (strcmp(command, "exit") == 0)
            break;
        
        run_command(command);
    }
    exit(0);
}