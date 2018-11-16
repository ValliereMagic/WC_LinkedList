#include <stdio.h>
#include <string.h>
#include "WC_LinkedList.h"

void modify_string_list(linked_list_t* str_list) {

    printf("\nAdding 10 duplicate string elements to the list...\n");

    char random_string[] = "HELLO LIST";

    for (int i = 0; i < 10; i++) {

        linked_list_add(str_list, random_string, strlen(random_string) + 1);
    }

    printf("Elements added. State of list:\n");

    linked_list_print(str_list);

    
    printf("\nTesting removal of element by index...\n");

    unsigned char success = linked_list_remove_at(str_list, 4);

    printf("Status of element removal: %d.\nState of list:\n", success);

    linked_list_print(str_list);

    
    printf("\nTesting removal of element by value...\n");

    success = linked_list_remove_value(str_list, random_string, 
                                                     strlen(random_string) + 1);

    printf("Status of element removal: %d.\nState of list:\n", success);

    linked_list_print(str_list);
}

int main(void) {
    
    printf("Testing the creation of linked list of each type...\n");

    linked_list_t* arb_list = linked_list_new(WC_LINKEDLIST_OBJ);
    
    linked_list_t* int_list = linked_list_new(WC_LINKEDLIST_INT);

    linked_list_t* doub_list = linked_list_new(WC_LINKEDLIST_DOUBLE);

    linked_list_t* string_list = linked_list_new(WC_LINKEDLIST_STRING);

    printf("Creation completed.\n");


    printf("\nTesting string list...\n");

    modify_string_list(string_list);

    printf("String list tests completed.\n");


    printf("\nFreeing linked lists...\n");

    linked_list_free(arb_list);

    linked_list_free(int_list);

    linked_list_free(doub_list);

    linked_list_free(string_list);

    printf("Lists freed.\n");

    return 0;
}
