CC = gcc
CFLAGS = -g -Wall -std=c99 -I${INCLUDE}

INCLUDE=/home/cs352/fall14/assign10

all:	skipping

skipping: skipping.o list.o processOpts.o printer.o
	${CC} ${CFLAGS} -o skipping skipping.o processOpts.o list.o printer.o

skipping.o: skipping.c
	${CC} ${CFLAGS} -c skipping.c

list.o: list.c list.h
	${CC} ${CFLAGS} -c list.c

processOpts.o: processOpts.c processOpts.h
	${CC} ${CFLAGS} -c processOpts.c

printer.o: printer.c printer.h
	${CC} ${CFLAGS} -c printer.c

# Add a clean target that removes the .o files
clean:
	rm *.o skipping
