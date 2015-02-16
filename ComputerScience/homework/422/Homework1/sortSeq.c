#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "quicksort.h"

int main( int argc, char *argv[] ) {
    char *filename = argv[1];

    FILE *file = fopen(filename, "r");
    char **lines = calloc( MAX_LINES, sizeof(char *)  );
    int length = 0;

    // Read lines from the file
    while( !feof(file) ) {
        char *line = calloc(LINE_LENGTH, sizeof(char));

        fgets(line, LINE_LENGTH, file);
        lines[length] = line;
        length++;
    }

    clock_t start = clock();
    quicksort(lines, length);
    double microseconds = ((double) (clock() - start) / CLOCKS_PER_SEC * 1000000 );

    int seconds = (int)(microseconds / CLOCKS_PER_SEC);

    for( int i = 0; i < length; i++ ) {
        printf("%s", lines[i]);
    }

    printf( "runtime: %d seconds, %f microseconds\n", seconds, microseconds  );

    // Free the memory used by the program
    for( int i = 0; i < length; i++ ) {
        free(lines[i]);
    }

    free( lines );
    fclose( file );
}
