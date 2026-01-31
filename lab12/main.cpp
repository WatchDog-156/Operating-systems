#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(){
    int sd = socket(AF_INET, SOCK_STREAM, 0);
    
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5000);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int con_status = connect(sd, (struct sockaddr*)&addr, sizeof(addr));
    if(con_status != 0){
        std::cout << "Nie powiodło się connect()" << std::endl;
        return 1;
    }

    int wr_status = write(sd, "Hello, World!!!\n", sizeof("Hello, World!!!\n"));
    if(wr_status != sizeof("Hello, World!!!\n")){
        std::cout << "Nie powiodło się write()" << std::endl;
        return 1;
    }

    char buf[8000];
    int rd_status = read(sd, buf, 8000);
    if(rd_status == 0){
        std::cout << "Nie powiodło się read()" << std::endl;
        return 1;
    }

    std::cout << "Odczytano: " << buf;
    return 0;
}