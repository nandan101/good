#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[1024];
    socklen_t len;

    // 1. Create a UDP Socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 2. Set server details
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    // 3. Send a hello message to the server
    strcpy(buffer, "Hello Server!\n");
    sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&servaddr, sizeof(servaddr));
    printf("✅ Initial message sent to server!\n");

    // 4. Now continuously receive messages from server
    len = sizeof(servaddr);
    while (1) {
        recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&servaddr, &len);
        printf("📩 Server: %s", buffer);

        // Exit condition
        if (strncmp(buffer, "exit", 4) == 0) {
            printf("🚀 Server has closed the connection. Exiting...\n");
            break;
        }

        // Clear buffer
        memset(buffer, 0, sizeof(buffer));
    }

    // Close the socket
    close(sockfd);
    return 0;
}

