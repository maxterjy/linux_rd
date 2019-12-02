.PHONY: server client

all: server client

server:
	gcc server.c -o server -g -pthread

client:
	gcc client.c -o client -g -pthread	

clean:
	rm server client