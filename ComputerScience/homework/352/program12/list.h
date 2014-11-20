#ifndef LIST_H
#define LIST_H

typedef struct node {
    char *title;
    struct node *next;
} Node;

typedef struct list {
    Node *head;
    int (*comparisonFunction)(char *, char *);
} List;


extern Node *createNode( char *title, Node *next );

extern Node *find( char *titleToFind, Node *list );

extern void insertElement( char *titleToInsert, Node *list );

extern void removeTitle( char *titleToRemove, Node *list );

extern List *createList( int (*comparisonFunction)(char *, char *) );

#endif
