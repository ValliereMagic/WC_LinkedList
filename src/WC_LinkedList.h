#ifndef WC_LINKEDLIST_H

    #define WC_LINKEDLIST_H

    /*
    * Possible types for list:
    *   WC_LINKEDLIST_INT: integer stored.
    *   WC_LINKEDLIST_DOUBLE: a double is stored.
    *   WC_LINKEDLIST_STRING: a null terminated string is stored.
    *   WC_LINKEDLIST_OBJ: something else is stored. (could be whatever you want!).
    */

    typedef enum element_type_t {
        
        WC_LINKEDLIST_INT, WC_LINKEDLIST_DOUBLE, WC_LINKEDLIST_STRING, WC_LINKEDLIST_OBJ
    
    } element_type_t;

    typedef struct list_value_t {
        
        //length in bytes of the item returned.
        size_t item_length;

        //pointer to the first byte of the item.
        void* item;
    
    } list_value_t;

    //define linked_list type (forward declared).
    typedef struct linked_list_t linked_list_t;

    //define linked list iterator (forward declared).
    typedef struct linked_list_iterator_t linked_list_iterator_t;

    //List iterator functions:

    //create a new linked_list_iterator and return it.
    //will return NULL on failure. 
    //Must be freed when done using.
    linked_list_iterator_t* linked_list_iterator(linked_list_t* list);

    //free a linked list iterator
    void linked_list_free_iterator(linked_list_iterator_t* list_it);

    //Function checks to see if theres another element in the list 
    //after the current one.
    unsigned char linked_list_has_next(linked_list_iterator_t* list_it);

    //retrieve the next element in the list; on the first call from
    //a new iterator the first element in the list will be returned.
    //will return NULL on error.
    void* linked_list_get_next(linked_list_iterator_t* list_it);

    //List functions:

    //free an allocated linked_list.
    void linked_list_free(linked_list_t* list_to_free);

    //create a new linked_list.
    linked_list_t* linked_list_new(element_type_t type);

    //add an element to the linked_list, obj_length is how many bytes the element passed is.
    //make sure that obj_length includes the null terminator if value is a string.
    unsigned char linked_list_add(linked_list_t* list, void* value, size_t obj_length);

    //Get the value at the passed index of the list
    //returns NULL on failure.
    //item within list_value_t is a valid pointer in the list.
    //make sure that if the data it points to is to be manipulated
    //that the data is cloned to a local variable first.
    list_value_t linked_list_get(linked_list_t* list, size_t index);

    //set an element in the linked_list, obj_length is how many bytes the element passed is.
    //make sure that obj_length includes the null terminator if value is a string.
    unsigned char linked_list_set(linked_list_t* list, size_t index, void* value, size_t obj_length);

    //check whether the list contains a value
    //returns 1 if the value exists in the list. 0 when the value isn't there.
    unsigned char linked_list_contains(linked_list_t* list, void* value, size_t obj_length);
   
    //return the length of the list
    //returns a size of 0 when the list is NULL.
    size_t linked_list_size(linked_list_t* list);

    //clone the list passed and return. Returned list must be freed using linked_list_free.
    linked_list_t* linked_list_clone(linked_list_t* list);

    //shuffle the order of values in the linked list
    void linked_list_shuffle(linked_list_t* list);

    //remove an element from the list using an index.
    //will return 1 on success. 0 Otherwise. (NULL list passed, out of bounds, empty list, etc.)
    unsigned char linked_list_remove_at(linked_list_t* list, size_t index);

    //remove an element from the list using a value stored in it.
    //will return 1 on success. 0 Otherwise.
    int linked_list_remove_value(linked_list_t* list, void* value, size_t obj_length);

    //print out all of the elements of a linked list.
    //will print out the hex values for a list of type WC_LINKEDLIST_OBJ
    void linked_list_print(linked_list_t* list);

#endif
