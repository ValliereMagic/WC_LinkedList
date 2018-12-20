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

    printf("Status of element removal: %d.\nState of list:\n", success);

    linked_list_print(int_list);
}

void modify_double_list(linked_list_t* doub_list) {

    printf("\nAdding some doubles to the list...\n");

    for (double i = 3.14; i <= 15; i++) {
        
        linked_list_add(doub_list, &i, sizeof(double));
    }

    printf("Elements added. State of list:\n");

    linked_list_print(doub_list);

    printf("\nRetrieving the value at element 4:\n");

    list_value_t element_four = linked_list_get(doub_list, 4);

    printf("Value at element 4: %f\n", *(double*)element_four.item);

    printf("Length of element 4 in bytes: %ld\n", (unsigned long)element_four.item_length);

    printf("\nTesting removal of element by index...\n");

    unsigned char success = linked_list_remove_at(doub_list, 0);

    printf("Status of element removal: %d.\nState of list:\n", success);

    linked_list_print(doub_list);


    printf("\nTesting removal of element by value...\n");

    double doub_to_remove = 5.14;

    success = linked_list_remove_value(doub_list, &doub_to_remove, sizeof(double));

    printf("Status of element removal: %d.\nState of list:\n", success);

    linked_list_print(doub_list);

}

void modify_arb_list(linked_list_t* arb_list) {

    printf("\nAdding some bytes to the list...\n");

    unsigned char someBytes[] = {3, 56, 112, 45, 128};

    for (int i = 0; i < 15; i++) {

        linked_list_add(arb_list, someBytes, sizeof(unsigned char) * 5);
    }

    printf("Elements added.\nStatus of list:\n");

    linked_list_print(arb_list);

    printf("\nSetting the element 3 to a new value.\n");

    unsigned char someMoreBytes[] = {8, 34, 90, 234, 65, 12, 78};

    linked_list_set(arb_list, 3, someMoreBytes, sizeof(unsigned char) * 7);

    printf("Element 3 updated.\nStatus of list:\n");

    linked_list_print(arb_list);

    printf("\nRemoving a byte array from the list by index...\n");

    unsigned char success = linked_list_remove_at(arb_list, 12);

    printf("Status of element removal: %d.\nStatus of list:\n", success);

    linked_list_print(arb_list);


    printf("\nRemoving a byte array from the list by value...\n");

    success = linked_list_remove_value(arb_list, someBytes, 
                                       sizeof(unsigned char) * 5);

    printf("Status of element removal: %d.\nStatus of list:\n", success);

    linked_list_print(arb_list);

    printf("Cloning the arbitrary list.\n");

    linked_list_t* cloned_arb_list = linked_list_clone(arb_list);

    printf("Printing the cloned list:\n");

    linked_list_print(cloned_arb_list);

    printf("Freeing the cloned list:\n");

    linked_list_free(cloned_arb_list);
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


    printf("\nTesting arbitrary value list...\n");

    modify_arb_list(arb_list);

    printf("Arbitrary list tests completed.\n");


    printf("\nFreeing linked lists...\n");

    linked_list_free(arb_list);

    linked_list_free(int_list);

    linked_list_free(doub_list);

    linked_list_free(string_list);

    printf("Lists freed.\n");

    return 0;
}
