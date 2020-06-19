#include <sys/types.h>

#include <sys/socket.h>

#include <netinet/in.h>

#include <arpa/inet.h>

#include <string.h>

#include <unistd.h>

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;

#include <stdio.h>

#include <stdlib.h>

#define PORT 8080

int main(void) {
    int erreur = 0;
    char buff[1024];

    /* Socket et contexte d'adressage du serveur */
    SOCKADDR_IN sin;
    SOCKET sock;
    socklen_t recsize = sizeof(sin);

    /* Socket et contexte d'adressage du client */
    SOCKADDR_IN csin;
    SOCKET csock;
    socklen_t crecsize = sizeof(csin);

    int sock_err;

    if (!erreur) {
        /* Création d'une socket */
        sock = socket(AF_INET, SOCK_STREAM, 0);

        /* Si la socket est valide */
        if (sock != INVALID_SOCKET) {
            printf("La socket %d est maintenant ouverte en mode TCP/IP\n", sock);

            /* Configuration */
            sin.sin_addr.s_addr = htonl(INADDR_ANY); /* Adresse IP automatique */
            sin.sin_family = AF_INET; /* Protocole familial (IP) */
            sin.sin_port = htons(PORT); /* Listage du port */
            sock_err = bind(sock, (SOCKADDR * ) & sin, recsize);

            /* Si la socket fonctionne */
            if (sock_err != SOCKET_ERROR) {
                /* Démarrage du listage (mode server) */
                sock_err = listen(sock, 5);
                printf("Listage du port %d...\n", PORT);

                /* Si la socket fonctionne */
                if (sock_err != SOCKET_ERROR) {
                    /* Attente pendant laquelle le client se connecte */
                    printf("Patientez pendant que le client se connecte sur le port %d...\n", PORT);
                    csock = accept(sock, (SOCKADDR * ) & csin, & crecsize);
                    printf("Un client se connecte avec la socket %d de %s:%d\n", csock, inet_ntoa(csin.sin_addr), htons(csin.sin_port));
                } else
                    perror("listen");
            } else
                perror("bind");

            while (strcmp(buff, "quit") != 0) {
                if (recv(csock, buff, 1024, 0) != 0) {
                    printf("Message recu: %s\n", buff);
                    if (strcmp(buff, "coucou") == 0) {
                        sprintf(buff, "Bonjour");
                        send(csock, buff, strlen(buff) + 1, 0);
                        printf("Envoi du message : %s\n", buff);
                        
                    } else {
                        send(csock, buff, strlen(buff) + 1, 0);
                        printf("Envoi du message : %s\n", buff);
					}
                }
            }
            printf("Fermeture de la socket client\n");
            closesocket(csock);
            printf("Fermeture de la socket serveur\n");
            closesocket(sock);
            printf("Fermeture du serveur terminée\n");
            
        } else
            perror("socket");

    }

    return EXIT_SUCCESS;
}