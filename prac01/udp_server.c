#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

void error_die(char *err)
{
    perror(err);
    exit(EXIT_FAILURE);
}

int main(void)
{
    printf("Configuring local address...\n");
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo *bind_address;
    if(getaddrinfo(0, "9001", &hints, &bind_address)) {
        error_die("getaddrinfo() failed.");
    }

    printf("Creating socket...\n");
    int socket_listen;
    socket_listen = socket(bind_address->ai_family, bind_address->ai_socktype,
                           bind_address->ai_protocol);
    if (socket_listen < 0) {
        error_die("socket() failed.");
    }

    printf("Binding socket to local address...\n");
    if (bind(socket_listen, bind_address->ai_addr, bind_address->ai_addrlen)) {
        error_die("bind() failed.");
    }

    freeaddrinfo(bind_address);

    fd_set master;
    FD_ZERO(&master);
    FD_SET(socket_listen, &master);
    int max_socket = socket_listen;

    printf("Waiting for conection...\n");

    while(1) {
        fd_set reads;
        reads = master;

        if (select(max_socket + 1, &reads, 0,0,0) < 0) {
            error_die("select() failed.");
        }

        if (FD_ISSET(socket_listen, &reads)) {
            struct sockaddr_storage client_address;
            socklen_t client_len = sizeof(client_address);

            char read[1024];
            int bytes_received = recvfrom(socket_listen, read, 1024, 0,
                                          (struct sockaddr*)&client_address,
                                          &client_len);

            if (bytes_received < 1) {
                error_die("connection closed.");
            }

            for(int i = 0; i < bytes_received; i++) {
                if (i % 16 == 0) printf("\n");
                printf("%02x:", read[i]);
            }

            char* msg = "Hello I have received.";
            sendto(socket_listen, msg, strlen(msg), 0,
                   (struct sockaddr*)&client_address, client_len);
            
        }
    }

    close(socket_listen);
    printf("Finished.\n");
    return 0;
}
