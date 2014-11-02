CC = gcc
CFLAGS = -g -Wall -std=c99 -I${INCLUDE}

LDFLAGS=/home/cs352/fall14/assign9
LIBS=shuffle

INCLUDE=/home/cs352/fall14/assign9


all:	cardDealer

cardDealer: cardDealer.o processOpts.o deal.o \
            ${LDFLAGS}/libshuffle.a ${INCLUDE}/shuffle.h
	${CC} ${CFLAGS} -o cardDealer cardDealer.o processOpts.o deal.o \
		-L${LDFLAGS} -l${LIBS}

cardDealer.o: cardDealer.c deal.h processOpts.o processOpts.h
	${CC} ${CFLAGS} -c cardDealer.c

# Add a target for processOpts.o
processOpts.o: processOpts.c processOpts.h
	${CC} ${CFLAGS} -c processOpts.c

# Add a target for deal.o
deal.o: deal.c deal.h processOpts.h
	${CC} ${CFLAGS} -c deal.c -L${LDFLAGS} -l${INCLUDE}/shuffle.h

# Add a clean target that removes the .o files
clean:
	rm *.o
