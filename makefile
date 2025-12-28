PROGRAM = rezta
CC      = gcc
CFLAGS  = -Wall -std=c99 -pedantic -g -DTTF
LDFLAGS = -lSDL2 -lSDL2_ttf -lm

all: $(PROGRAM) 

$(PROGRAM): sdl2testing.o bezier.o lista.o diccionariocurvas.o bola.o dibujar.o
	$(CC) sdl2testing.o bezier.o lista.o diccionariocurvas.o bola.o dibujar.o -o $(PROGRAM) $(LDFLAGS)

sdl2testing.o: sdl2testing.c 
	$(CC) $(CFLAGS)	-c sdl2testing.c	

bezier.o: bezier.c bezier.h
	$(CC) $(CFLAGS)	-c bezier.c

lista.o: lista.c lista.h
	$(CC) $(CFLAGS)	-c lista.c	

bolas.o: bola.c bola.h
	$(CC) $(CFLAGS)	-c bola.c		

diccionariocurvas.o: diccionariocurvas.c diccionariocurvas.h
	$(CC) $(CFLAGS)	-c diccionariocurvas.c		

dibujar.o: dibujar.c dibujar.h
	$(CC) $(CFLAGS)	-c dibujar.c		

clean:
	rm -vf *.o $(PROGRAM)			