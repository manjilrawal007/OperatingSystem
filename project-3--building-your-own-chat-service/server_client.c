// Developers: Gaurav Adhikari and Manjil Rawal

#include "server.h"

#define PRIMARY_ROOM "Lobby"

// LOCKS AND COUNTER FOR SYNCHRONIZATION
extern int activeReaderCount;
extern pthread_mutex_t readWriteLock;
extern pthread_mutex_t readerCountMutex;

extern struct node *userList;       // User list
extern struct room_node *roomList; // Room list

extern const char *welcomeMessage;

// Delimiters for parsing commands
#define separators " \n"

// Helper functions for synchronization
void acquireReaderLock() {
    pthread_mutex_lock(&readerCountMutex);
    activeReaderCount++;
    if (activeReaderCount == 1) {
        pthread_mutex_lock(&readWriteLock);
    }
    pthread_mutex_unlock(&readerCountMutex);
}

void releaseReaderLock() {
    pthread_mutex_lock(&readerCountMutex);
    activeReaderCount--;
    if (activeReaderCount == 0) {
        pthread_mutex_unlock(&readWriteLock);
    }
    pthread_mutex_unlock(&readerCountMutex);
}

void acquireWriterLock() {
    pthread_mutex_lock(&readWriteLock);
}

void releaseWriterLock() {
    pthread_mutex_unlock(&readWriteLock);
}

// Remove leading and trailing whitespace from a string
char *stripWhitespace(char *input) {
    char *end;

    while (isspace((unsigned char)*input)) input++;

    if (*input == 0) return input;

    end = input + strlen(input) - 1;
    while (end > input && isspace((unsigned char)*end)) end--;

    end[1] = '\0';
    return input;
}

// Thread function for handling client communication
void *handleClientCommunication(void *arg) {
    int clientSocket = *(int *)arg;
    free(arg);

    int bytesRead, argCount;
    char inputBuffer[MAXBUFF], outputBuffer[MAXBUFF];
    char tempBuffer[MAXBUFF];
    char command[MAXBUFF], username[30];
    char *parsedArguments[80];
    char *token;

    struct node *currentClient;
    struct room_node *currentRoom;

    // Send welcome message
    send(clientSocket, welcomeMessage, strlen(welcomeMessage), 0);

    // Assign guest username
    sprintf(username, "guest%d", clientSocket);

    // Add guest to user list and default room
    acquireWriterLock();
    userList = insertFirstU(userList, clientSocket, username);
    currentRoom = findR(roomList, PRIMARY_ROOM);
    if (currentRoom != NULL) {
        currentClient = findU(userList, username);
        if (currentClient != NULL) {
            addUserToRoom(currentRoom, currentClient);
        }
    }
    releaseWriterLock();

    while (1) {
        if ((bytesRead = read(clientSocket, inputBuffer, MAXBUFF - 1)) <= 0) {
            printf("Client disconnected: %s\n", username);
            break;
        }
        inputBuffer[bytesRead] = '\0';
        strcpy(command, inputBuffer);
        strcpy(outputBuffer, inputBuffer);

        argCount = 0;
        token = strtok(command, separators);
        while (token != NULL && argCount < 80) {
            parsedArguments[argCount++] = token;
            token = strtok(NULL, separators);
        }
        parsedArguments[argCount] = NULL;

        for (int i = 0; i < argCount; i++) {
            parsedArguments[i] = stripWhitespace(parsedArguments[i]);
        }

        if (parsedArguments[0] == NULL) {
            sprintf(inputBuffer, "\nchat>");
            send(clientSocket, inputBuffer, strlen(inputBuffer), 0);
            continue;
        }

        if (strcmp(parsedArguments[0], "create") == 0) {
            if (argCount < 2) {
                strcpy(inputBuffer, "Usage: create <room>\nchat>");
                send(clientSocket, inputBuffer, strlen(inputBuffer), 0);
                continue;
            }

            printf("Create room: %s\n", parsedArguments[1]);
            acquireWriterLock();
            roomList = insertFirstR(roomList, parsedArguments[1]);
            releaseWriterLock();

            sprintf(inputBuffer, "Room '%s' created.\nchat>", parsedArguments[1]);
            send(clientSocket, inputBuffer, strlen(inputBuffer), 0);
        } else if (strcmp(parsedArguments[0], "join") == 0) {
            // Implement 'join' functionality
        } else if (strcmp(parsedArguments[0], "leave") == 0) {
            // Implement 'leave' functionality
        } else if (strcmp(parsedArguments[0], "connect") == 0) {
            // Implement 'connect' functionality
        } else if (strcmp(parsedArguments[0], "disconnect") == 0) {
            // Implement 'disconnect' functionality
        } else if (strcmp(parsedArguments[0], "rooms") == 0) {
            // Implement 'rooms' functionality
        } else if (strcmp(parsedArguments[0], "users") == 0) {
            // Implement 'users' functionality
        } else if (strcmp(parsedArguments[0], "login") == 0) {
            // Implement 'login' functionality
        } else if (strcmp(parsedArguments[0], "help") == 0) {
            // Implement 'help' functionality
        } else if (strcmp(parsedArguments[0], "exit") == 0 || strcmp(parsedArguments[0], "logout") == 0) {
            // Implement 'exit/logout' functionality
        } else {
            // Implement message broadcasting functionality
        }

        memset(inputBuffer, 0, sizeof(inputBuffer));
    }

    acquireWriterLock();
    // Remove user cleanup
    releaseWriterLock();
    close(clientSocket);
    printf("User '%s' has disconnected.\n", username);
    return NULL;
}
