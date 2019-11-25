#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "WC_LinkedList.h"
#include "sodium.h"

//List node
struct node {
    //value that the node holds.
    void* value;
    //bytes of memory value takes up.
    size_t value_length;
    //next node in the list.
    struct node* next;
};

//List
struct linked_list {
    //front node of the list.
    struct node* head;
    //end of the list
    struct node* tail;
    //number of nodes in the list.
    size_t length;
    //type of the element stored.
    enum linked_list_type e_type;
};

//List iterator
struct linked_list_iterator {
    //current iterator node
    struct node* current;
    //new flag, to return current on first
    //get_next
    unsigned char is_new;
    //current index of iteration
    size_t current_index;
    //max iteration index
    size_t max_index;
};

/*
* List iterator functions
*/

//create a new linked_list_iterator and return it. will return NULL on failure.
//Must be freed when done using.
struct linked_list_iterator* linked_list_get_iterator(struct linked_list* list) {
    struct linked_list_iterator* it = malloc(sizeof(struct linked_list_iterator));
    //make sure that the system returned memory for the iterator, and that the
    //list exists, and make sure the list has at least one element.
    if (it == NULL || list == NULL || list->head == NULL || list->length == 0) {
        return NULL;
    }
    //initialize iterator values.
    it->current = list->head;
    it->is_new = 0;
    it->current_index = 0;
    it->max_index = list->length - 1;
    return it;
}

//free a linked list iterator
void linked_list_free_iterator(struct linked_list_iterator* list_it) {
    free(list_it);
}

//Function checks to see if there is another element in the list after the
//current one.
unsigned char linked_list_has_next(struct linked_list_iterator* list_it) {
    //Checks if the list iterator actually exists before attempting to
    //dereference its properties
    if (list_it == NULL) {
        return 0;
    }
    //Check to see if there is another element in the linked list. If there is
    //an element return 1 otherwise return 0 
    // handle the case where there is exactly 1 element in the list, and the list
	// is new.
	if (list_it->current_index == 0 && list_it->max_index == 0 &&
	    list_it->is_new == 0)
		return 1;
	else
		return (list_it->current_index < list_it->max_index);
}

//retrieve the next element in the list; on the first call from a new iterator
//the first element in the list will be returned.
void* linked_list_get_next(struct linked_list_iterator* list_it) {
    //make sure the list iterator exists.
    if (list_it == NULL) {
        return NULL;
    }
    //on the first call return the head of the list otherwise, element 0 will
    //never be iterated over.
    if (list_it->is_new == 0) {
        list_it->is_new = 1;
        struct node* current = list_it->current;
        //make sure the current node in the iterator exists.
        if (current == NULL) {
            return NULL;
        }
        return current->value;
    } else {
        //pull current out of the iterator
        struct node* current = list_it->current;
        //make sure current exists.
        if (current == NULL) {
            return NULL;
        }
        //jump to the next element to retrieve.
        current = current->next;
        //make sure that the element after current exists.
        if (current == NULL) {
            return NULL;
        }
        //move the iterator to the next element in the list.
        list_it->current = current;
        //increment the iterator's index
        list_it->current_index++;
        //return the value stored in the next element.
        return current->value;
    }
}

/*
* Private Functions
*/

//function for freeing a single node.
static void node_free(struct node* node_to_free) {
    if (node_to_free == NULL || node_to_free->value == NULL) { 
        fputs("Error. Attempting to free NULL node.\n", stderr);
        return;
    }
    free(node_to_free->value);
    free(node_to_free);
}

//function for allocating memory for a new list element's value.
static void* allocate_element(void* value, size_t obj_length) {
    //allocate memory required for new element.
    void* new_list_element = malloc(obj_length);
    //copy the passed value into it's new container.
    memcpy(new_list_element, value, obj_length);
    return new_list_element;
}

//one if the elements are equal, 0 otherwise.
static unsigned char is_element_equal(char* value_one, size_t value_one_length,
                                      char* value_two, size_t value_two_length) {
    //make sure each element has the same length.
    if (value_one_length != value_two_length) {
        //elements are not the same length, therefore they cannot be equal.
        return 0;
    }
    //check each byte of value_one and value_two to see if they are equal.
    for (size_t i = 0; i < value_one_length; i++) {
        //if one byte isn't equal, the elements aren't equal.
        if (value_one[i] != value_two[i]) {
            return 0;
        }
    }
    //elements are equal.
    return 1;
}

//one if the elements are equal, 0 otherwise.
static unsigned char is_floating_point_element_equal(double* value_one, double* value_two) {
    //allowed inaccuracy
    double epsilon = 0.00001;
    return (fabs(*value_one - *value_two) < epsilon);
}

