# Makefile de exemplo (Manual do GNU Make)
     
CFLAGS = -Wall -std=c90 -g  # flags de compilacao
LDFLAGS = -lm

CC = gcc

# arquivos-objeto
	objects = teste.o libconjunto.o  liblef.o libfila.o 
     
teste: teste.o libfila.o
	$(CC) -o teste teste.o libfila.o $(LDFLAGS)

libfila.o: libfila.c
	$(CC) -c $(CFLAGS) libfila.c

teste.o: teste.c
	$(CC) -c $(CFLAGS) teste.c

clean:
	rm -f $(objects) teste


