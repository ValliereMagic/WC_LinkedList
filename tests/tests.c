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

void modify_int_list(linked_list_t* int_list) {

    printf("\nAdding 10 integers to the list...\n");

    for (int i = 1; i <= 10; i++) {
        
        linked_list_add(int_list, &i, sizeof(int));
    }

    printf("Elements added. State of list:\n");

    linked_list_print(int_list);


    printf("\nTesting removal of element by index...\n");

    unsigned char success = linked_list_remove_at(int_list, 6);

    printf("Status of element removal: %d.\nState of list:\n", success);

    linked_list_print(int_list);


    printf("\nTesting removal of element by value...\n");

    int int_to_remove = 5;

    success = linked_list_remove_value(int_list, &int_to_remove, sizeof(int));

    printf("Status of element removal: %d.\n State of list:\n", success);

    linked_list_print(int_list);
}

void modify_double_list(linked_list_t* doub_list) {

    printf("\nAdding some doubles to the list...\n");

    for (double i = 3.14; i <= 15; i++) {
        
        linked_list_add(doub_list, &i, sizeof(double));
    }

    printf("Elements added. State of list:\n");

    linked_list_print(doub_list);


    printf("\nTesting removal of element by index...\n");

    unsigned char success = linked_list_remove_at(doub_list, 6);

    printf("Status of element removal: %d.\nState of list:\n", success);

    linked_list_print(doub_list);


    printf("\nTesting removal of element by value...\n");

    double doub_to_remove = 5.14;

    success = linked_list_remove_value(doub_list, &doub_to_remove, sizeof(double));

    printf("Status of element removal: %d.\n State of list:\n", success);

    linked_list_print(doub_list);

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


    printf("\nTesting integer list...\n");

    modify_int_list(int_list);

    printf("Integer list tests completed.\n");


    printf("\nTesting double list...\n");

    modify_double_list(doub_list);

    printf("Double list tests completed.\n");


    printf("\nFreeing linked lists...\n");

    linked_list_free(arb_list);

    linked_list_free(int_list);

    linked_list_free(doub_list);

    linked_list_free(string_list);

    printf("Lists freed.\n");

    return 0;
}
