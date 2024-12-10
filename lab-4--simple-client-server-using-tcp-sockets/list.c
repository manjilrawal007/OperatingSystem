#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

// Allocates memory for a new list and initializes it
list_t* allocate_list() {
    list_t* new_list = (list_t*)malloc(sizeof(list_t));
    new_list->head = NULL;
    return new_list;
}

// Frees all memory associated with a list
void free_list(list_t* list) {
    node_t* current_node = list->head;
    node_t* next_node;
    while (current_node != NULL) {
        next_node = current_node->next;
        free(current_node);
        current_node = next_node;
    }
    free(list);
}

// Adds a new element to the front of the list
void add_to_front(list_t* list, int value) {
    node_t* new_node = (node_t*)malloc(sizeof(node_t));
    new_node->data = value;
    new_node->next = list->head;
    list->head = new_node;
}

// Adds a new element to the back of the list
void add_to_back(list_t* list, int value) {
    node_t* new_node = (node_t*)malloc(sizeof(node_t));
    new_node->data = value;
    new_node->next = NULL;
    if (list->head == NULL) {
        list->head = new_node;
    } else {
        node_t* current_node = list->head;
        while (current_node->next != NULL) {
            current_node = current_node->next;
        }
        current_node->next = new_node;
    }
}

// Adds a new element at a specified index in the list
void add_at_index(list_t* list, int index, int value) {
    if (index == 0) {
        add_to_front(list, value);
        return;
    }
    node_t* new_node = (node_t*)malloc(sizeof(node_t));
    new_node->data = value;
    node_t* current_node = list->head;
    for (int i = 0; i < index - 1 && current_node != NULL; i++) {
        current_node = current_node->next;
    }
    if (current_node != NULL) {
        new_node->next = current_node->next;
        current_node->next = new_node;
    } else {
        free(new_node);  // Index out of bounds
    }
}

// Removes and returns the first element in the list
int remove_from_front(list_t* list) {
    if (list->head == NULL) return -1;
    node_t* temp_node = list->head;
    int value = temp_node->data;
    list->head = list->head->next;
    free(temp_node);
    return value;
}

// Removes and returns the last element in the list
int remove_from_back(list_t* list) {
    if (list->head == NULL) return -1;
    node_t* current_node = list->head;
    if (current_node->next == NULL) {
        int value = current_node->data;
        free(current_node);
        list->head = NULL;
        return value;
    }
    while (current_node->next->next != NULL) {
        current_node = current_node->next;
    }
    int value = current_node->next->data;
    free(current_node->next);
    current_node->next = NULL;
    return value;
}

// Removes and returns an element at a specific index in the list
int remove_at_index(list_t* list, int index) {
    if (index == 0) {
        return remove_from_front(list);
    }
    node_t* current_node = list->head;
    for (int i = 0; i < index - 1 && current_node != NULL; i++) {
        current_node = current_node->next;
    }
    if (current_node != NULL && current_node->next != NULL) {
        node_t* to_remove = current_node->next;
        int value = to_remove->data;
        current_node->next = to_remove->next;
        free(to_remove);
        return value;
    }
    return -1;  // Index out of bounds
}

// Retrieves the element at a specific index in the list
int get_element_at(list_t* list, int index) {
    node_t* current_node = list->head;
    for (int i = 0; i < index && current_node != NULL; i++) {
        current_node = current_node->next;
    }
    if (current_node != NULL) {
        return current_node->data;
    }
    return -1;  // Index out of bounds
}

// Returns the total number of elements in the list
int get_list_length(list_t* list) {
    int length = 0;
    node_t* current_node = list->head;
    while (current_node != NULL) {
        length++;
        current_node = current_node->next;
    }
    return length;
}

// Converts the list to a string representation
char* list_to_string(list_t* list) {
    static char result[1024];
    memset(result, 0, sizeof(result));
    node_t* current_node = list->head;
    while (current_node != NULL) {
        char buffer[32];
        sprintf(buffer, "%d -> ", current_node->data);
        strcat(result, buffer);
        current_node = current_node->next;
    }
    strcat(result, "NULL");
    return result;
}
