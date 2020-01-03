all: control.o writer.o
	gcc -o control control.o
	gcc -o write writer.o

control.o: control.c
	gcc -c control.c

writer.o: writer.c
	gcc -c writer.c

clean:
	rm *.o
	rm *~

gdb: a.out
	gdb a.out
