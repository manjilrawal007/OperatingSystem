#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>  // For close()

#include "list.h"  // Assuming this contains the linked list functions

#define SERVER_PORT 9001
#define RESPONSE_ACK "ACKNOWLEDGED"

int main(int argc, char const* argv[]) {
    int bytes_received, value, position;  // Declare variables for value and position
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Failed to create socket");
        exit(1);
    }

    char input_buffer[1024];
    char response_buffer[1024];
    char* command;

    struct sockaddr_in server_address;
    LinkedList* linked_list = create_list();  // Allocate the linked list

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("Failed to bind socket");
        exit(1);
    }

    if (listen(server_socket, 1) < 0) {
        perror("Failed to listen on socket");
        exit(1);
    }

    printf("Waiting for a client to connect...\n");
    int client_socket = accept(server_socket, NULL, NULL);
    if (client_socket < 0) {
        perror("Failed to accept client connection");
        exit(1);
    }
    printf("Client connected!\n");

    while (1) {
        memset(input_buffer, 0, sizeof(input_buffer));  // Clear input buffer at the start of each loop

        bytes_received = recv(client_socket, input_buffer, sizeof(input_buffer), 0);
        input_buffer[bytes_received] = '\0';

        if (bytes_received > 0) {
            command = strtok(input_buffer, " ");
            memset(response_buffer, 0, sizeof(response_buffer));  // Clear response buffer before each response

            if (strcmp(command, "exit") == 0) {
                destroy_list(linked_list);
                printf("Server shutting down...\n");
                close(client_socket);
                close(server_socket);
                exit(0);
            } 
            else if (strcmp(command, "print") == 0) {
                snprintf(response_buffer, sizeof(response_buffer), "%s", list_to_string(linked_list));
            } 
            else if (strcmp(command, "get_length") == 0) {
                value = get_list_size(linked_list);
                snprintf(response_buffer, sizeof(response_buffer), "Length = %d", value);
            } 
            else if (strcmp(command, "add_back") == 0) {
                command = strtok(NULL, " ");
                value = atoi(command);
                add_element_to_back(linked_list, value);
                snprintf(response_buffer, sizeof(response_buffer), "%s %d", RESPONSE_ACK, value);
            } 
            else if (strcmp(command, "add_front") == 0) {
                command = strtok(NULL, " ");
                value = atoi(command);
                add_element_to_front(linked_list, value);
                snprintf(response_buffer, sizeof(response_buffer), "%s %d", RESPONSE_ACK, value);
            } 
            else if (strcmp(command, "add_position") == 0) {
                command = strtok(NULL, " ");
                position = atoi(command);
                command = strtok(NULL, " ");
                value = atoi(command);
                add_element_at_index(linked_list, position, value);
                snprintf(response_buffer, sizeof(response_buffer), "%s %d at %d", RESPONSE_ACK, value, position);
            } 
            else if (strcmp(command, "remove_back") == 0) {
                value = remove_element_from_back(linked_list);
                snprintf(response_buffer, sizeof(response_buffer), "Removed = %d", value);
            } 
            else if (strcmp(command, "remove_front") == 0) {
                value = remove_element_from_front(linked_list);
                snprintf(response_buffer, sizeof(response_buffer), "Removed = %d", value);
            } 
            else if (strcmp(command, "remove_position") == 0) {
                command = strtok(NULL, " ");
                position = atoi(command);
                value = remove_element_at_index(linked_list, position);
                snprintf(response_buffer, sizeof(response_buffer), "Removed = %d from %d", value, position);
            } 
            else if (strcmp(command, "get") == 0) {
                command = strtok(NULL, " ");
                position = atoi(command);
                value = get_element_at_index(linked_list, position);
                snprintf(response_buffer, sizeof(response_buffer), "Element at %d = %d", position, value);
            } 
            else {
                snprintf(response_buffer, sizeof(response_buffer), "Unknown command");
            }

            send(client_socket, response_buffer, strlen(response_buffer) + 1, 0);  // Send with null terminator
        }
    }

    return 0;
}

