#include <stdio.h>
#include <stdlib.h>

//List types
typedef enum element_type_t {
    WC_LINKEDLIST_INT, WC_LINKEDLIST_DOUBLE, WC_LINKEDLIST_STRING, WC_LINKEDLIST_OBJ
} element_type_t;

//List node
typedef struct node_t {
    //value that the node holds.
    void* value;
    
    //bytes of memory value takes up.
    unsigned int value_length;
    
    //type of the element stored.
    element_type_t e_type;
    
    //next node in the list.
    struct node_t* next;
} node_t;

//List
typedef struct linked_list_t {
    //front node of the list.
    node_t* head;
    
    //number of nodes in the list.
    unsigned int length;
} linked_list_t;

//create a new empty linked list
linked_list_t* linked_list_new() {
    //allocate memory required for a new list
    linked_list_t* new_list = malloc(sizeof(linked_list_t));
    new_list->head = NULL;
    new_list->length = 0;
    return new_list;
}

//free a linked list from memory
void linked_list_free() {

}