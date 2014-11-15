CC = gcc
CFLAGS = -g -Wall -std=c99 -I${INCLUDE}

INCLUDE=/home/cs352/fall14/assign11

all:	disAssemble

disAssemble: disAssemble.o processOpts.o parser.o registers.o
	${CC} ${CFLAGS}  -o disAssemble disAssemble.o processOpts.o parser.o registers.o

disAssemble.o: disAssemble.c
	${CC} ${CFLAGS} -c disAssemble.c

processOpts.o: processOpts.c processOpts.h
	${CC} ${CFLAGS} -c processOpts.c

parser.o: parser.c parser.h
	${CC} ${CFLAGS} -c parser.c

registers.o: registers.c registers.h
	${CC} ${CFLAGS} -c registers.c


# Add a clean target that removes the .o files
clean:
	@rm *.o disAssemble 2> /dev/null || true
