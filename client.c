/*
 * Echo Server
 * client.c
 */

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#define BUFLEN 1500

int main (int argc, char *argv[]) {
    int sockfd = 0;
    int check = 0;
    char buf[BUFLEN];
    struct sockaddr_in serv_addr; 

    if (argc != 3) {
        printf("\n Usage: %s <ip> <port>\n", argv[0]);
        return 1;
    }

    // deschidere socket
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("[CLIENT]-----Erorr opening socket!\n");
        return 1;
    }

    // completare informatii despre adresa serverului
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));
    if (inet_aton(argv[1], &(serv_addr.sin_addr)) == 0) {
        printf("[CLIENT]-----Error converting IP address!\n");
        return 1;
    }

    // conectare la server
    check = connect(sockfd, (struct sockaddr*)(&serv_addr), sizeof(struct sockaddr));
    if (check == -1) {
        printf("[CLIENT]-----Error connecting to server!\n");
        return 1;
    }

    // citire de la tastatura, trimitere de cereri catre server, asteptare raspuns
    printf("===>>> ECHO SERVER DEMO <<<===\n");
    printf("You can end the connection by typing exit.\n");
    printf("You> ");
    fgets(buf, BUFLEN, stdin);
    buf[strlen(buf) - 1] = '\0';

    while (strcmp(buf, "exit") != 0) {
        check = send(sockfd, buf, strlen(buf), 0);
        if (check == -1) {
            printf("[CLIENT]-----Error sending message!\n");
            return 1;
        }

        check = recv(sockfd,buf,BUFLEN,0);
        if (check == 0) {
            printf("[CLIENT]-----Connection with SERVER ended!\n");
            return 0;
        }
        if (check < 0) {
            printf("[CLIENT]-----Error receiving message!\n");
            return 1;
        }
        printf("Server> %s\n", buf);

 
        printf("You> ");
        fgets(buf, BUFLEN, stdin);
        buf[strlen(buf) - 1] = '\0';
    }

    // inchidere socket
    close(sockfd);

    return 0;
}
