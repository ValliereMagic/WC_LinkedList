#include <stdio.h>
#include "WC_LinkedList.h"

void test_add_string_element(linked_list_t* list) {
    
    printf("//Adding some elements to a list\\\\\n");
    
    for (int i = 0; i < 1500; i++) {
        linked_list_add(list, "HELLO", WC_LINKEDLIST_STRING, 6);
    }
}

void test_allocate_deallocate(void) {
    printf("//Creating new Linked List\\\\\n");
    linked_list_t* new_list = linked_list_new();

    test_add_string_element(new_list);

    printf("//Freeing newly allocated Linked List\\\\\n");
    linked_list_free(new_list);
}

int main(void) {
    printf("Hello LinkedList\n");

    test_allocate_deallocate();
    return 0;
}