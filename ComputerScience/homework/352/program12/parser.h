#ifndef PARSER_H
#define PARSER_H

#include "list.h"

/* Commands */
extern void title( char *title, List *titles );
extern void saveTitles( char *filename, List *titles );
extern void add( char *command, List *titles );
extern void deleteTitle( char *command, List *titles );

/* Other functions */
extern void parseFile( char *filename, int *flags );
#endif
