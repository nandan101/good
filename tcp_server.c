#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char file_content[1024];
    FILE *file;

    // 1. Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 2. Bind socket
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // 3. Listen for connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    // 4. Accept connection
    new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
    if (new_socket < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    // 5. Receive file name from client
    read(new_socket, buffer, 1024);
    printf("Client requested file: %s\n", buffer);

    // 6. Open the file
    file = fopen(buffer, "r");
    if (file == NULL) {
        // File not found, send error message
        send(new_socket, "File not found", strlen("File not found"), 0);
    } else {
        // File found, send its contents
        while (fgets(file_content, sizeof(file_content), file) != NULL) {
            send(new_socket, file_content, strlen(file_content), 0);
            memset(file_content, 0, sizeof(file_content));
        }
        fclose(file);
    }

    // 7. Close connection
    close(new_socket);
    close(server_fd);

    return 0;
}

