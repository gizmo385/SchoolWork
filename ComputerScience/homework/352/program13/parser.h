#ifndef PARSER_H
#define PARSER_H

#include "bst.h"

/* Commands */
extern void title( char *title, BST *titles );
extern void saveTitles( char *filename, BST *titles );
extern void add( char *command, BST *titles );
extern void deleteTitle( char *command, BST *titles );

/* Other functions */
extern void parseFile( char *filename, BST *titles );
extern char *processLine( FILE * file );
extern int startsWith( char *string, const char *substring );
#endif
