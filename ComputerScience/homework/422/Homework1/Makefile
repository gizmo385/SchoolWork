CC = gcc
CFLAGS = -g -Wall -std=gnu99

# Primary targets

build: catalog

debug: clean catalog-debug

# Executable targets
sortSeq: sortSeq.o quicksort.o
	${CC} ${CFLAGS} -o sortSeq sortSeq.o quicksort.o

sortProcess: sortProcess.o quicksort.o
	${CC} ${CFLAGS} -o sortProcess sortProcess.o quicksort.o

sortThreads: sortThreads.o quicksort.o
	${CC} ${CFLAGS} -o sortThreads sortSeq.o quicksort.o

# Utility targets
quicksort.o: quicksort.c quicksort.h
	${CC} ${CFLAGS} -c quicksort.c

# Add a clean target that silently removes the .o files
clean:
	@rm *.o sort{Seq,Process,Thread} 2> /dev/null || true
