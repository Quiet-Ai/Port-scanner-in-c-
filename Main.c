#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

// Function to handle errors and exit
void handle_error(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main() {
    int sockfd, sport, eport, result;
    char ip[16];

    printf("Target IP: ");
    if (scanf("%15s", ip) != 1) {
        fprintf(stderr, "Invalid IP address input.\n");
        return EXIT_FAILURE;
    }

    printf("Starting port: ");
    if (scanf("%d", &sport) != 1 || sport < 0 || sport > 65535) {
        fprintf(stderr, "Invalid starting port number.\n");
        return EXIT_FAILURE;
    }

    printf("Ending port: ");
    if (scanf("%d", &eport) != 1 || eport < 0 || eport > 65535 || eport < sport) {
        fprintf(stderr, "Invalid ending port number.\n");
        return EXIT_FAILURE;
    }

    struct sockaddr_in sr_addr;
    memset(&sr_addr, 0, sizeof(sr_addr));
    sr_addr.sin_family = AF_INET;
    sr_addr.sin_addr.s_addr = inet_addr(ip);

    for (int i = sport; i <= eport; ++i) {
        sr_addr.sin_port = htons(i);
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            handle_error("socket");
        }

        result = connect(sockfd, (struct sockaddr *) &sr_addr, sizeof(sr_addr));
        if (result == 0) {
            printf("Open port: %d\n", i);
        } else if (result != -1) {
            handle_error("connect");
        }

        close(sockfd);
    }

    return EXIT_SUCCESS;
}
