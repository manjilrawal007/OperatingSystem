// TeamMembers: Gaurav Adhikari, Manjil Rawal

#include "list.h"

// Add a user to the user list
struct node* addUser(struct node *userListHead, int connectionSocket, char *userName) {
    if (searchUser(userListHead, userName) == NULL) {
        struct node *newUser = (struct node*) malloc(sizeof(struct node));
        if (newUser == NULL) {
            perror("Error: Memory allocation for new user failed");
            exit(EXIT_FAILURE);
        }
        newUser->socket = connectionSocket;
        strcpy(newUser->username, userName);
        newUser->dm_connections = NULL;
        newUser->next = userListHead;
        userListHead = newUser;
    } else {
        printf("Duplicate username detected: %s\n", userName);
    }
    return userListHead;
}

// Search for a user by their username
struct node* searchUser(struct node *userListHead, char* userName) {
    struct node* temp = userListHead;
    while (temp != NULL) {
        if (strcmp(temp->username, userName) == 0) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

// Remove a user from the user list
struct node* removeUser(struct node *userListHead, char *userName) {
    struct node *temp = userListHead;
    struct node *prev = NULL;

    while (temp != NULL && strcmp(temp->username, userName) != 0) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) { // User not found
        return userListHead;
    }

    if (temp == userListHead) {
        userListHead = userListHead->next;
    } else {
        prev->next = temp->next;
    }

    // Release DM connections
    struct node *dmConnection = temp->dm_connections;
    while(dmConnection != NULL) {
        struct node *tempConnection = dmConnection;
        dmConnection = dmConnection->next;
        free(tempConnection);
    }

    free(temp);
    return userListHead;
}

// Display all users
void displayUsers(struct node *userListHead) {
    struct node *temp = userListHead;
    while (temp != NULL) {
        printf("User: %s\n", temp->username);
        temp = temp->next;
    }
}

// Add a room to the room list
struct room_node* addRoom(struct room_node *roomListHead, char *roomName) {
    if (searchRoom(roomListHead, roomName) == NULL) {
        struct room_node *newRoom = (struct room_node*) malloc(sizeof(struct room_node));
        if (newRoom == NULL) {
            perror("Error: Memory allocation for new room failed");
            exit(EXIT_FAILURE);
        }
        strcpy(newRoom->roomname, roomName);
        newRoom->users = NULL;
        newRoom->next = roomListHead;
        roomListHead = newRoom;
    } else {
        printf("Duplicate room name detected: %s\n", roomName);
    }
    return roomListHead;
}

// Search for a room by its name
struct room_node* searchRoom(struct room_node *roomListHead, char* roomName) {
    struct room_node* temp = roomListHead;
    while (temp != NULL) {
        if (strcmp(temp->roomname, roomName) == 0) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

// Remove a room from the room list
struct room_node* removeRoom(struct room_node *roomListHead, char *roomName) {
    struct room_node *temp = roomListHead;
    struct room_node *prev = NULL;

    while (temp != NULL && strcmp(temp->roomname, roomName) != 0) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) { // Room not found
        return roomListHead;
    }

    if (temp == roomListHead) {
        roomListHead = roomListHead->next;
    } else {
        prev->next = temp->next;
    }

    // Release users in the room
    struct node *roomUsers = temp->users;
    while(roomUsers != NULL) {
        struct node *tempUser = roomUsers;
        roomUsers = roomUsers->next;
        free(tempUser);
    }

    free(temp);
    return roomListHead;
}

// List all rooms, append to a buffer
void listAllRooms(struct room_node *roomListHead, char *buffer) {
    struct room_node *temp = roomListHead;
    while (temp != NULL) {
        strcat(buffer, temp->roomname);
        strcat(buffer, "\n");
        temp = temp->next;
    }
}

// Add a user to a specific room
void assignUserToRoom(struct room_node *room, struct node *user) {
    // Check if user is already part of the room
    struct node *temp = room->users;
    while(temp != NULL) {
        if(strcmp(temp->username, user->username) == 0) {
            // User already exists in the room
            return;
        }
        temp = temp->next;
    }

    // Add user to the room's user list
    struct node *newLink = (struct node*) malloc(sizeof(struct node));
    if (newLink == NULL) {
        perror("Error: Memory allocation for room user failed");
        exit(EXIT_FAILURE);
    }
    strcpy(newLink->username, user->username);
    newLink->socket = user->socket;
    newLink->dm_connections = NULL;
    newLink->next = room->users;
    room->users = newLink;
}

// Remove a user from a specific room
void removeUserFromRoom(struct room_node *room, char *userName) {
    struct node *temp = room->users;
    struct node *prev = NULL;

    while (temp != NULL && strcmp(temp->username, userName) != 0) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) { // User not found in the room
        return;
    }

