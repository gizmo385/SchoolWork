CC = gcc
CFLAGS = -g -Wall -std=c99 -I${INCLUDE}

INCLUDE=/home/cs352/fall14/assign11

all:	disAssemble

disAssemble: disAssemble.o processOpts.o
	${CC} ${CFLAGS}  -o disAssemble disAssemble.o processOpts.o

disAssemble.o: disAssemble.c disAssemble.h
	${CC} ${CFLAGS} -c disAssemble.c

processOpts.o: processOpts.c processOpts.h
	${CC} ${CFLAGS} -c processOpts.c

# Add a clean target that removes the .o files
clean:
	rm *.o disAssemble
