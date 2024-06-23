#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define CONNECTION_PORT 12345
#define SERVER_IP "127.0.0.1"  // Replace with actual server IP address

int main(void)
{
    int sock = 0;
    struct sockaddr_in server_addr;
    char msg[1024];

    // Create a socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(CONNECTION_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Connect to server
    int connection_result = connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (connection_result < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connection to server succeeded\n");


    while (1) {
        printf("Enter message here: ");
        fgets(msg, sizeof(msg), stdin);

        // Send message to server
        int send_result = send(sock, msg, strlen(msg), 0);
        if (send_result == -1) {
            perror("Send failed");
            break;
        }
    }

    close(sock);
    return 0;
}
