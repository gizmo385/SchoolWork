#include <stdlib.h>

#include "list.h"

/*
 * Creates a node based on the provided word and reference to the node that comes after it
 *
 * Arguments:
 * word -- The word that serves as the data for this node
 * next -- The node that follows this node in the linked list. IMPORTANT: THIS CAN BE NULL
 *
 * Returns:
 * The created node containing the word and pointing to the provided node
 */
struct node *createNode( char *word, struct node *next ) {
    struct node *newNode = (struct node *) malloc( sizeof( struct node ) );
    newNode->word = word;
    newNode->next = next;

    return newNode;
}

/*
 * Attempts to find the provide string of characters in the linked list. This will return the first
 * node containing the desired word.
 *
 * Arguments:
 * word -- The word that you would like to search for within the list.
 * list -- The starting node for the list that you would like to traverse.
 *
 * Returns:
 * The node containing the desired word. If the word was not found, this will return the NULL node.
 */
struct node *find( char *wordToFind, struct node *list ) {
    struct node *currentNode = list;

    while( currentNode->next != NULL && currentNode->word < wordToFind ) {
        currentNode = currentNode->next;
    }

    return currentNode;
}

/*
 * Inserts the element into the list
 *
 * Arguments:
 * wordToInsert -- The word that you would like to insert into the list
 * list         -- The list that you would like to insert the element into
 */
void insertElement( char *wordToInsert, struct node *list ) {
    struct node *currentNode = find( wordToInsert, list );

    struct node *tempNode = createNode( currentNode->word, currentNode->next );

    currentNode->word = wordToInsert;
    currentNode->next = tempNode;
}

/*
 * Removes the element from the list
 *
 * Arguments:
 * wordToRemove -- The word that you would like to remove from the list
 * list         -- The list that you would like to remove the element from
 *
 * Returns:
 * The head of the list with the element removed.
 */
void removeWord( char *wordToRemove, struct node *list ) {
    struct node *currentNode = find( wordToRemove, list );
    struct node *tempNode = currentNode->next;

    currentNode->word = tempNode->word;
    currentNode->next = tempNode->next;

    free( tempNode );
}

/*
 * Peeks at the first node on the stack and returns the word contained within the node
 *
 * Arguments:
 * list -- The list whose first element we are peeking at
 *
 * Returns:
 * The word contained within the first node. If the list is empty, this will return null
 */
char *peek( struct node *list ) {
    return list->word;
}

/*
 * Pops the first node off of the stack and returns the word contained within the node
 *
 * Arguments:
 * list -- The list to pop off of
 *
 * Returns:
 * The word contained within the node popped off the list. If the list is empty, this will return
 * null.
 */
char *pop( struct node *list ) {
    char *word = NULL;

    if( list != NULL && list->word != NULL ) {
        word = list->word;
        removeWord( word, list);
    }

    return word;
}

/*
 * Pushes a word onto the list. This will create a new node and put it on the head of the list.
 *
 * Arguments:
 * wordToPush -- The word that you would want to push onto the list
 * list       -- The head of the list is being pushed to
 *
 */
void push( char *wordToPush, struct node *list ) {
    struct node *tempNode = createNode( list->word, list->next );
    list->word = wordToPush;
    list->next = tempNode;
}

/*
 * Creates an empty linked list
 *
 * Returns:
 * A null node which represents an empty list.
 */
struct node *createList() {
    return createNode( NULL, NULL );
}
