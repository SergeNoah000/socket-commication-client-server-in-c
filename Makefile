CC = gcc
CFLAGS = -Wall -Wextra --no-warning

.PHONY: all compil run c

all:runnerc




#compil:programme

runnerc: 
	$(CC) $(CFLAGS) -o server server.c && $(CC) $(CFLAGS) -o client client.c  && $(CC) $(CFLAGS) -o server_full server_updated.c -lpthread && $(CC) $(CFLAGS) -o infinite_client client_updated.c


clean:
	rm -f server client infinite_client server_full  *.txt *.log

run:
	./server
