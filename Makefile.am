#Include M4 Macros
ACLOCAL_AMFLAGS = -I m4

#Tests
check_PROGRAMS = list_tests

#Tests sources
list_tests_SOURCES = \
    tests/tests.c

#Tests CFlags

#include linkedlist header file
list_tests_CFLAGS = -I./src/

#Link tests to Linked list
list_tests_LDADD = libWC_linkedlist.la

#Linkedlist Library
lib_LTLIBRARIES = libWC_linkedlist.la

#Linkedlist Sources
libWC_linkedlist_la_SOURCES = \
    src/WC_LinkedList.h \
    src/WC_LinkedList.c

#Linkedlist Version
libWC_linkedlist_la_LDFLAGS = -version-info 1:0:0 -no-undefined

libWC_linkedlist_la_LIBADD = @WC_sodium_flags@

#Install linked list headers
include_HEADERS = src/WC_LinkedList.h
