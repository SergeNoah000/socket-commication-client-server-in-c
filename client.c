#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Creating socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation error");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPaddress from text to binary form
if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
perror("Invalid address/ Address not supported");
exit(EXIT_FAILURE);
}

// Connecting to the server
if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
perror("Connection failed");
exit(EXIT_FAILURE);
}

// Sending data to the server
sprintf(buffer, "Hello 1, Server!");
send(sock, buffer, strlen(buffer), 0);
printf("Hello message sent to the server\n");

// Receiving data from the server
recv(sock, buffer, BUFFER_SIZE, 0);
printf("Message received: %s\n", buffer);



// Sending data to the server
sprintf(buffer, "Hello 2, Server!");
send(sock, buffer, strlen(buffer), 0);
printf("Hello message sent to the server\n");

// Receiving data from the server
recv(sock, buffer, BUFFER_SIZE, 0);
printf("Message received: %s\n", buffer);



// Sending data to the server
sprintf(buffer, "Hello 3, Server!");
send(sock, buffer, strlen(buffer), 0);
printf("Hello message sent to the server\n");

// Receiving data from the server
recv(sock, buffer, BUFFER_SIZE, 0);
printf("Message received: %s\n", buffer);


// Sending data to the server
sprintf(buffer, "Hello 4, Server!");
send(sock, buffer, strlen(buffer), 0);
printf("Dernier message envoyé");

// Receiving data from the server
recv(sock, buffer, BUFFER_SIZE, 0);
printf("Message received: %s\n", buffer);


// Closing the socket
close(sock);
return 0;
}