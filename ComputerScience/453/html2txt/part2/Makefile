CC = gcc
CFLAGS = -g
CFLAGS += -Wall
CFLAGS += -std=gnu99

OS = $(shell uname -s)

# Load the proper library for Flex
ifeq ($(OS),Darwin)
	FLEX_LIB = -ll
endif

ifeq ($(OS),Linux)
	FLEX_LIB = -lfl
endif


myhtml2txt: myhtml2txt.l myhtml2txt.y
	bison -d myhtml2txt.y
	flex -o myhtml2txt.lex.c myhtml2txt.l
	${CC} ${CFLAGS} -o myhtml2txt myhtml2txt.tab.c myhtml2txt.lex.c ${FLEX_LIB}

.PHONY : clean
clean:
	/bin/rm -f myhtml2txt myhtml2txt.lex.c myhtml2txt.tab.c myhtml2txt.tab.h a.out
