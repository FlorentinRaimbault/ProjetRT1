#include <sys/types.h>

#include <sys/socket.h>

#include <netinet/in.h>

#include <arpa/inet.h>

#include <unistd.h>

#include <string.h>

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
    char message[1024];
    char buff[1024];
    SOCKET sock;
    SOCKADDR_IN sin;

    if (!erreur) {
        /* Création de la socket */
        sock = socket(AF_INET, SOCK_STREAM, 0);

        /* Configuration de la connexion */
        sin.sin_addr.s_addr = inet_addr("127.0.0.1");
        sin.sin_family = AF_INET;
        sin.sin_port = htons(PORT);

        /* Si le client arrive à se connecter */
        if (connect(sock, (SOCKADDR * ) & sin, sizeof(sin)) != SOCKET_ERROR)
            printf("Connexion à %s sur le port %d\n", inet_ntoa(sin.sin_addr), htons(sin.sin_port));
        else
            printf("Impossible de se connecter\n");

        while (strcmp(buff, "quit") != 0) {
            printf("Entrer le message à envoyer : ");
            scanf("%s", message);
            strcpy(buff, message);
            send(sock, buff, strlen(buff) + 1, 0);
            printf(" Envoi du message : %s\n", buff);
            recv(sock, buff, 1024, 0);
            printf("Message recu: %s\n", buff);
        }
        /* On ferme la socket précédemment ouverte */
        printf("Fermeture de la socket\n");
        closesocket(sock);
    }

    return EXIT_SUCCESS;
}
