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


extern struct node *createNode( char *word, struct node *next );

extern struct node *find( char *wordToFind, struct node *list );

extern void insertElement( char *wordToInsert, struct node *list );

extern void removeWord( char *wordToRemove, struct node *list );

extern char *pop( struct node *list );

extern char *peek( struct node *list );

extern void push( char *wordToPush, struct node *list );

extern struct node *createList();
