CFLAGS=-O2 -Wall -Wextra -pedantic

all: Interpreter Transpiler

%.o: %.c %.h
	gcc $(CFLAGS) -c -o $@ $<

interpreter.o: interpreter.c
	gcc $(CFLAGS) -c -o $@ $<

transpiler.o: transpiler.c
	gcc $(CFLAGS) -c -o $@ $<

Interpreter: input.o parser.o eli.o interpreter.o
	gcc $(CFLAGS) input.o parser.o eli.o interpreter.o -o Interpreter

Transpiler: input.o parser.o eli.o transpiler.o
	gcc $(CFLAGS) input.o parser.o eli.o transpiler.o -o Transpiler

clean:
	rm -f *.o Interpreter Transpiler
