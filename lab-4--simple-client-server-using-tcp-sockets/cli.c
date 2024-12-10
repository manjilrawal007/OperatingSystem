#include <netinet/in.h>  // For sockaddr_in structure
#include <stdio.h>       // For input/output functions
#include <stdlib.h>      // For exit(), atoi(), etc.
#include <string.h>      // For string manipulation functions
#include <sys/socket.h>  // For socket-related functions
#include <sys/types.h>   // For socket types
#include <unistd.h>      // For close() function

#define SERVER_PORT 9001
#define MAX_INPUT_LENGTH 1024

// Function to retrieve a command line from the user
char* fetchCommand(char *inputBuffer) {
    do {
        if ((fgets(inputBuffer, MAX_INPUT_LENGTH, stdin) == NULL) && ferror(stdin)) {
            fprintf(stderr, "Error reading input with fgets");
            exit(1);
        }
    } while (inputBuffer[0] == '\n');  // Skip empty input lines

    inputBuffer[strlen(inputBuffer) - 1] = '\0';  // Remove the trailing newline character
    return inputBuffer;
}

int main(int argc, char const* argv[]) {
    int socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketDescriptor < 0) {
        perror("Failed to create socket");
        exit(1);
    }

    char inputBuffer[MAX_INPUT_LENGTH];
    char serverResponse[MAX_INPUT_LENGTH];
    struct sockaddr_in serverAddress;

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(SERVER_PORT);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // Attempt to connect to the server
    if (connect(socketDescriptor, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Failed to connect to the server");
        exit(1);
    }
    printf("Successfully connected to the server!\n");

    while (1) {
        printf("Enter Command (or menu): ");
        fetchCommand(inputBuffer);

        send(socketDescriptor, inputBuffer, strlen(inputBuffer), 0);

        // Handle the "exit" command
        if (strcmp(inputBuffer, "exit") == 0) {
            printf("Closing client connection...\n");
            close(socketDescriptor);  // Properly close the socket
            exit(0);
        }
        // Display menu for the "menu" command
        else if (strcmp(inputBuffer, "menu") == 0) {
            printf("AVAILABLE COMMANDS:\n-------------------\n1. print\n2. get_length\n3. add_back <value>\n4. add_front <value>\n5. add_position <index> <value>\n6. remove_back\n7. remove_front\n8. remove_position <index>\n9. get <index>\n10. exit\n");
        }

        // Receive and display the server's response
        recv(socketDescriptor, serverResponse, sizeof(serverResponse), 0);
        printf("\nSERVER RESPONSE: %s\n", serverResponse);

        // Clear the input buffer for the next command
        memset(inputBuffer, 0, MAX_INPUT_LENGTH);
    }

    return 0;
}
