CC = gcc
CFLAGS = -g -Wall -std=gnu99

# Primary targets

all: JacobiC JacobiJava

# Executable targets
JacobiJava: Jacobi.java
	javac Jacobi.java

JacobiC: Jacobi.o
	${CC} ${CFLAGS} -o Jacobi Jacobi.o -lpthread

# Add a clean target that silently removes the .o files
clean:
	@rm *.o *.class Jacobi 2> /dev/null || true
