# Makefile de exemplo (Manual do GNU Make)
     
CFLAGS = -Wall -std=c90 -g  # flags de compilacao
LDFLAGS = -lm

CC = gcc

# arquivos-objeto
	objects = teste.o liblef.o
     
teste: teste.o liblef.o
	$(CC) -o teste teste.o liblef.o $(LDFLAGS)

liblef.o: liblef.c
	$(CC) -c $(CFLAGS) liblef.c

teste.o: teste.c
	$(CC) -c $(CFLAGS) teste.c

clean:
	rm -f $(objects) teste


