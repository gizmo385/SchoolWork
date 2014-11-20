#include <stdlib.h>

#include "list.h"

/*
 * Creates a node based on the provided title and reference to the node that comes after it
 *
 * Arguments:
 * title -- The title that serves as the data for this node
 * next -- The node that follows this node in the linked list. IMPORTANT: THIS CAN BE NULL
 *
 * Returns:
 * The created node containing the title and pointing to the provided node
 */
Node *createNode( char *title, Node *next ) {
    Node *newNode = (Node *) malloc( sizeof( Node ) );
    newNode->title = title;
    newNode->next = next;

    return newNode;
}

/*
 * Attempts to find the provide string of characters in the linked list. This will return the first
 * node containing the desired title.
 *
 * Arguments:
 * title -- The title that you would like to search for within the list.
 * list -- The starting node for the list that you would like to traverse.
 *
 * Returns:
 * The node containing the desired title. If the title was not found, this will return the NULL node.
 */
Node *find( char *titleToFind, Node *list ) {
    Node *currentNode = list;

    while( currentNode->next != NULL && currentNode->title < titleToFind ) {
        currentNode = currentNode->next;
    }

    return currentNode;
}

/*
 * Inserts the element into the list
 *
 * Arguments:
 * titleToInsert -- The title that you would like to insert into the list
 * list         -- The list that you would like to insert the element into
 */
void insertElement( char *titleToInsert, Node *list ) {
    Node *currentNode = find( titleToInsert, list );

    Node *tempNode = createNode( currentNode->title, currentNode->next );

    currentNode->title = titleToInsert;
    currentNode->next = tempNode;
}

/*
 * Removes the element from the list
 *
 * Arguments:
 * titleToRemove -- The title that you would like to remove from the list
 * list         -- The list that you would like to remove the element from
 *
 * Returns:
 * The head of the list with the element removed.
 */
void removeTitle( char *titleToRemove, Node *list ) {
    Node *currentNode = find( titleToRemove, list );
    Node *tempNode = currentNode->next;

    currentNode->title = tempNode->title;
    currentNode->next = tempNode->next;

    free( tempNode );
}

/*
 * Creates an empty linked list
 *
 * Returns:
 * A null node which represents an empty list.
 */
List *createList( int (*comparisonFunction)(char *, char *) ) {
    List *list = (List *) malloc( sizeof(List) );
    Node *head = createNode(NULL, NULL);

    list->head = head;
    list->comparisonFunction = comparisonFunction;

    return list;
}
