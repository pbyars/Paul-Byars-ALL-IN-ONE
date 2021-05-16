CFLAGS = -Wall -c
CC = gcc

all: client.o server.o
	${CC} client.o -o client
	${CC} server.o -o server
client.o: client.c
	${CC} ${CFLAGS} client.c
server.o: server.c
	${CC} ${CFLAGS} server.c
clean:
	rm -f *.o client server
