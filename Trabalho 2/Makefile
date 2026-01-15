CC = gcc
CFLAGS = -Wall -O2

OBJ = main.o grafo.o mochila.o dp.o guloso.o

tp2: $(OBJ)
	$(CC) $(CFLAGS) -o tp2 $(OBJ)

main.o: main.c grafo.h dp.h guloso.h
	$(CC) $(CFLAGS) -c main.c

grafo.o: grafo.c grafo.h
	$(CC) $(CFLAGS) -c grafo.c

mochila.o: mochila.c mochila.h
	$(CC) $(CFLAGS) -c mochila.c

dp.o: dp.c dp.h grafo.h mochila.h
	$(CC) $(CFLAGS) -c dp.c

guloso.o: guloso.c guloso.h grafo.h
	$(CC) $(CFLAGS) -c guloso.c

clean:
	rm -f tp2 *.o
