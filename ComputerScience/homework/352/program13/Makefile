CC = gcc
CFLAGS = -g -Wall -std=gnu99

build: catalog

# Primary source targets
catalog: catalog.o processOpts.o utils.o bst.o parser.o
	${CC} ${CFLAGS} -o catalog catalog.o processOpts.o utils.o bst.o parser.o

bst.o: bst.c bst.h utils.o
	${CC} ${CFLAGS} -c bst.c

catalog.o: catalog.c catalog.h utils.o processOpts.o utils.o parser.o
	${CC} ${CFLAGS} -c catalog.c

parser.o: parser.c parser.h utils.o bst.o processOpts.o
	${CC} ${CFLAGS} -c parser.c

# Utility targets
utils.o: utils.c utils.h
	${CC} ${CFLAGS} -c utils.c

processOpts.o: processOpts.c processOpts.h utils.o
	${CC} ${CFLAGS} -c processOpts.c

# Add a clean target that silently removes the .o files
clean:
	@rm *.o catalog 2> /dev/null || true
