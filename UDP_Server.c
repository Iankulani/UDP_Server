#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

// Function to handle incoming data from client
void handle_client(int server_socket, struct sockaddr_in *client_addr) {
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(*client_addr);

    // Receive data from client
    int recv_len = recvfrom(server_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)client_addr, &addr_len);
    if (recv_len == -1) {
        perror("recvfrom failed");
        exit(1);
    }

    // Null-terminate the received data
    buffer[recv_len] = '\0';
    printf("Received from client: %s\n", buffer);

    // Send response to client
    const char *response = "Message received";
    if (sendto(server_socket, response, strlen(response), 0, (struct sockaddr *)client_addr, addr_len) == -1) {
        perror("sendto failed");
        exit(1);
    }
}

int main() {
    int server_socket;
    struct sockaddr_in server_addr, client_addr;

    // Create a UDP socket
    if ((server_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Prepare server address structure
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    printf("Enter the port number to listen on: ");
    scanf("%d", &server_addr.sin_port);

    // Bind the socket to the address
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        close(server_socket);
        exit(1);
    }

    printf("Server listening on port %d...\n", ntohs(server_addr.sin_port));

    while (1) {
        // Handle client messages
        handle_client(server_socket, &client_addr);
    }

    close(server_socket);
    return 0;
}
