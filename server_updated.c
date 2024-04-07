// #include <arpa/inet.h>
// #include <netinet/in.h>
// #include <pthread.h>
// #include <stdbool.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <sys/socket.h>
// #include <unistd.h>
// #include <fcntl.h>
// #include <sys/stat.h>
// #include <asm-generic/socket.h>
// #include <time.h>

// #define PORT 8080
// #define BUFFER_SIZE 1024
// #define LOG_FILE "client_messages.log"


// int active_threads = 0;
// pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;





// void *handle_client(void *arg);

// int main() {
//     int server_fd, new_socket;
//     struct sockaddr_in address;
//     int opt = 1;
//     int addrlen = sizeof(address);
//     pthread_t thread_id;

//     // Creating socket file descriptor
//     if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
//         perror("socket failed");
//         exit(EXIT_FAILURE);
//     }

//     // Forcefully attaching socket to the port
//     if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR , &opt,
//                    sizeof(opt))) {
//         perror("setsockopt error");
//         exit(EXIT_FAILURE);
//     }
//     address.sin_family = AF_INET;
//     address.sin_addr.s_addr = INADDR_ANY;
//     address.sin_port = htons(PORT);

//     // Forcefully attaching socket to the port
//     if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
//         perror("bind failed");
//         exit(EXIT_FAILURE);
//     }

//     // Listening for incoming connections
//     if (listen(server_fd, 3) < 0) {
//         perror("listen");
//         exit(EXIT_FAILURE);
//     }

//     printf("Server listening on port %d\n", PORT);

//     while (true) {
//         // Accepting incoming connections
//         if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
//                                  (socklen_t *)&addrlen)) < 0) {
//             perror("accept");
//             exit(EXIT_FAILURE);
//         }

//         printf("New connection from %s on port %d\n",
//                inet_ntoa(address.sin_addr), ntohs(address.sin_port));
//         //pthread_t *nouveau_thread = (pthread_t *) malloc(sizeof(pthread_t));

//         // Creating a new thread for the client
//         if (pthread_create(&thread_id, NULL, handle_client,
//                            &new_socket) < 0) {
//             perror("could not create thread");
//             exit(EXIT_FAILURE);
//         }

//         // Attente de la fin du thread avant d'accepter une nouvelle connexion
//         pthread_detach(thread_id);
//     }

//     return 0;
// }

// void *handle_client(void *arg) {
//     int new_socket = *(int *)arg;
//     pthread_mutex_lock(&mutex);
//     active_threads++;
//     pthread_mutex_unlock(&mutex);
//     char buffer[BUFFER_SIZE] = {0};
//     struct sockaddr_in client_address;
//     socklen_t client_address_size =sizeof(client_address);

//     // Getting the client's IP address
//     getpeername(new_socket, (struct sockaddr *)&client_address,
//     &client_address_size);

//     // Receiving data from the client
//     recv(new_socket, buffer, BUFFER_SIZE, 0);

//     // Writing the client's IP address and message to a file
//     int fd = open(LOG_FILE, O_APPEND | O_CREAT | O_WRONLY, 0644);
//     char *log_message = malloc(sizeof(char ) * 1200);
//     pthread_t id_thread = pthread_self();
//     time_t date ;
//     time(&date);

//     sprintf(log_message, "[ %s ] Client IP: %s, Message: %s, thread id: %ld  sur %d\n",
//     ctime(&date),
//     inet_ntoa(client_address.sin_addr), buffer, id_thread, active_threads);
//     write(fd, log_message, strlen(log_message));
//     free(log_message);
//     close(fd);
    
//     // Sending data back to the client
//     char *message  =   malloc(sizeof(char ) * 1200);
//     sprintf(message, "Message bien recu: %s ", buffer);
//     send(new_socket, message, 1024, 0);
//     fprintf(stdout, "[%d]:Reponse { %s } envoyé !\n", active_threads, message);
//     free(message);
//     // Closing the connection
//     close(new_socket);
//     pthread_mutex_lock(&mutex);
//     active_threads--;
//     pthread_mutex_unlock(&mutex);
//     pthread_exit(NULL);
// }   











#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
// #include <asm-generic/fcntl.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>

#define PORT 8080
#define MAX_THREADS 5
#define BUFFER_SIZE 1024
#define LOG_FILE "connections_log.log"

typedef struct {
    int client_socket;
    struct sockaddr_in client_address;
} Connection;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
Connection *connections_queue;
int queue_size = 0;
int queue_capacity = 10; // Adjust this based on your requirements
int active_threads = 0;

void *handle_connection(void *arg);

void clear_last_lines(int num_lines);

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt error");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listening for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    // Initialize connections queue
    connections_queue = (Connection *)malloc(sizeof(Connection) * queue_capacity);

    // Initialize thread pool
    pthread_t threads[MAX_THREADS];
    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_create(&threads[i], NULL, handle_connection, NULL);
    }

    while (true) {
        // Accepting incoming connections
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        clear_last_lines(5);

        printf("\033[1;32m"); // Vert gras
        printf("Active threads: [ %d ]\n", active_threads);
        printf("\033[0m"); // Réinitialisation de la couleur

        printf("New connection from %s on port %d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));


        // Add the new connection to the queue
        pthread_mutex_lock(&mutex);
        if (queue_size >= queue_capacity) {
            queue_capacity *= 2;
            connections_queue = realloc(connections_queue, sizeof(Connection) * queue_capacity);
        }
        connections_queue[queue_size].client_socket = new_socket;
        connections_queue[queue_size].client_address = address;
        queue_size++;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }

    return 0;
}

void *handle_connection(void *arg) {
    while (true) {
        pthread_mutex_lock(&mutex);
        while (queue_size == 0) {
            pthread_cond_wait(&cond, &mutex);
        }
        active_threads++;
        int client_socket = connections_queue[queue_size - 1].client_socket;
        struct sockaddr_in client_address = connections_queue[queue_size - 1].client_address;
        queue_size--;
        pthread_mutex_unlock(&mutex);

        char buffer[BUFFER_SIZE] = {0};

        // Receiving data from the client
        ssize_t bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            close(client_socket);
            continue;
        }

        // Writing the client's IP address and message to stdout
        printf("Received message from client %s:%d: %s\n", inet_ntoa(client_address.sin_addr),
               ntohs(client_address.sin_port), buffer);

        // Sending data back to the client
        char *message = malloc(sizeof(char)*65);
        sprintf( message, "Message bien reçu [nbre_threads_actifs: %d].",  active_threads);
        send(client_socket, message, strlen(message), 0);

        // Writing the client's IP address and message to a file
        int fd = open(LOG_FILE, O_APPEND | O_CREAT | O_WRONLY, 0644);
        char *log_message = malloc(sizeof(char ) * 1200);
        time_t date ;
        time(&date);

        sprintf(log_message, "[ %s ] Client IP: %s, Message: %s, thread id: %d  sur %d\n",
        ctime(&date),
        inet_ntoa(client_address.sin_addr), buffer, active_threads, queue_capacity);
        write(fd, log_message, strlen(log_message));
        free(log_message);
        close(fd);

        // Closing the connection
        close(client_socket);
        
        pthread_mutex_lock(&mutex);
        active_threads--;
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}




void clear_last_lines(int num_lines) {
    printf("\033[%dA\033[K", num_lines); // Déplacer le curseur vers le haut de "num_lines" lignes et effacer
}