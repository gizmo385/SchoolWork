CC = gcc
CFLAGS = -g -Wall -std=gnu99

build: clean catalog

debug: catalog-debug

catalog: catalog.o processOpts.o utils.o list.o parser.o
	${CC} ${CFLAGS} -o catalog catalog.o processOpts.o utils.o list.o parser.o

catalog-debug: catalog-debug.o processOpts.o utils.o list.o parser.o
	${CC} ${CFLAGS} -o catalog catalog-debug.o processOpts.o utils.o list.o parser.o

list.o: list.c list.h utils.o
	${CC} ${CFLAGS} -c list.c

catalog.o: catalog.c catalog.h utils.o processOpts.o utils.o parser.o
	${CC} ${CFLAGS} -c catalog.c

catalog-debug.o: catalog.c catalog.h utils.o processOpts.o utils.o parser.o
	${CC} ${CFLAGS} -DDEBUG -o catalog-debug.o -c catalog.c

parser.o: parser.c parser.h utils.o list.o processOpts.o
	${CC} ${CFLAGS} -c parser.c

# Utility targets
utils.o: utils.c utils.h
	${CC} ${CFLAGS} -c utils.c

processOpts.o: processOpts.c processOpts.h utils.o
	${CC} ${CFLAGS} -c processOpts.c

# Add a clean target that silently removes the .o files
clean:
	@rm *.o catalog 2> /dev/null || true
