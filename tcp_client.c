#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    char file_name[100];

    // 1. Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 2. Server information
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    // 3. Connect to server
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // 4. Get file name from user
    printf("Enter the file name to request: ");
    scanf("%s", file_name);

    // 5. Send file name to server
    send(sock, file_name, strlen(file_name), 0);

    // 6. Receive file content or error message
    printf("File content received from server:\n");
    while (read(sock, buffer, 1024) > 0) {
        printf("%s", buffer);
        memset(buffer, 0, sizeof(buffer));
    }

    // 7. Close socket
    close(sock);

    return 0;
}

