#ifndef LINKED_LIST_H
#define LINKED_LIST_H

// Node structure representing an element in the list
typedef struct Node {
    int value;
    struct Node* next;
} Node;

// List structure containing the head of the linked list
typedef struct {
    Node* head;
} LinkedList;

// Function to allocate and initialize a new list
LinkedList* create_list();

// Function to free all memory associated with a list
void destroy_list(LinkedList* list);

// Function to add an element to the front of the list
void add_element_to_front(LinkedList* list, int value);

// Function to add an element to the back of the list
void add_element_to_back(LinkedList* list, int value);

// Function to add an element at a specific index in the list
void add_element_at_index(LinkedList* list, int index, int value);

// Function to remove and return the first element of the list
int remove_element_from_front(LinkedList* list);

// Function to remove and return the last element of the list
int remove_element_from_back(LinkedList* list);

// Function to remove and return an element at a specific index in the list
int remove_element_at_index(LinkedList* list, int index);

// Function to retrieve the element at a specific index in the list
int get_element_at_index(LinkedList* list, int index);

// Function to calculate the total number of elements in the list
int get_list_size(LinkedList* list);

// Function to convert the list to a string representation
char* list_to_string(LinkedList* list);

#endif