//retrieve the node at an index in the list will return NULL on failure.
static struct node* linked_list_get_node(struct linked_list* list, size_t index) {
    //make sure the list exists.
    if (list == NULL) {
        fputs("Error, attempting to retrieve a node from a NULL linked list.\n", stderr);
        return NULL;
    }
    //make sure that the index is within the bounds of the list.
    if (index > list->length) {
        fputs("Error. Attempting to get element from an index out of bounds.\n", stderr);
        return NULL;
    }
    //retrieve head of list for iteration
    struct node* list_iterator = list->head;
    //jump to the node at the index specified.
    for (size_t i = 0; i < index; i++) {
        list_iterator = list_iterator->next;
    }
    return list_iterator;
}

//Returns 1 when a value is successfully found. previous will be set to the node
//previous to the node found to be equal. This is useful for the
//linked_list_remove_value function.
static unsigned char linked_list_find_value(struct linked_list* list, void* value, 
                                            size_t obj_length, struct node** previous) {
    //make sure list exists.
    if (list == NULL) { 
        fputs("Error. Attempting to remove a value from a NULL list.\n", stderr);
        return 0;
    }
    //save head to make sure list itself doesn't lose it's head.
    struct node* current = list->head;
    //make sure that the list isn't empty.
    if (current == NULL) { 
        fputs("Error. Attempting to remove values from an empty list.", stderr);
        return 0;
    }
    //make sure not attempting to remove a NULL value from the list.
    if (value == NULL) {
        fputs("Error. Attempting to remove a NULL value from the list.\n", stderr);
        return 0;
    }
    //make sure that previous is not NULL If it is NULL previous will not be
    //used.
    if (previous != NULL) {
        //used for removal when removing element not at index 0.
        *previous = NULL;
    }
    //retrieve length of list.
    int list_length = list->length;
    //retrieve type of list.
    enum linked_list_type list_type = list->e_type;
    //look through all of the elements in the list.
    for (int i = 0; i < list_length; i++) {
        //check for an element in the list equal to the value passed.
        unsigned char equal;
        //handle equality for floating point numbers.
        if (list_type == WC_LINKEDLIST_DOUBLE) {
            equal = is_floating_point_element_equal(current->value, value);
        } else {
            equal = is_element_equal(value, obj_length, current->value, current->value_length);
        }
        //Found an element in the list equal to the value passed.
        if (equal) {
            return 1;
        }
        //move over one in the list only using previous if a valid previous
        //pointer passed to function.
        if (previous != NULL) {
            *previous = current;
        }
        current = current->next;
    }
    //Failed to find the value in the list. Return 0.
    return 0;    
}

/*
* Public Functions
*/

//free a linked list from memory.
void linked_list_free(struct linked_list* list_to_free) {
    if (list_to_free == NULL) { 
        fputs("Error. Attempting to free null linked list.\n", stderr);
        return;
    }
    struct node* list_head = list_to_free->head;
    size_t list_size = list_to_free->length;
    //free every node in the list.
    struct node* temp_node;
    
    for (size_t i = 0; i < list_size; i++) {
        //save the next one in the list so it can be freed next.
        temp_node = list_head->next;
        //free the current one.
        node_free(list_head);
        //move on to the next node.
        list_head = temp_node;
    }
    //free the list itself after all nodes freed.
    free(list_to_free);
}

//create a new empty linked list.
struct linked_list* linked_list_new(enum linked_list_type type) {
    //allocate memory required for a new list
    struct linked_list* new_list = malloc(sizeof(struct linked_list));
    //make sure that the list allocation was successful.
    if (new_list == NULL) {
        fputs("Error. Allocation of a new Linked List failed. System may be out of memory.\n", stderr);
        return NULL;
    }
    //set list properties to default values
    new_list->head = NULL;
    new_list->tail = NULL;
    new_list->length = 0;
    new_list->e_type = type;
    return new_list;
}

//add a new element to the list.
unsigned char linked_list_add(struct linked_list* list, void* value, size_t obj_length) {
    //make sure the passed list exists.
    if (list == NULL) { 
        fputs("Error. attempting to add an element to a NULL Linked List\n", stderr);
        //return that the addition failed.
        return 0;
    }
    //make sure a null value is not being added to the list.
    if (value == NULL) {
        fputs("Error. attempting to add a NULL element to a list.\n", stderr);
        //return that the addition failed.
        return 0;
    }
    //create a new node to add to the list.
    struct node* new_node = malloc(sizeof(struct node));
    //allocate a new container for the original value passed.
    void* new_value = allocate_element(value, obj_length);
    //make sure that allocation was successful.
    if (new_value == NULL) {
        fputs("Error. System out of memory, allocating a new element failed.\n", stderr);
        return 0;
    }
    new_node->value = new_value;
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
        struct node* list_tail = list->tail;
        //make sure the tail exists before attempting to dereference tail->next
        if (list_tail == NULL) {
            fputs("Error. the tail of the list is NULL.\n", stderr);
        }
        //set the new tail of the list
        list_tail->next = new_node;
        //update the tail node
        list->tail = list_tail->next;
    }
    //add one to the list length, as an element has been added.
    list->length++;
    //return that the addition was successful.
    return 1;    
}

