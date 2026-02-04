#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(){
    int sd = socket(AF_INET, SOCK_STREAM, 0);
    if( sd < 0){
        std::cout << "Nie powiodło się socket()" << std::endl;
        return 1;
    }
    
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(5000)
    };
    if( inet_pton( AF_INET, "127.0.0.1", &addr.sin_addr ) <= 0 ){
        std::cout << "Nie powiodło się inet_pton()" << std::endl;
        return 1;
    }

    socklen_t len = sizeof(addr);
    if( bind(sd, (struct sockaddr *)  &addr, len) < 0){
        std::cout << "Nie powiodło się bind()" << std::endl;
        return 1;
    }

    if( listen(sd, 5) < 0){
        std::cout << "Nie powiodło się listen()" << std::endl;
        return 1;
    }

    while(1){
        std::cout << "Czekam na połączenie" << std::endl;
       
        struct sockaddr_in client = { };
        const int clientSocket = accept( sd,( struct sockaddr * ) & client, & len );
        if( clientSocket < 0 )
        {
            std::cout << "Nie powiodło accept()" << std::endl;
            continue;
        }
       
        char buffer[8000];
        if( recv( clientSocket, buffer, sizeof( buffer ), 0 ) <= 0 )
        {
            std::cout << "Nie powiodło się revc()" << std::endl;
            return 1;
        }

        std::cout <<  "Wiadomosc od kilenta: " << buffer << std::endl;
       
        char prefix[] = "Wiadomosc z servera: ";
        strcat(prefix, buffer);
        if( send( clientSocket, prefix, strlen( prefix), 0 ) <= 0 )
        {
            std::cout << "Nie powiodło się send()" << std::endl;
            return 1;
        }
       
        shutdown( clientSocket, SHUT_RDWR );
    }

    if( shutdown( sd, SHUT_RDWR) != 0){
        std::cout << "Nie powiodło się shutdown()" << std::endl;
        return 1;
    }

    return 0;
}