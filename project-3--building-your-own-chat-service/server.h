// TeamMembers: Manjil Rawal and Gaurav Adhikari

#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include "list.h"

#define SERVER_PORT 8888
#define CONNECTION_BACKLOG 10
#define BUFFER_SIZE 2096

#define INITIAL_ROOM "Lobby"

// Function prototypes for server operations
int initializeServerSocket();
int launchServer(int serverSocket, int backlog);
int handleClientConnection(int serverSocket);
void *processClientMessages(void *clientSocketPtr);
void handleSignalInterrupt(int signalNumber);

// Global variables
extern int serverSocketFd;          // Main server socket
extern struct node *userListHead;   // Head of the user list
extern struct room_node *roomListHead; // Head of the room list

// Synchronization primitives for thread safety
extern int activeReaderCount;
extern pthread_mutex_t lock;
extern pthread_mutex_t rwLock;

// Server Message of the Day
extern const char *welcomeMessage;

#endif // SERVER_H
