// TeamMembers: Manjil Rawal and Gaurav Adhikari

#include "server.h"

int serverSocketFd; // Socket for the server

/////////////////////////////////////////////
// SYNCHRONIZATION LOCKS AND COUNTER

int activeReaders = 0; // Tracks the number of active readers

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;  // Mutex lock for synchronization
pthread_mutex_t rwLock = PTHREAD_MUTEX_INITIALIZER;  // Read/Write lock

/////////////////////////////////////////////

const char *serverGreeting = "Welcome to the Chat Server!\n\nchat>";

struct node *userListHead = NULL; // Head of the user list
struct room_node *roomListHead = NULL; // Head of the room list

int main(int argc, char **argv) {

   // Set up SIGINT handler for proper shutdown
   signal(SIGINT, sigintHandler);

   //////////////////////////////////////////////////////
   // Initialize the default room where clients join after connecting
   //////////////////////////////////////////////////////

   // Create default room "Lobby"
   roomListHead = addRoom(roomListHead, DEFAULT_ROOM);
   if (roomListHead == NULL) {
       printf("Error: Failed to create default room\n");
       exit(1);
   }

   // Create the server socket
   serverSocketFd = initializeServerSocket();

   // Begin listening for incoming connections
   if (launchServer(serverSocketFd, BACKLOG) == -1) {
      printf("Error: Server launch failed\n");
      exit(1);
   }
   
   printf("Server is running and listening on PORT: %d\n", PORT);
    
   // Main loop to accept connections
   while (1) {
      // Accept a new client and create a thread for handling it
      int clientSocket = acceptClient(serverSocketFd);
      if (clientSocket != -1) {
         pthread_t clientThread;
         int *pClientSocket = malloc(sizeof(int));
         if (pClientSocket == NULL) {
             perror("Error: Memory allocation for client socket failed");
             exit(EXIT_FAILURE);
         }
         *pClientSocket = clientSocket;
         pthread_create(&clientThread, NULL, clientReceiveHandler, pClientSocket);
         pthread_detach(clientThread); // Detach thread to clean up resources after use
      }
   }

   close(serverSocketFd);
}

// Function to initialize the server socket
int initializeServerSocket() {
    int opt = 1;   
    int masterSocket;
    struct sockaddr_in serverAddress; 
    
    // Create the main socket  
    if ((masterSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {   
        perror("Error: Socket creation failed");   
        exit(EXIT_FAILURE);   
    }   
    
    // Allow the master socket to accept multiple connections
    if (setsockopt(masterSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0) {   
        perror("Error: Failed to set socket options");   
        exit(EXIT_FAILURE);   
    }   
    
    // Configure the socket address  
    serverAddress.sin_family = AF_INET;   
    serverAddress.sin_addr.s_addr = INADDR_ANY;   
    serverAddress.sin_port = htons(PORT);   
         
    // Bind the socket to the local host and port  
    if (bind(masterSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {   
        perror("Error: Socket binding failed");   
        exit(EXIT_FAILURE);   
    }   

   return masterSocket;
}

// Function to start the server
int launchServer(int serverSocket, int backlog) {
   int status = 0;
   if ((status = listen(serverSocket, backlog)) == -1) {
      perror("Error: Listening failed");
      return -1;
   }
   return status;
}

// Function to accept a new client connection
int acceptClient(int serverSocket) {
   int clientSocketFd = -1;
   socklen_t addressSize = sizeof(struct sockaddr_storage);
   struct sockaddr_storage clientAddress;

   // Accept the incoming connection from a client
   if ((clientSocketFd = accept(serverSocket, (struct sockaddr *)&clientAddress, &addressSize)) == -1) {
      perror("Error: Failed to accept client connection");
   }
   return clientSocketFd;
}

/* SIGI
