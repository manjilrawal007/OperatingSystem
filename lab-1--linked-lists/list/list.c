// linkedlist/linkedlist_impl.c
//
// Implementation for linked list.
//
// Author: Manjil Rawal

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "linkedlist.h"

list_t *create_list() {
    list_t *new_list = (list_t *)malloc(sizeof(list_t));
    new_list->head = NULL;
    return new_list;
}

void free_list(list_t *list) {
    node_t *current_node = list->head;
    node_t *next_node;
    while (current_node != NULL) {
        next_node = current_node->next;
        free(current_node);
        current_node = next_node;
    }
    free(list);
}

node_t *create_node(elem value) {
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    new_node->value = value;
    new_node->next = NULL;
    return new_node;
}

void print_list(list_t *list) {
    node_t *current_node = list->head;
    while (current_node != NULL) {
        printf("%d ", current_node->value);
        current_node = current_node->next;
    }
    printf("\n");
}

char *convert_list_to_string(list_t *list) {
    char *buffer = (char *)malloc(sizeof(char) * 1024);
    char temp_buffer[20];

    node_t *current_node = list->head;
    while (current_node != NULL) {
        sprintf(temp_buffer, "%d->", current_node->value);
        strcat(buffer, temp_buffer);
        current_node = current_node->next;
    }
    strcat(buffer, "NULL");

    return buffer;
}

int get_list_length(list_t *list) {
    int count = 0;
    node_t *current_node = list->head;
    while (current_node != NULL) {
        count++;
        current_node = current_node->next;
    }
    return count;
}

void add_to_list_end(list_t *list, elem value) {
    node_t *new_node = create_node(value);
    if (list->head == NULL) {
        list->head = new_node;
    } else {
        node_t *current_node = list->head;
        while (current_node->next != NULL) {
            current_node = current_node->next;
        }
        current_node->next = new_node;
    }
}

void add_to_list_start(list_t *list, elem value) {
    node_t *new_node = create_node(value);
    new_node->next = list->head;
    list->head = new_node;
}

void add_to_list_at_index(list_t *list, elem value, int index) {
    if (index < 1 || index > get_list_length(list)) {
        return;
    }

    if (index == 1) {
        add_to_list_start(list, value);
        return;
    }

    node_t *new_node = create_node(value);
    node_t *current_node = list->head;
    int position = 1;

    while (position < index - 1) {
        current_node = current_node->next;
        position++;
    }

    new_node->next = current_node->next;
    current_node->next = new_node;
}

elem remove_from_list_end(list_t *list) {
    if (list == NULL || list->head == NULL) {
        return -1;
    }

    node_t *current_node = list->head;

    if (current_node->next == NULL) {
        elem value = current_node->value;
        free(current_node);
        list->head = NULL;
        return value;
    }

    while (current_node->next->next != NULL) {
        current_node = current_node->next;
    }

    elem value = current_node->next->value;
    free(current_node->next);
    current_node->next = NULL;
    return value;
}

elem remove_from_list_start(list_t *list) {
    if (list == NULL || list->head == NULL) {
        return -1;
    }

    node_t *current_node = list->head;
    elem value = current_node->value;
    list->head = current_node->next;
    free(current_node);

    return value;
}

elem remove_from_list_at_index(list_t *list, int index) {
    if (list->head == NULL) {
        return -1;
    }

    if (index == 1) {
        return remove_from_list_start(list);
    }

    node_t *current_node = list->head;
    int position = 1;

    while (current_node != NULL && position != index - 1) {
        current_node = current_node->next;
        position++;
    }

    if (current_node == NULL || current_node->next == NULL) {
        return -1;
    }

    node_t *node_to_remove = current_node->next;
    elem value = node_to_remove->value;
    current_node->next = node_to_remove->next;
    free(node_to_remove);

    return value;
}

bool is_value_in_list(list_t *list, elem value) {
    node_t *current_node = list->head;
    while (current_node != NULL) {
        if (current_node->value == value) {
            return true;
        }
        current_node = current_node->next;
    }
    return false;
}

elem get_value_at_index(list_t *list, int index) {
    node_t *current_node = list->head;
    int position = 1;

    while (current_node != NULL && position != index) {
        current_node = current_node->next;
        position++;
    }

    if (current_node == NULL) {
        return -1;
    }

    return current_node->value;
}

int get_index_of_value(list_t *list, elem value) {
    node_t *current_node = list->head;
    int position = 1;

    while (current_node != NULL) {
        if (current_node->value == value) {
            return position;
        }
        current_node = current_node->next;
        position++;
    }
    return -1;
}


