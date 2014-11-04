CC = gcc
CFLAGS = -g -Wall -std=c99 -I${INCLUDE}

INCLUDE=/home/cs352/fall14/assign10

all:	skipping

skipping: skipping.o iterative.o recursive.o list.o
	${CC} ${CFLAGS} -o skipping skipping.o

skipping.o: skipping.c skipping.h
	${CC} ${CFLAGS} -c skipping.c

iterative.o: iterative.c iterative.h
	${CC} ${CFLAGS} -c iterative.c

recursive.o: recursive.c recursive.h
	${CC} ${CFLAGS} -c recursive.c

list.o: list.c list.h
	${CC} ${CFLAGS} -c list.c

# Add a clean target that removes the .o files
clean:
	rm *.o
