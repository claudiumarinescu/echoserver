/*
 * Echo Server
 * server.c
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>

#define BUFLEN 1500

int main(int argc, char *argv[])
{
    int listenfd = 0, connfd = 0, check = 0;
    char buf[BUFLEN];
    struct sockaddr_in serv_addr; 

    if(argc != 3) {
        printf("\n Usage: %s <ip> <port>\n", argv[0]);
        return 1;
    }

    // deschidere socket
    listenfd = socket(PF_INET, SOCK_STREAM, 0);
    if (listenfd == -1) {
        printf("[SERVER]-----Erorr opening socket!\n");
        return 1;
    }

    // completare informatii despre adresa serverului
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));
    if (inet_aton(argv[1], &(serv_addr.sin_addr)) == 0) {
        printf("[CLIENT]-----Error converting IP address!\n");
        return 1;
    }

    // legare proprietati de socket
    check = bind(listenfd,(struct sockaddr*)(&serv_addr),sizeof(serv_addr));
    if (check == -1) {
        printf("[SERVER]-----Error binding socket!\n");
        return 1;
    }

    // ascultare de conexiuni
    check = listen(listenfd, 2);
    if (check == -1) {
        printf("[SERVER]-----Error listening!\n");
        return 1;
    }

    // acceptarea unei conexiuni, primirea datelor, trimiterea raspunsului
    unsigned int serv_len = sizeof(serv_addr);
    connfd = accept(listenfd,(struct sockaddr*)(&serv_addr),&serv_len);
    if (connfd == -1) {
        printf("[SERVER]-----Error accepting connection!\n");
        return 1;
    }


    while (1) {
        check = recv(connfd,buf,BUFLEN,0);
        if (check == 0) {
            printf("[SERVER]-----Connection with CLIENT ended!\n");
            return 0;
        }
        if (check < 0) {
            printf("[SERVER]-----Error receiving message from CLIENT!\n");
            return 1;
        }

        check = send(connfd, buf, strlen(buf), 0);
        if (check == -1) {
            printf("[SERVER]-----Error sending message!\n");
            return 1;
        }
    }

    // inchidere socket
    close(connfd);

    return 0;
}
