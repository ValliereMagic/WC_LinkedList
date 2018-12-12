#include <stdio.h>
#include <stdlib.h>
#include <math.h>
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

    //end of the list
    node_t* tail;
    
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
    
    if (node_to_free == NULL || node_to_free->value == NULL) { 

        fprintf(stderr, "Error. Attempting to free NULL node.\n");

        return;
    }
        
    free(node_to_free->value);
    
    free(node_to_free);
}

//function for allocating memory for a new list element's value.
void* allocate_element(void* value, size_t obj_length) {

    //allocate memory required for new element.
    void* new_list_element = malloc(obj_length);

    //copy the passed value into it's new container.
    memcpy(new_list_element, value, obj_length);

    return new_list_element;
}

//one if the elements are equal,
//0 otherwise.
unsigned char is_element_equal(char* value_one, size_t value_one_length, char* value_two, size_t value_two_length) {
    
    //make sure each element has the same length.
    if (value_one_length != value_two_length) {
        
        //elements are not the same length,
        //therefore they cannot be equal.
        return 0;
    }

    //check each byte of value_one and value_two
    //to see if they are equal.
    for (size_t i = 0; i < value_one_length; i++) {
        
        //if one byte isn't equal, the elements aren't equal.
        if (value_one[i] != value_two[i]) {
            
            return 0;
        }
    }

    //elements are equal.
    return 1;
}

//one if the elements are equal,
//0 otherwise.
unsigned char is_floating_point_element_equal(double* value_one, double* value_two) {

    //allowed inaccuracy
    double epsilon = 0.00001;

    return (fabs(*value_one - *value_two) < epsilon);
}

/*
* Public Functions
*/

//free a linked list from memory.
void linked_list_free(linked_list_t* list_to_free) {
    
    if (list_to_free == NULL) { 
        
        fprintf(stderr, "Error. Attempting to free null linked list.\n");
        
        return;
    }
        
    node_t* list_head = list_to_free->head;
    
    int list_size = list_to_free->length;

    //free every node in the list.
    node_t* temp_node;
    
    for (int i = 0; i < list_size; i++) {
        
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
}

//create a new empty linked list.
linked_list_t* linked_list_new(element_type_t type) {
    
    //allocate memory required for a new list
    linked_list_t* new_list = malloc(sizeof(linked_list_t));
    
    //set list properties to default values

    new_list->head = NULL;

    new_list->tail = NULL;
    
    new_list->length = 0;
    
    new_list->e_type = type;
    
    return new_list;
}

//add a new element to the list.
unsigned char linked_list_add(linked_list_t* list, void* value, size_t obj_length) {

    //make sure the passed list exists.
    if (list == NULL) { 
        
        fprintf(stderr, "Error. attempting to add an element to a NULL Linked List\n");
        
        //return that the addition failed.
        return 0;
    }

    //make sure a null value is not being added to the list.
    if (value == NULL) {

        fprintf(stderr, "Error. attempting to add a NULL element to a list.\n");

        //return that the addition failed.
        return 0;
    }

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

        //set the tail to the new head
        list->tail = new_node;
        
    //list has at least one element
    } else {

        //make a copy of the list tail
        //to make sure not null
        node_t* list_tail = list->tail;

        //make sure the tail exists before
        //attempting to dereference tail->next
        if (list_tail == NULL) {
            
            fprintf(stderr, "Error. the tail of the list is NULL.\n");
        }
        
        //set the new tail of the list
        list_tail->next = new_node;

        //update the tail node
        list->tail = list_tail->next;
    }

    //add one to the list length, as an element
    //has been added.
    list->length++;
    
    //return that the addition was successful.
    return 1;    
}

//remove an element from the list at an index
unsigned char linked_list_remove_at(linked_list_t* list, unsigned int index) {

    //make sure list exists.
    if (list == NULL) { 
        
        fprintf(stderr, "Error. Attempting to remove a value from a NULL list.\n");

        return 0;
    }

    //calculate the maximum value that the index can be.
    unsigned int max_index_value = list->length - 1;

    //make sure that the index is within the size of the list.
    if (index > max_index_value) { 
            
        fprintf(stderr, "Error. Attempting to remove at an index out of bounds.\n");

        return 0;
    }

    //copy list head for iteration.
    node_t* list_head = list->head;

    //make sure that the list isn't empty.
    if (list_head == NULL) { 
                
        fprintf(stderr, "Error. Attempting to remove values from an empty list.");

        return 0;
    }
                
    //handle removing from list with 1 element.
    //It is implied that index is also 0, because
    //index cannot be less than 0; and index is 
    //less than or equal to max_index_value.
    if (max_index_value == 0) {
        
        //free only element in the list.
        node_free(list_head);
        
        list->head = NULL;
    
    //Removing the first element in the list
    //when the list has more than one element.
    } else if (index == 0) {
        
        //replace the head with the next element in the list.
        list->head = list_head->next;

        //free the head element.
        free(list_head);

    } else {

        //index must be > 0 and <= max_index_value
        //for code below this comment to execute.

        //iterate to the element before the one to be removed.
        for (unsigned int i = 0; i < index - 1; i++) {

            //iterate to the next node in the list.
            list_head = list_head->next;

        }

        //now at the element infront of the one to remove.
        node_t* node_to_free = list_head->next;

        //jump over the element to remove (removing it from the list).
        list_head->next = node_to_free->next;

        //free node that was removed.
        node_free(node_to_free);
    }

    //decrement list length
    list->length--;

    //return that the removal was successful.
    return 1;
}

