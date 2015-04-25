#Compilateur et options de compilation
CC=gcc
CFLAGS=-Wall -ansi -pedantic -Wextra -g -lm

#Fichiers du projet
SOURCES=main.c graph.c calcul.c position.c
OBJECTS=$(SOURCES:.c=.o)

#Nom du programme
EXEC=main

$(EXEC): $(OBJECTS)
	$(CC)  $^ -o $(EXEC) $(CFLAGS)

.c.o:
	$(CC) -c $*.c $(CFLAGS)

clean:
	rm $(OBJECTS) $(EXEC)

