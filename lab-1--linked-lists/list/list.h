// linkedlist/linkedlist_interface.h
//
// Interface definition for linked list.
//
// Author: Manjil Rawal

#include <stdbool.h>

/* Defines the type of the elements in the linked list. Modify this if needed. */
typedef int element_type;

/* Defines the node structure. Each node contains its element and a pointer to the
 * next node in the list. The last element in the list has a NULL next pointer. */
typedef struct node {
    element_type value;
    struct node *next;
} node_t;

/* Defines the list structure, which points to the first node in the
 * list. */
typedef struct list {
    node_t *head;
} list_t;

/* Functions for creating and deleting lists. Use these functions to handle
 * memory allocation and deallocation for the linked list library. */
list_t *create_list();
void free_list(list_t *list);

/* Prints the contents of the list. */
void print_list(list_t *list);

/* Converts the list to a string representation. */
char *convert_list_to_string(list_t *list);

/* Creates a node on the heap and returns it. */
node_t *create_node(element_type value);

/* Returns the length of the list. */
int get_list_length(list_t *list);

/* Functions for adding elements to the list. */
void add_to_list_end(list_t *list, element_type value);
void add_to_list_start(list_t *list, element_type value);
void add_to_list_at_index(list_t *list, element_type value, int index);

/* Functions for removing elements from the list. Returns the removed element. */
element_type remove_from_list_end(list_t *list);
element_type remove_from_list_start(list_t *list);
element_type remove_from_list_at_index(list_t *list, int index);

/* Checks whether a given element exists in the list. */
bool is_value_in_list(list_t *list, element_type value);

/* Returns the element at the specified index. The index starts at 1. Returns -1 on error. */
element_type get_value_at_index(list_t *list, int index);

/* Returns the index at which the specified element is found. Returns -1 if the element is not found. */
int get_index_of_value(list_t *list, element_type value);
