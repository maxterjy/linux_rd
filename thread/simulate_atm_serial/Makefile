.PHONY: demo io.o network.o all server

all: server

server: io.o network.o
	gcc -g3 server.c io.o network.o -o server

demo: io.o
	gcc demo.c io.o -o demo -g3

io.o:
	gcc -g3 -c io.c -o io.o

network.o:
	gcc -g3 -c network.c -o network.o

clean:
	rm *.o demo
