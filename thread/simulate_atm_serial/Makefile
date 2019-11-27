.PHONY: demo io.o

demo: io.o
	gcc demo.c io.o -o demo -g3

io.o:
	gcc -g3 -c io.c -o io.o


clean:
	rm *.o demo
