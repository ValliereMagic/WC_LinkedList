#ifndef WC_LINKEDLIST_H
    #define WC_LINKEDLIST_H
    /*
    * Possible types for list:
    *   WC_LINKEDLIST_INT: integer stored.
    *   WC_LINKEDLIST_DOUBLE: a double is stored.
    *   WC_LINKEDLIST_STRING: a null terminated string is stored.
    *   WC_LINKEDLIST_OBJ: something else is stored. (could be whatever you want!).
    */
    enum linked_list_type {
        WC_LINKEDLIST_INT, WC_LINKEDLIST_DOUBLE, WC_LINKEDLIST_STRING, WC_LINKEDLIST_OBJ
    };
    
    struct list_value {
        //length in bytes of the item returned.
        size_t item_length;
        //pointer to the first byte of the item.
        void* item;
    };
    //define linked_list type (forward declared).
    struct linked_list;
    //define linked list iterator (forward declared).
    struct linked_list_iterator;
    //List iterator functions:
    //create a new struct linked_list_iterator and return it.
    //will return NULL on failure. 
    //Must be freed when done using.
    struct linked_list_iterator* linked_list_get_iterator(struct linked_list* list);
    //free a linked list iterator
    void linked_list_free_iterator(struct linked_list_iterator* list_it);
    //Function checks to see if theres another element in the list 
    //after the current one.
    unsigned char linked_list_has_next(struct linked_list_iterator* list_it);
    //retrieve the next element in the list; on the first call from
    //a new iterator the first element in the list will be returned.
    //will return NULL on error.
    void* linked_list_get_next(struct linked_list_iterator* list_it);
    //List functions:
    //free an allocated struct linked_list.
    void linked_list_free(struct linked_list* list_to_free);
    //create a new struct linked_list.
    struct linked_list* linked_list_new(enum linked_list_type type);
    //add an element to the struct linked_list, obj_length is how many bytes the element passed is.
    //make sure that obj_length includes the null terminator if value is a string.
    unsigned char linked_list_add(struct linked_list* list, void* value, size_t obj_length);
    //Get the value at the passed index of the list
    //returns NULL on failure.
    //item within list_value is a valid pointer in the list.
    //make sure that if the data it points to is to be manipulated
    //that the data is cloned to a local variable first.
    struct list_value linked_list_get(struct linked_list* list, size_t index);
    //set an element in the struct linked_list, obj_length is how many bytes the element passed is.
    //make sure that obj_length includes the null terminator if value is a string.
    unsigned char linked_list_set(struct linked_list* list, size_t index, void* value, size_t obj_length);
    //check whether the list contains a value
    //returns 1 if the value exists in the list. 0 when the value isn't there.
    unsigned char linked_list_contains(struct linked_list* list, void* value, size_t obj_length);
    //return the length of the list
    //returns a size of 0 when the list is NULL.
    size_t linked_list_size(struct linked_list* list);
    //clone the list passed and return. Returned list must be freed using linked_list_free.
    struct linked_list* linked_list_clone(struct linked_list* list);
    //shuffle the order of values in the linked list
    void linked_list_shuffle(struct linked_list* list);
    //remove an element from the list using an index.
    //will return 1 on success. 0 Otherwise. (NULL list passed, out of bounds, empty list, etc.)
    unsigned char linked_list_remove_at(struct linked_list* list, size_t index);
    //remove an element from the list using a value stored in it.
    //will return 1 on success. 0 Otherwise.
    int linked_list_remove_value(struct linked_list* list, void* value, size_t obj_length);
    //print out all of the elements of a linked list.
    //will print out the hex values for a list of type WC_LINKEDLIST_OBJ
    void linked_list_print(struct linked_list* list);
#endif
