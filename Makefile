LIBRARY_NAME := libcorgidb.a

DIR_INC = ./include
SRCS = ./src/config.c ./src/corgi.c ./src/cstring.c ./src/dict.c ./src/error.c ./src/hash.c ./src/memory.c ./src/result.c ./src/transaction.c
OBJS := ${SRCS:.c=.o}

CC = gcc
CFLAGS = -Wall -I ${DIR_INC}

.PHONY: all

all: ${LIBRARY_NAME}

${OBJS}: ${SRCS}
	${CC} ${CFLAGS} -c $< -o $@

${LIBRARY_NAME}: ${OBJS}
	ar -rcs ${LIBRARY_NAME} ${OBJS}

clean:
	rm src/*.o
	rm ${LIBRARY_NAME}
