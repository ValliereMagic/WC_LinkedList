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

    //define linked_list type (forward declared).
    typedef struct linked_list_t linked_list_t;

    //free an allocated linked_list.
    void linked_list_free(linked_list_t* list_to_free);

    //create a new linked_list.
    linked_list_t* linked_list_new(element_type_t type);

    //add an element to the linked_list, obj_length is how many bytes the element passed is.
    //make sure that obj_length includes the null terminator if value is a string.
    unsigned char linked_list_add(linked_list_t* list, void* value, size_t obj_length);

    //remove an element from the list using an index.
    //will return 1 on success. 0 Otherwise. (NULL list passed, out of bounds, empty list, etc.)
    unsigned char linked_list_remove_at(linked_list_t* list, unsigned int index);

#endif
