#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define CONNECTION_PORT 12345
#define MAX_MSG_LEN 1024

int main(void) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[MAX_MSG_LEN] = {0};

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(CONNECTION_PORT);

    // Bind socket to localhost
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listening for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Waiting for incoming connection...\n");

    // Accept incoming connections
    new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (new_socket < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // Read messages from client
    while (1) {
        int byte_received = recv(new_socket, buffer, MAX_MSG_LEN, 0);
        if (byte_received == -1) {
            perror("recv");
            break;
        } else if (byte_received == 0) {
            printf("Connection closed by client.\n");
            break;
        }
        printf("Client: %s\n", buffer);
        memset(buffer, 0, MAX_MSG_LEN);
    }

    close(new_socket);
    close(server_fd);
    return 0;
}
