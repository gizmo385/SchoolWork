/*
 * A node, in this case, is defined as a char* that represents a word and a pointer to another node
 * in the list.
 *
 * Elements:
 * word -- The contents of this node. Represents a word being read from a file.
 * next -- The node the immediately follows this node.
 */
struct node {
    char *word;
    struct node *next;
};


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
extern struct node *createNode( char *word, struct node *next );

/*
 * Attempts to find the provide string of characters in the linked list. This will return the first
 * node containing the desired word.
 *
 * Arguments:
 * word -- The word that you would like to search for within the list.
 * list -- The starting node for the list that you would like to traverse.
 *
 * Returns:
 * The node containing the desired word. This can be NULL.
 */
extern struct node *find( char *wordToFind, struct node *list );

/*
 * Inserts the element into the list
 *
 * Arguments:
 * wordToInsert -- The word that you would like to insert into the list
 * list         -- The list that you would like to insert the element into
 *
 * Returns:
 * The head of the list with the element inserted.
 */
extern struct node *insertElement( char *wordToInsert, struct node *list );

/*
 * Inserts the node into the list
 *
 * Arguments:
 * nodeToInsert -- The node that you would like to insert into the list
 * list         -- The list that you would like to insert the element into
 *
 * Returns:
 * The head of the list with the node inserted
 */
extern struct node *insertNode( struct node *nodeToInsert, struct node *list );

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
extern struct node *removeElement( char *wordToRemoved, struct node *list );

/*
 * Removes the node from the list
 *
 * Arguments:
 * nodeToRemove -- The node that you would like to remove from the list
 * list         -- The list that you would like to remove the element from
 *
 * Returns:
 * The head of the list with the node removed.
 */
extern struct node *removeElement( char *nodeToRemove, struct node *list );