//Get the value at the passed index of the list returns NULL on failure. Item
//within WC_list_value is a valid pointer in the list. make sure that if the
//data it points to is to be manipulated that the data is cloned to a local
//variable first.
struct list_value linked_list_get(struct linked_list* list, size_t index) {
    //initialize value in case element_to_retrieve is NULL.
    struct list_value value_to_return;
    value_to_return.item_length = 0;
    value_to_return.item = NULL;
    //get the element at the index specified.
    struct node* element_to_retrieve = linked_list_get_node(list, index);
    //list may be null, or maybe index out of bounds.
    if (element_to_retrieve == NULL) {
        return value_to_return;
    }
    //copy the values into the return object.
    value_to_return.item_length = element_to_retrieve->value_length;
    value_to_return.item = element_to_retrieve->value;
    return value_to_return;
}

//change a value in a node to another value.
unsigned char linked_list_set(struct linked_list* list, size_t index, void* value, size_t obj_length) {
    struct node* list_element_to_modify = linked_list_get_node(list, index);
    //list could be NULL, or index could be out of bounds.
    if (list_element_to_modify == NULL) {
        return 0;
    }
    //make a copy of the element's value pointer to manipulate.
    void* list_element_value = list_element_to_modify->value;
    //reallocate the value in the element to the right size
    list_element_value = realloc(list_element_value, obj_length);
    //if the memory reallocation fails don't modify the list, and return
    //unsuccessful.
    if (list_element_value == NULL) {
        return 0;
    }
    //copy in the new value
    memcpy(list_element_value, value, obj_length);
    list_element_to_modify->value = list_element_value;
    list_element_to_modify->value_length = obj_length;
    return 1;
}

//check whether the list contains a value returns 1 if the value exists in the
//list. 0 when the value isn't there.
unsigned char linked_list_contains(struct linked_list* list, void* value, size_t obj_length) {
    return linked_list_find_value(list, value, obj_length, NULL);
}

//return the length of the list
size_t linked_list_size(struct linked_list* list) {
    if (list == NULL) {
        //list doesn't exist therefore it has no length.
        return 0;
    }
    return list->length;
}

//clone the list passed.
struct linked_list* linked_list_clone(struct linked_list* list) {
    //make sure the list to clone exists.
    if (list == NULL) {
        fputs("Error. Cannot clone a NULL list.\n", stderr);
    }
    //retrieve list type and length.
    size_t list_length = list->length;
    enum linked_list_type list_type = list->e_type;
    //allocate the new list to populate
    struct linked_list* new_list = linked_list_new(list_type);
    //set the new list to have the same type as the original list.
    new_list->e_type = list_type;
    //original list iterator
    struct node* original_list_current = list->head;
    //list is empty, returning the new empty list.
    if (original_list_current == NULL) {
        return new_list;
    }
    //add every value in the original list to the new list.
    for (size_t i = 0; i < list_length; i++) {
        //add to new node in new list. (from the values stored in
        //original_list_current).
        linked_list_add(new_list, original_list_current->value, original_list_current->value_length);
        //iterate to the next node in the original list.
        original_list_current = original_list_current->next;
    }
    return new_list;
}

//shuffle the order of values in the linked list
void linked_list_shuffle(struct linked_list* list) {
    //make sure that the list actually exists.
    if (list == NULL) {
        fputs("Error. Cannot clone a NULL list.\n", stderr);
        return;
    }
    //Initiate libsodium, and return on failure.
    if (sodium_init() < 0) {
        return;
    }
    size_t list_length = list->length;
    //iterate through every element in the list
    for (size_t i = 0; i < list_length; i++) {
        uint32_t random_index = randombytes_uniform((uint32_t)list_length);
        //retrieve the element at the random index
        struct node* random_list_node = linked_list_get_node(list, (size_t)random_index);
        struct node* current_node = linked_list_get_node(list, i);
        //Make sure that get_node didn't have any issues.
        if (random_list_node == NULL || current_node == NULL) {
            fputs("Error. Unable to retrieve within bounds nodes in "
                  "linked_list_shuffle.\n", stderr);
            return;
        }
        //retrieve the value and length of the current node
        void* temp_value = current_node->value;
        size_t temp_val_length = current_node->value_length;
        //replace the value and length in the current node with the value and
        //length in the random node.
        current_node->value = random_list_node->value;
        current_node->value_length = random_list_node->value_length;
        //replace the value and length in the random node with the saved value
        //and length that were in the current_node.
        random_list_node->value = temp_value;
        random_list_node->value_length = temp_val_length;
    }
}

