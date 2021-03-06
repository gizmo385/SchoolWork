#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "utils.h"

ListNode *newListNode( void* data, ListNode *next ) {
    ListNode *node = malloc( sizeof(ListNode) );
    node->data = data;
    node->next = next;

    return node;
}

List *newList( ComparisonFunction comparisonFunction ) {
    List *list = malloc( sizeof(List) );
    list->size = 0;
    list->head = newListNode(NULL, NULL);

    if( comparisonFunction != NULL ) {
        list->comparisonFunction = comparisonFunction;
    }

    return list;
}

void listInsert( List *list, void *data ) {
    // You cannot insert NULL into the list
    if( data == NULL ) {
        return;
    }

    ListNode *current = list->head;
    ComparisonFunction compare = list->comparisonFunction;

    // Determine where the data will be inserted
    if(compare) {
        while( current->next != NULL && compare(current->data, data) < 0 ) {
            current = current->next;
        }
    }

    // Insert the element
    ListNode *tempNode = newListNode( current->data, current->next );
    current->data = data;
    current->next = tempNode;
    list->size += 1;
}

ListNode *listFind( List *list, void *data ) {
    ListNode *current = list->head;
    ComparisonFunction compare = list->comparisonFunction;

    if(!compare) {
        return NULL;
    }

    while( current->next != NULL && compare( current->data, data ) != 0 ) {
        current = current->next;
    }

    // If the data in current isn't equal to the data we're trying to find, return NULL
    if( compare( current->data, data ) != 0 ) {
        return NULL;
    } else {
        return current;
    }
}

void listFree( List *list ) {
    ListNode *current = list->head;

    // Free the nodes along the list
    while( current->next != NULL ) {
        ListNode *next = current->next;
        free( current->data );
        free( current );
        current = next;
    }

    // Free the last node and the list
    free( current->data );
    free( current );
    free( list );
}
