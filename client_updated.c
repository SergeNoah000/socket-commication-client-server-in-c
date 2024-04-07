#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char *args[]) {
    struct sockaddr_in serv_addr;

    // Vérification du nombre de paramètres
    if (argc < 3) {
        fprintf(stderr, "Utilisation: client ADRESSE_SERVEUR PORT_SERVEUR\n");
        exit(EXIT_SUCCESS);
    }

    char *address, buffer[BUFFER_SIZE] = {0};
    address = args[1];
    long port = atoi(args[2]);
    int i = 0;
    while (1) {
        int sock = 0;

        // Creating socket file descriptor
        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            perror("Erreur lors de la création du socket\n");
            exit(EXIT_FAILURE);
        }

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);

        // Convertir l'adresse IP du texte en forme binaire
        if (inet_pton(AF_INET, address, &serv_addr.sin_addr) <= 0) {
            printf("Voici l'adresse: %s\n", address);
            perror("Adresse invalide / Adresse non prise en charge!");
            exit(EXIT_FAILURE);
        }

        // Connexion au serveur
        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            perror("Connexion échouée");
            exit(EXIT_FAILURE);
        }

        // Envoi de données au serveur
        sprintf(buffer, "Salutation %d, accepte ma connexion\n", i);
        send(sock, buffer, strlen(buffer), 0);
        printf("Message d'echo %d  envoyé au serveur.\n", i);

        // Réception de données du serveur
        recv(sock, buffer, BUFFER_SIZE, 0);
        printf("Réponse %d du serveur: %s\n\n", i, buffer);
	
        // Fermeture du socket
        close(sock);
	i++;
    }

    return 0;
} 