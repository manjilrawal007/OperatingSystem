// TeamMembers: Gaurav Adhikari and Manjil Rawal

#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Structure for user linked list node
struct userNode {
    char userName[30];
    int connectionSocket;
    struct userNode *next;
    struct userNode *dmList; // List of DM connections
};

// Structure for room linked list node
struct roomNode {
    char roomName[30];
    struct roomNode *next;
    struct userNode *userList; // List of users in the room
};

// Functions to manage users
struct userNode* addUser(struct userNode *head, int connectionSocket, char *userName);
struct userNode* searchUser(struct userNode *head, char* userName);
struct userNode* removeUser(struct userNode *head, char *userName);
void displayUsers(struct userNode *head);

// Functions to manage rooms
struct roomNode* addRoom(struct roomNode *head, char *roomName);
struct roomNode* searchRoom(struct roomNode *head, char* roomName);
struct roomNode* removeRoom(struct roomNode *head, char *roomName);
void listAllRooms(struct roomNode *head, char *buffer);
void assignUserToRoom(struct roomNode *room, struct userNode *user);
void removeUserFromRoom(struct roomNode *room, char *userName);
void listRoomUsers(struct roomNode *room, char *buffer);

// Functions to manage direct messages (DM)
bool createDMConnection(struct userNode *head, char *user1, char *user2);
bool removeDMConnection(struct userNode *head, char *user1, char *user2);
bool checkDMConnection(struct userNode *head, char *user1, char *user2);

#endif // LIST_H