    if (temp == room->users) {
        room->users = room->users->next;
    } else {
        prev->next = temp->next;
    }

    free(temp);
}

// List all users in a room, append to a buffer
void listRoomUsers(struct room_node *room, char *buffer) {
    struct node *temp = room->users;
    while(temp != NULL) {
        strcat(buffer, temp->username);
        strcat(buffer, "\n");
        temp = temp->next;
    }
}

// Create a DM connection between two users
bool createDMConnection(struct node *userListHead, char *user1, char *user2) {
    struct node *firstUser = searchUser(userListHead, user1);
    struct node *secondUser = searchUser(userListHead, user2);

    if(firstUser == NULL || secondUser == NULL) {
        return false;
    }

    // Verify if already connected
    struct node *temp = firstUser->dm_connections;
    while(temp != NULL) {
        if(strcmp(temp->username, user2) == 0) {
            // Already connected
            return false;
        }
        temp = temp->next;
    }

    // Link secondUser to firstUser's DM connections
    struct node *newDM1 = (struct node*) malloc(sizeof(struct node));
    if (newDM1 == NULL) {
        perror("Error: Memory allocation for DM connection failed");
        exit(EXIT_FAILURE);
    }
    strcpy(newDM1->username, secondUser->username);
    newDM1->socket = secondUser->socket;
    newDM1->dm_connections = NULL;
    newDM1->next = firstUser->dm_connections;
    firstUser->dm_connections = newDM1;

    // Link firstUser to secondUser's DM connections
    struct node *newDM2 = (struct node*) malloc(sizeof(struct node));
    if (newDM2 == NULL) {
        perror("Error: Memory allocation for DM connection failed");
        exit(EXIT_FAILURE);
    }
    strcpy(newDM2->username, firstUser->username);
    newDM2->socket = firstUser->socket;
    newDM2->dm_connections = NULL;
    newDM2->next = secondUser->dm_connections;
    secondUser->dm_connections = newDM2;

    return true;
}

// Remove a DM connection between two users
bool removeDMConnection(struct node *userListHead, char *user1, char *user2) {
    struct node *firstUser = searchUser(userListHead, user1);
    struct node *secondUser = searchUser(userListHead, user2);

    if(firstUser == NULL || secondUser == NULL) {
        return false;
    }

    // Remove secondUser from firstUser's DM connections
    struct node *temp = firstUser->dm_connections;
    struct node *prev = NULL;
    while(temp != NULL && strcmp(temp->username, user2) != 0) {
        prev = temp;
        temp = temp->next;
    }

    if(temp != NULL) {
        if(prev == NULL) {
            firstUser->dm_connections = temp->next;
        } else {
            prev->next = temp->next;
        }
        free(temp);
    }

    // Remove firstUser from secondUser's DM connections
    temp = secondUser->dm_connections;
    prev = NULL;
    while(temp != NULL && strcmp(temp->username, user1) != 0) {
        prev = temp;
        temp = temp->next;
    }

    if(temp != NULL) {
        if(prev == NULL) {
            secondUser->dm_connections = temp->next;
        } else {
            prev->next = temp->next;
        }
        free(temp);
    }

    return true;
}

// Check if two users have an active DM connection
bool checkDMConnection(struct node *userListHead, char *user1, char *user2) {
    struct node *firstUser = searchUser(userListHead, user1);
    if(firstUser == NULL) return false;

    struct node *temp = firstUser->dm_connections;
    while(temp != NULL) {
        if(strcmp(temp->username, user2) == 0) {
            return true;
        }
        temp = temp->next;
    }
    return false;
}
