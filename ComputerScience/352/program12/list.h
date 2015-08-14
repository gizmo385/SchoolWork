#ifndef LIST_H
#define LIST_H

typedef struct node {
    char *title;
    struct node *next;
} Node;

typedef struct list {
    Node *head;
    int (*comparisonFunction)(const char *, const char *);
    int reversed;
} List;


extern Node *createNode( char *title, Node *next );

extern Node *find( char *titleToFind, List *list );

extern void insertElement( char *titleToInsert, List *list );

extern void removeTitle( char *titleToRemove, List *list );

extern List *createList( int (*comparisonFunction)(const char *, const char *), int reversed );

#endif