//remove an element from the list at an index
unsigned char linked_list_remove_at(struct linked_list* list, size_t index) {
    //make sure list exists.
    if (list == NULL) { 
        fputs("Error. Attempting to remove a value from a NULL list.\n", stderr);
        return 0;
    }
    //calculate the maximum value that the index can be.
    size_t max_index_value = list->length - 1;
    //make sure that the index is within the size of the list.
    if (index > max_index_value) {
        fputs("Error. Attempting to remove at an index out of bounds.\n", stderr);
        return 0;
    }
    //copy list head for iteration.
    struct node* list_head = list->head;
    //make sure that the list isn't empty.
    if (list_head == NULL) {
        fputs("Error. Attempting to remove values from an empty list.", stderr);
        return 0;
    }
    //handle removing from list with 1 element. It is implied that index is also
    //0, because index cannot be less than 0; and index is less than or equal to
    //max_index_value.
    if (max_index_value == 0) {
        //free only element in the list.
        node_free(list_head);
        list->head = NULL;
    //Removing the first element in the list when the list has more than one
    //element.
    } else if (index == 0) {
        //replace the head with the next element in the list.
        list->head = list_head->next;
        //free the head element.
        node_free(list_head);
    } else {
        //index must be > 0 and <= max_index_value for code below this comment
        //to execute. get the element before the one to be removed.
        list_head = linked_list_get_node(list, index - 1);
        //get_node must have failed...
        if (list_head == NULL) {
            return 0;
        }
        //now at the element infront of the one to remove.
        struct node* node_to_free = list_head->next;
        // make sure the node to free is not the tail of the list.
		// If it is, set it to the element before it.
		if (node_to_free == list->tail) {
			list->tail = list_head;
		}
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
int linked_list_remove_value(struct linked_list* list, void* value, size_t obj_length) {
    //previous node to the one containing the value: (if found)
    struct node* previous;
    //If an equal element is discovered, remove it.
    if (linked_list_find_value(list, value, obj_length, &previous)) {
        //equal to the first element in the list
        if (previous == NULL) {
            //make a copy of head, to free its value when it is skipped over.
            struct node* temp = list->head;
            //remove head from the list.
            list->head = temp->next;
            //free current.
            node_free(temp);
        //equality not at first element.
        } else {
            //get the node to remove from the list. we know it exists, otherwise
            //linked_list_find_value would return 0.
            struct node* current = previous->next;
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
    //element was not found in the list. return that removal failed.
    return 0;
}

/*
* START private print functions
*/

static void print_int(struct node* list_head, size_t length) {
    for (size_t i = 0; i < length; i++) {
        //print out the integer value stored.
        printf("%d", *(int*)list_head->value);
        //add comma for every element except the last one.
        if (i < length - 1) {
            printf("%c ", ',');
        }
        //go to the next element in the list.
        list_head = list_head->next;
    }
}

static void print_double(struct node* list_head, size_t length) {
    for (size_t i = 0; i < length; i++) {
        //print out the double value stored.
        printf("%f", *(double*)list_head->value);
        //add comma for every element except the last one.
        if (i < length - 1) {
            printf("%c ", ',');
        }
        //go to the next element in the list.
        list_head = list_head->next;
    }
}

static void print_string(struct node* list_head, size_t length) {
    for (size_t i = 0; i < length; i++) {
        //print out the string value stored.
        printf("%s", (char*)list_head->value);
        //add comma for every element except the last one.
        if (i < length - 1) {
            printf("%c ", ',');
        }
        //go to the next element in the list.
        list_head = list_head->next;
    }
}

static void print_obj(struct node* list_head, size_t length) {
    for (size_t i = 0; i < length; i++) {
        //pull and caste bytes of element.
        unsigned char* current_value_bytes = list_head->value;
        //determine length of element (how many bytes)
        size_t current_value_length = list_head->value_length;
        //print out each of the bytes in the value. (As Hex)
        for (size_t i = 0; i < current_value_length; i++) {
            //only put colons between hex values. not at the front or end
            //elements.
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
void linked_list_print(struct linked_list* list) {
    //cannot print a NULL list.
    if (list == NULL) {
        fputs("Error. Attempting to print out a NULL linked list.\n", stderr);
        return;
    }
    //make copy of front of list for iteration.
    struct node* current = list->head;
    //retrieve linked list size
    size_t list_size = list->length;
    //print out the list based on the type of elements stored.
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
