#ifndef QUICKSORT_H
#define QUICKSORT_H

#define MAX_LINES   500000
#define LINE_LENGTH 200

extern void quicksort( char *words[], int length );
extern void merge( char *first[], int firstLength, char *second[], int secondLength );

#endif
