#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "WC_LinkedList.h"

//List node
typedef struct node_t {
    
    //value that the node holds.
    void* value;
    
    //bytes of memory value takes up.
    size_t value_length;
    
    //next node in the list.
    struct node_t* next;

} node_t;

//List
typedef struct linked_list_t {
    
    //front node of the list.
    node_t* head;
    
    //number of nodes in the list.
    size_t length;

    //type of the element stored.
    element_type_t e_type;

} linked_list_t;

/*
* Private Functions
*/

//function for freeing a single node.
void node_free(node_t* node_to_free) {
    
    if (node_to_free != NULL && node_to_free->value != NULL) {
        
        free(node_to_free->value);
        free(node_to_free);
    
    } else {
        fprintf(stderr, "Error. Attempting to free NULL node.\n");
    }
}

//function for allocating memory for a new list element's value.
void* allocate_element(void* value, size_t obj_length) {
    
    //allocate memory required for new element.
    void* new_list_element = malloc(obj_length);

    //copy the passed value into it's new container.
    memcpy(new_list_element, value, obj_length);

    return new_list_element;
}

/*
* Public Functions
*/

//free a linked list from memory.
void linked_list_free(linked_list_t* list_to_free) {
    
    if (list_to_free != NULL) {
        
        node_t* list_head = list_to_free->head;
        int list_size = list_to_free->length;

        //free every node in the list.
        node_t* temp_node;
        for (int i = 0; i < list_size - 1; i++) {
            
            //save the next one in the list
            //so it can be freed next.
            temp_node = list_head->next;
            
            //free the current one.
            node_free(list_head);

            //move on to the next node.
            list_head = temp_node;
        }

        //free the list itself after all nodes
        //freed.
        free(list_to_free);

    } else {
        
        fprintf(stderr, "Error. Attempting to free null linked list.\n");
    }
}

//create a new empty linked list.
linked_list_t* linked_list_new(element_type_t type) {
    
    //allocate memory required for a new list
    linked_list_t* new_list = malloc(sizeof(linked_list_t));
    
    new_list->head = NULL;
    new_list->length = 0;
    new_list->e_type = type;
    
    return new_list;
}

//add a new element to the list.
int linked_list_add(linked_list_t* list, void* value, size_t obj_length) {

    //make sure the passed list exists.
    if (list != NULL) {
        
        //create a new node to add to the list.
        node_t* new_node = malloc(sizeof(node_t));
    
        //allocate a new container to hold the element value passed.
        new_node->value = allocate_element(value, obj_length);

        //set the length of the element stored into the node.
        new_node->value_length = obj_length;

        //this will be the last node in the list.
        new_node->next = NULL;
        
        //list is empty
        if (list->head == NULL) {
            
            //set head to the new created list node.
            list->head = new_node;
            
            //add one to the list length, as an element
            //has been added.
            list->length++;
            
            //return that the addition was successful.
            return 1;
        
        } else {
            
            //copy head to a current instance to make sure
            //the list stays intact.
            node_t* current = list->head;

            //length of the list so far.
            size_t list_length = list->length;

            //iterate to the last node in the list.
            for (int i = 0; i < list_length - 1; i++) {
                current = current->next;
            }

            //add the new node onto the end of the list.
            current->next = new_node;

            //increment the list size.
            list->length++;

            //return that the addition was successful.
            return 1;
        }
        
    
    } else {
        
        fprintf(stderr, "Error. attempting to add an element to a NULL Linked List\n");
        
        //return that the addition failed.
        return 0;
    }
}
