#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    char buffer[1024];
    socklen_t len;
    
    // 1. Create a UDP Socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 2. Bind the socket to PORT 8080
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is ready and waiting for client messages...\n");

    len = sizeof(cliaddr);

    // 3. First receive a message from the client
    recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&cliaddr, &len);
    printf(" Client: %s", buffer);

    // 4. Now continuously send messages to the client
    while (1) {
        printf("Type a message to send to the client: ");
        fgets(buffer, 1024, stdin);

        // Send the message to client
        sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&cliaddr, len);
        printf("Message sent to client!\n");

        // If server sends 'exit', close the connection
        if (strncmp(buffer, "exit", 4) == 0) {
            printf("Server closed the connection. Exiting...\n");
            break;
        }
    }

    // Close the socket
    close(sockfd);
    return 0;
}