//remove an element from the list using a value
int linked_list_remove_value(linked_list_t* list, void* value, size_t obj_length) {

    //make sure list exists.
    if (list == NULL) { 
        
        fprintf(stderr, "Error. Attempting to remove a value from a NULL list.\n");

        return 0;
    }

    //save head to make sure list itself doesn't lose it's head.
    node_t* current = list->head;

    //make sure that the list isn't empty.
    if (current == NULL) { 
                
        fprintf(stderr, "Error. Attempting to remove values from an empty list.");

        return 0;
    }

    //make sure not attempting to remove a NULL value from the list.
    if (value == NULL) {

        fprintf(stderr, "Error. Attempting to remove a NULL value from the list.\n");

        return 0;
    }

    //used for removal when removing element not at index 0.
    node_t* previous = NULL;
    
    //retrieve length of list.
    int list_length = list->length;

    //retrieve type of list.
    element_type_t list_type = list->e_type;

    //look through all of the elements in the list.
    for (int i = 0; i < list_length; i++) {

        //check for an element in the list equal to the
        //value passed.
        unsigned char equal;
        
        //handle equality for floating point numbers.
        if (list_type == WC_LINKEDLIST_DOUBLE) {

            equal = is_floating_point_element_equal(current->value, value);
        
        } else {

            equal = is_element_equal(value, obj_length, current->value, current->value_length);
        }

        //If an equal element is discovered, remove it.
        if (equal) {
            
            //equal to the first element in the list
            if (previous == NULL) {
                
                //remove current from the list.
                list->head = current->next;

                //free current.
                node_free(current);

            //equality not at first element.
            } else {
            
                //jump over current element.
                previous->next = current->next;
                
                //free the node to remove.
                node_free(current);
            }

            //decrement list length.
            list->length--;

            //return successful removal.
            return 1;            
        }

        //move over one in the list
        previous = current;
        
        current = current->next;
    }

    //element was not found in the list.
    //return that removal failed.
    return 0;
}

/*
* START private print functions
*/

void print_int(node_t* list_head, size_t length) {

    for (size_t i = 0; i < length; i++) {
        
        //print out the integer value stored.
        printf("%d", *(int*)list_head->value);

        //add comma for every element
        //except the last one.
        if (i < length - 1) {
            
            printf("%c ", ',');
        }

        //go to the next element in the list.
        list_head = list_head->next;
    }
}

void print_double(node_t* list_head, size_t length) {

    for (size_t i = 0; i < length; i++) {

        //print out the double value stored.
        printf("%f", *(double*)list_head->value);

        //add comma for every element
        //except the last one.
        if (i < length - 1) {
            
            printf("%c ", ',');
        }

        //go to the next element in the list.
        list_head = list_head->next;
    }
}

void print_string(node_t* list_head, size_t length) {

    for (size_t i = 0; i < length; i++) {

        //print out the string value stored.
        printf("%s", (char*)list_head->value);

        //add comma for every element
        //except the last one.
        if (i < length - 1) {
            
            printf("%c ", ',');
        }
        //go to the next element in the list.
        list_head = list_head->next;
    }
}

void print_obj(node_t* list_head, size_t length) {

    for (size_t i = 0; i < length; i++) {

        //pull and caste bytes of element.
        unsigned char* current_value_bytes = list_head->value;
        
        //determine length of element (how many bytes)
        size_t current_value_length = list_head->value_length;
        
        //print out each of the bytes in the value.
        //(As Hex)
        for (size_t i = 0; i < current_value_length; i++) {

            //only put colons between hex values.
            //not at the front or end elements.
            if (i > 0 && i < current_value_length - 1) {
                
                printf(":");
            }

            //print the current byte in the iteration
            printf("%02x", current_value_bytes[i]);
        }

        //add comma for every element
        //except the last one.
        if (i < length - 1) {
            
            printf("%c ", ',');
        }

        //go to the next element in the list.
        list_head = list_head->next;
    }
}

/*
* END private print functions
*/

//print out a list to the console.
void linked_list_print(linked_list_t* list) {

    //cannot print a NULL list.
    if (list == NULL) {
        
        fprintf(stderr, "Error. Attempting to print out a NULL linked list.\n");

        return;
    }

    //make copy of front of list for iteration.
    node_t* current = list->head;   
    
    //retrieve linked list size
    size_t list_size = list->length;

    //print out the list based
    //on the type of elements stored.
    switch (list->e_type) {
        
        case WC_LINKEDLIST_INT: {
            
            print_int(current, list_size);
            break;
        }

        case WC_LINKEDLIST_DOUBLE: {

            print_double(current, list_size);
            break;
        }

        case WC_LINKEDLIST_STRING: {

            print_string(current, list_size);
            break;
        }

        case WC_LINKEDLIST_OBJ: {
            
            print_obj(current, list_size);
            break;
        }
    }

    printf("\n");
}
