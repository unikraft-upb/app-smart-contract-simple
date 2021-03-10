CFLAGS = -Wall -g

.PHONY: all

all: client

client: client.o sock_util.o timer.o

sock_util.o: sock_util.c

client.o: client.c

timer.o: timer.c

.PHONY: clean

clean:
	-rm -f client
	-rm -f client.o sock_util.o timer.o
	-rm -f *~
