CC = gcc
CFLAGS = -g -Wall -std=gnu99

# Primary targets

build: sortSeq sortProcess

# Executable targets
sortSeq: sortSeq.o quicksort.o
	${CC} ${CFLAGS} -o sortSeq sortSeq.o quicksort.o

sortProcess: sortProcess.o quicksort.o
	${CC} ${CFLAGS} -o sortProcess sortProcess.o quicksort.o

sortThread: sortThread.o quicksort.o
	${CC} ${CFLAGS} -o sortThread sortThread.o quicksort.o -lpthread

# Utility targets
sortThread.o: sortThread.c
	${CC} ${CFLAGS} -c sortThread.c

sortProcess.o: sortProcess.c
	${CC} ${CFLAGS} -c sortProcess.c

sortSeq.o: sortSeq.c
	${CC} ${CFLAGS} -c sortSeq.c

quicksort.o: quicksort.c quicksort.h
	${CC} ${CFLAGS} -c quicksort.c

# Add a clean target that silently removes the .o files
clean:
	@rm *.o sort{Seq,Process,Thread} 2> /dev/null || true
